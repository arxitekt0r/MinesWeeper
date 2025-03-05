#ifndef START_WINDOW_H
#define START_WINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include "gamewindow.h"

class StartWindow : public QWidget
{
    Q_OBJECT
public:
    explicit StartWindow(QWidget *parent = nullptr);
    ~StartWindow();
private slots:
    void handle_beginner();
    void handle_intermediate();
    void handle_expert();

private:
    QPushButton* beginner;
    QPushButton* intermediate;
    QPushButton* expert;
    QVBoxLayout* layout;
    int current_level;
    GameWindow *game_window = nullptr;
signals:
    void hide_signal();
    void show_signal();

private:
    void connect_with_game();

};

#endif // START_WINDOW_H
