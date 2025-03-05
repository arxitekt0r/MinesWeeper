#include "mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(300, 150);
    startwindow = new StartWindow(this);
    this->setCentralWidget(startwindow);
    connect(startwindow, &StartWindow::hide_signal, [this](){
        this ->hide();
    });
    connect(startwindow, &StartWindow::show_signal, [this](){
        this ->show();
    });

}

MainWindow::~MainWindow()
{
    if(startwindow)
    {
        delete startwindow;
        startwindow = nullptr;
    }
}
