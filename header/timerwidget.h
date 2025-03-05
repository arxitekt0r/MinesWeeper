#ifndef TIMERWIDGET_H
#define TIMERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QElapsedTimer>

class TimerWidget : public QWidget {
    Q_OBJECT

public:
    TimerWidget(QWidget *parent = nullptr);

private:
    QLabel *timerLabel;
    QElapsedTimer elapsedTimer;
    QTimer *updateTimer;

    void setupUI();
    void setupTimer();
private slots:
    void updateTime();
public slots:
    void startTimer();
    void stopTimer();
};

#endif
