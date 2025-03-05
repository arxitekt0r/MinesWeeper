#include "gamewindow.h"
#include "timerwidget.h"

GameWindow::GameWindow(int row, int col, int mine, QWidget *parent)
    : QWidget{parent}
    , rows{row}
    , cols{col}
    , mines{mine}
{
    numbers.push_back(nullptr);
    for(int i = 1; i < 9; ++i)
    {
        QString path = ":/ico/icons/";
        path.append(QString::number(i));
        path.append(".ico");
        QIcon* ptr = new QIcon(path);
        numbers.push_back(ptr);
    }

    home_icon = new QIcon(":/ico/icons/home.ico");
    smile_emoji = new QIcon(":/ico/icons/smile.ico");
    sad_emoji = new QIcon(":/ico/icons/sad.ico");
    confused_emoji = new QIcon(":/ico/icons/confused.ico");
    bomb_icon = new QIcon(":/ico/icons/bomb.ico");
    flag_icon = new QIcon(":/ico/icons/flag.ico");
    unopened_icon = new QIcon(":/ico/icons/unopened.ico");
    opened_icon = new QIcon(":/ico/icons/opened.ico");

    int height = 50 + row * 25 + row + 1;
    int width = col * 25 + col + 1;
    this -> setMinimumSize(width, height);
    score = mines;
    open_cell_count = 0;

    general_layout = new QVBoxLayout(this);
    general_layout -> setSpacing(5);
    general_layout->setSizeConstraint(QLayout::SetMinimumSize);

    statistics_layout_container = new QWidget();
    statistics_layout_container->setMinimumSize(width, 50);
    statistics_layout_container->setMaximumHeight(50);

    game_board_layout_container = new QWidget();
    game_board_layout_container->setMinimumSize(width, height - 50);

    statistics_layout_container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    game_board_layout_container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    statistics_layout_container->setStyleSheet("background-color: Silver;");
    game_board_layout_container->setStyleSheet("background-color: Gray;");

    statistics_layout = new QHBoxLayout(statistics_layout_container);
    game_board_layout = new QGridLayout(game_board_layout_container);

    general_layout -> addWidget(statistics_layout_container);
    general_layout -> addWidget(game_board_layout_container);

    scoreLabel = new QLabel();
    set_score();
    scoreLabel->setAlignment(Qt::AlignCenter);
    QFont font("Arial", 10, QFont::Bold);
    scoreLabel->setFont(font);
    scoreLabel->setStyleSheet("QLabel {"
                              "color: #00FF00;"
                              "background-color: #000000;"
                              "border: 2px solid #FFFFFF;"
                              "}");
    scoreLabel->setMinimumSize(50, 35);
    scoreLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    statistics_layout ->addWidget(scoreLabel);


    go_home = new QPushButton();
    go_home ->setIcon(*home_icon);
    go_home ->setStyleSheet("QLabel {"
                           "color: #00FF00;"
                           "background-color: #000000;"
                           "border: 2px solid #FFFFFF;"
                           "}");
    go_home -> setMinimumSize(35, 35);
    go_home ->resize(35,35);
    go_home -> setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    statistics_layout ->addWidget(go_home);
    connect(go_home, &QPushButton::clicked,this, &GameWindow::handle_close_game);


    emoji = new QPushButton();
    emoji ->setIcon(*confused_emoji);
    emoji ->setStyleSheet("QLabel {"
                         "color: #00FF00;"
                         "background-color: #000000;"
                         "border: 2px solid #FFFFFF;"
                         "}");
    emoji -> setMinimumSize(go_home->size());
    emoji ->resize(go_home ->size());
    emoji -> setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    statistics_layout ->addWidget(emoji);
    connect(emoji, &QPushButton::clicked, this, &GameWindow::handle_restart);

    gameTimer = new TimerWidget();
    gameTimer ->setMinimumSize(80, 35);
    gameTimer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(this, &GameWindow::end, gameTimer, &TimerWidget::stopTimer);
    statistics_layout->addWidget(gameTimer);

    this ->setMaximumSize(this ->size());

    for (int i = 0; i < rows; ++i)
    {
        QList<CustomButton*> rowList;
        for (int j = 0; j < cols; ++j)
        {
            CustomButton* btn = new CustomButton();
            btn->setIcon(*unopened_icon);
            btn->setStyleSheet("QLabel {"
                               "color: silver;"
                               "background-color: silver;"
                               "border: 2px solid #FFFFFF;"
                               "}");
            btn->setMinimumSize(25, 25);
            btn->resize(25, 25);
            btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            btn->installEventFilter(this);
            rowList.append(btn);
            game_board_layout->addWidget(btn, i, j);
        }
        cells.append(rowList);
    }
    matrix.resize(rows, QVector<int>(cols, 0));
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            matrix[i][j] = 0;
        }
    }
    open_or_unopen.resize(rows, QVector<bool>(cols, false));
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            matrix[i][j] = false;
        }
    }

    fill_mines();
    fill_mines_count();
    connect_button_clicks();
    this -> resize(width, height);
    connect(this, &GameWindow::start, this, &GameWindow::startTimer);
    emit start();
}

GameWindow::~GameWindow()
{
}

void GameWindow::startTimer()
{
    gameTimer->show();
}

void GameWindow::handle_close_game()
{
    emit close_game();
    this -> hide();
}

void GameWindow::handle_restart()
{
    emit restart();
}

bool GameWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::Resize)
    {
        QPushButton* btn = qobject_cast<QPushButton*>(watched);
        if (btn)
        {
            int side = qMin(btn->width(), btn->height());
            btn->resize(side, side);
            btn->setIconSize(QSize(side - 4, side - 4));
        }
    }
    return QWidget::eventFilter(watched, event);
}

void GameWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    adjustButtonSizes();
}

void GameWindow::set_score()
{
    QString text = QString::number(score);
    scoreLabel->setText(text);
}


void GameWindow::adjustButtonSizes()
{
    int side = qMin(game_board_layout_container->width() / cols,
                    game_board_layout_container->height() / rows);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            QPushButton* btn = cells[i][j];
            btn->setFixedSize(side, side);
            btn->setIconSize(QSize(side - 4, side - 4));
        }
    }
}

void GameWindow::fill_mines()
{
    int mine_count = mines;
    QRandomGenerator *random = QRandomGenerator::global();

    while (mine_count > 0)
    {
        int i = random ->bounded(rows);
        int j = random ->bounded(cols);
        if (matrix[i][j] != -1)
        {
            matrix[i][j] = -1;
            --mine_count;
        }
    }
}

void GameWindow::fill_mines_count()
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (matrix[i][j] == 0)
            {
                for (int l = qMax(i-1,0); l <= qMin(i+1, rows-1); ++l)
                {
                    for (int m = qMax(j - 1, 0); m <= qMin(j+1, cols-1); ++m)
                    {
                        if (matrix[l][m] == -1)
                        {
                            ++matrix[i][j];
                        }
                    }
                }
            }
        }
    }
}

void GameWindow::open_board()
{
    for(int i = 0; i < cells.size(); ++i)
    {
        for(int j = 0; j < cells[0].size(); ++j)
        {
            QSize currentIconSize = cells[i][j]->iconSize();
            if(matrix[i][j] == -1)
            {
                cells[i][j] -> setIcon(*bomb_icon);
                cells[i][j] -> setIconSize(currentIconSize);
            }
            else if(matrix[i][j] == 0)
            {
                cells[i][j] -> setIcon(*opened_icon);
                cells[i][j] -> setIconSize(currentIconSize);
            }

            else
            {
                cells[i][j]->setIcon(*numbers[matrix[i][j]]);
                cells[i][j] -> setIconSize(currentIconSize);
            }
        }
    }
}

void GameWindow::connect_button_clicks()
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            connect(cells[i][j], &CustomButton::leftClicked, [this, i, j]() {
                open_cell(i, j);
            });
            connect(cells[i][j], &CustomButton::rightClicked, [this, i, j]() {
                QSize currentIconSize = cells[i][j]->iconSize();
                cells[i][j] ->setIcon(*flag_icon);
                cells[i][j] -> setIconSize(currentIconSize);
                score--;
                set_score();
            });
        }
    }
}

void GameWindow::disconnect_button_clicks()
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            disconnect(cells[i][j], &CustomButton::leftClicked, nullptr, nullptr);
            disconnect(cells[i][j], &CustomButton::rightClicked, nullptr, nullptr);
        }
    }
}

void GameWindow::open_cell(int i, int j)
{
    if (matrix[i][j] == -1)
    {
        disconnect_button_clicks();
        emoji->setIcon(*sad_emoji);
        open_board();
        emit end();
        return;
    }
    else if (matrix[i][j] > 0 && matrix[i][j] < 9)
    {
        QSize currentIconSize = cells[i][j]->iconSize();
        cells[i][j]->setIcon(*numbers[matrix[i][j]]);
        cells[i][j]->setIconSize(currentIconSize);
        disconnect(cells[i][j], &CustomButton::leftClicked, nullptr, nullptr);
        disconnect(cells[i][j], &CustomButton::rightClicked, nullptr, nullptr);
        open_or_unopen[i][j] = true;
        open_cell_count++;
    }
    else if (matrix[i][j] == 0)
    {
        open_cell_helper(i, j);
    }
    if (open_cell_count == rows * cols - mines)
    {
        disconnect_button_clicks();
        emoji->setIcon(*smile_emoji);
        open_board();
        emit end();
        return;
    }
}

void GameWindow::open_cell_helper(int i, int j)
{
    if (i < 0 || i >= rows || j < 0 || j >= cols || matrix[i][j] == -1 || open_or_unopen[i][j] == true)
    {
        return;
    }

    QSize currentIconSize = cells[i][j]->iconSize();
    if (matrix[i][j] == 0)
    {
        cells[i][j]->setIcon(*opened_icon);
        cells[i][j]->setIconSize(currentIconSize);
    }
    else if (matrix[i][j] > 0 && matrix[i][j] < 9)
    {
        cells[i][j]->setIcon(*numbers[matrix[i][j]]);
        cells[i][j]->setIconSize(currentIconSize);

    }
    disconnect(cells[i][j], &CustomButton::leftClicked, nullptr, nullptr);
    disconnect(cells[i][j], &CustomButton::rightClicked, nullptr, nullptr);
    open_cell_count++;
    open_or_unopen[i][j] = true;

    if (matrix[i][j] == 0)
    {
        for (int ii = -1; ii <= 1; ++ii)
        {
            for (int jj = -1; jj <= 1; ++jj)
            {
                if (ii != 0 || jj != 0)
                {
                    open_cell_helper(i + ii, j + jj);
                }
            }
        }
    }
    if (open_cell_count == rows * cols - mines)
    {
        disconnect_button_clicks();
        emoji->setIcon(*smile_emoji);
        open_board();
        emit end();
        return;
    }
}

