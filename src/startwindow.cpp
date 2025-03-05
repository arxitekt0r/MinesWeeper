#include "startwindow.h"

StartWindow::StartWindow(QWidget *parent)
    : QWidget{parent}
{
    this ->setMinimumSize(300,150);
    layout = new QVBoxLayout(this);
    this ->setLayout(layout);

    beginner= new QPushButton("Beginner");
    intermediate = new QPushButton("Intermediate");
    expert = new QPushButton("Expert");

    beginner->setStyleSheet("background: green; color: white;");
    intermediate->setStyleSheet("background: yellow; color: black;");
    expert->setStyleSheet("background: red; color: white;");


    beginner->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    intermediate->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    expert->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout -> addWidget(beginner);
    layout -> addWidget(intermediate);
    layout -> addWidget(expert);

    connect(beginner, &QPushButton::clicked, this, &StartWindow::handle_beginner);
    connect(intermediate, &QPushButton::clicked, this, &StartWindow::handle_intermediate);
    connect(expert, &QPushButton::clicked, this, &StartWindow::handle_expert);


}

StartWindow::~StartWindow()
{
    if(game_window)
    {
        delete game_window;
        game_window = nullptr;
    }
    if(layout)
    {
        delete layout;
        layout = nullptr;
    }
    if(beginner)
    {
        delete beginner;
        beginner = nullptr;
    }
    if(intermediate)
    {
        delete intermediate;
        intermediate = nullptr;
    }
    if(expert)
    {
        delete expert;
        expert = nullptr;
    }
}

void StartWindow::handle_beginner()
{
    current_level = 0;
    emit hide_signal();
    if(game_window)
    {
        delete game_window;
    }
    game_window = new GameWindow(9, 9, 10);
    connect_with_game();
    game_window ->show();
}

void StartWindow::handle_intermediate()
{
    current_level = 1;
    emit hide_signal();
    if(game_window)
    {
        delete game_window;
    }
    game_window = new GameWindow(12, 12, 20);
    connect_with_game();
    game_window ->show();
}

void StartWindow::handle_expert()
{
    current_level = 2;
    emit hide_signal();
    if(game_window)
    {
        delete game_window;
    }
    game_window = new GameWindow(16, 16, 50);
    connect_with_game();
    game_window ->show();
}

void StartWindow::connect_with_game()
{
    connect(game_window, &GameWindow::close_game, [this](){
        emit show_signal();
    });
    connect(game_window, &GameWindow::restart, [this](){
        game_window->close();
        if(current_level == 0)
        {
            handle_beginner();
        }
        else if(current_level == 1)
        {
            handle_intermediate();
        }
        else if(current_level == 2)
        {
            handle_expert();
        }
    });
}
