#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QResizeEvent>
#include <QSize>
#include <QPushButton>
#include <QLabel>
#include <QTime>
#include <QElapsedTimer>
#include <QVector>
#include <QList>
#include <QRandomGenerator>
#include <QDebug>
#include <QString>
#include <QMouseEvent>
#include "timerwidget.h"
#include "custombutton.h"

class GameWindow : public QWidget
{
    Q_OBJECT
public:
    explicit GameWindow(int row, int col, int mine, QWidget *parent = nullptr);
    ~GameWindow();

private:
    int rows;
    int cols;
    int mines;
    QVBoxLayout* general_layout;

    QHBoxLayout* statistics_layout;
    QWidget* statistics_layout_container;

    QGridLayout* game_board_layout;
    QWidget* game_board_layout_container;

private:
    TimerWidget* gameTimer;
    QLabel* scoreLabel;
    QPushButton* emoji;
    QPushButton* go_home;
    QList<QList<CustomButton*>> cells;
    QList<QList<int>> matrix;
    QList<QList<bool>> open_or_unopen;
    int score;
    int open_cell_count;

private slots:
    void startTimer();
    void handle_close_game();
    void handle_restart();

private:
    QVector<QIcon*> numbers;
    QIcon* home_icon;
    QIcon* smile_emoji;
    QIcon* sad_emoji;
    QIcon* confused_emoji;
    QIcon* bomb_icon;
    QIcon* flag_icon;
    QIcon* unopened_icon;
    QIcon* opened_icon;

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void set_score();
    void adjustButtonSizes();
    void fill_mines();
    void fill_mines_count();
    void open_board();
    void connect_button_clicks();
    void disconnect_button_clicks();
    void open_cell(int i, int j);
    void open_cell_helper(int i, int j);


signals:
    void start();
    void end();
    void close_game();
    void restart();
};

#endif
