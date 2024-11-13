#ifndef CLOCK_H
#define CLOCK_H

#include <QMainWindow>
#include <QTime>
#include <QTimer>

namespace Ui {
class Clock;
}

class Clock : public QMainWindow
{
    Q_OBJECT

public:
    explicit Clock(QWidget *parent = nullptr);
    ~Clock();
    void init();
private slots:
    void updateDisplay();
    void on_btn_start_clicked();
    void on_btn_stop_clicked();
    void on_btn_pause_clicked();
    void on_btn_point_clicked();

private:
    Ui::Clock *ui;
    QTimer *pTimer;
    QTime baseTime;
    //显示的时间
    QString timeStr;
};

#endif // CLOCK_H
