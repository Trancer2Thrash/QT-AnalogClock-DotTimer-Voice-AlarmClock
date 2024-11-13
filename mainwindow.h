#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QLCDNumber>
#include<QLabel>
#include<QPushButton>
#include"sub.h"
#include"clocktime.h"
//背景
#include <QPalette>
//期日显示
#include <QDateTime>
#include<QTimerEvent>
#include <QMouseEvent>
#include<QSoundEffect>
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);//构造函数
    ~MainWindow();//析构函数
    void paintEvent(QPaintEvent *);//画时钟函数
    //切换窗口
    void dealsub();
    void changeback();
    void dealsub1();
    void changeback1();
    //背景
    void setback();
    //音效
    void soundoff();
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
private slots:
    void showtime();
private:
    //lcd表盘
    QLCDNumber *lcd;
    //计时器
    sub w;//子闹钟窗口
    ClockTime ct;//子计时器

    QPushButton b;//切换按钮sub
    QPushButton c;//关闭按钮
    QPushButton d;//切换背景
    QPushButton e;//音效
    QPushButton f;//切换按钮ct

    int mouse_flag1=0;
    int mouse_x1;
    int mouse_y1;
    QString current_color1;//颜色
    float Opacity1; //透明度控制
    QSoundEffect * m_clocksound;
    bool m_isPlaying;
};
#endif // MAINWINDOW_H
