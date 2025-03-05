#include "TimerWidget.h"
#include <QFont>

TimerWidget::TimerWidget(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupTimer();
}

void TimerWidget::setupUI()
{
    timerLabel = new QLabel("Time: 0 s", this);
    timerLabel->setAlignment(Qt::AlignCenter);

    QFont font("Arial", 10, QFont::Bold);
    timerLabel->setFont(font);
    timerLabel->setStyleSheet("QLabel {"
                              "color: #00FF00;"
                              "background-color: #000000;"
                              "border: 2px solid #FFFFFF;"

                              "}");
    timerLabel->setMinimumSize(80, 35);
}

void TimerWidget::setupTimer()
{
    elapsedTimer.start();
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &TimerWidget::updateTime);
    updateTimer->start(1000);
}

void TimerWidget::stopTimer()
{
    if (updateTimer->isActive())
    {
        updateTimer->stop();
    }
}

void TimerWidget::updateTime()
{
    qint64 elapsedSeconds = elapsedTimer.elapsed() / 1000;
    timerLabel->setText(QString("Time: %1 s").arg(elapsedSeconds));
}

void TimerWidget::startTimer()
{
    updateTimer->start();
}
