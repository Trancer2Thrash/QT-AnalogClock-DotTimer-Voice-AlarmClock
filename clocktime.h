#ifndef CLOCKTIME_H
#define CLOCKTIME_H

#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include<QTimerEvent>
#include <QMouseEvent>
namespace Ui {
class ClockTime;
}
QT_END_NAMESPACE
class ClockTime : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClockTime(QWidget *parent = nullptr);
    ~ClockTime();
    void init();
private slots:
    void updateDisplay();
    void on_btn_start_clicked();
    void on_btn_stop_clicked();
    void on_btn_pause_clicked();
    void on_btn_point_clicked();
    void on_btn_switch_clicked();
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
signals:
    void mysignal1();
private:
    Ui::ClockTime *ui;
    QTimer *pTimer;
    QTime baseTime;
    //显示的时间
    QString timeStr;

    int mouse_flag2=0;
    int mouse_x2;
    int mouse_y2;
};

#endif // CLOCKTIME_H
