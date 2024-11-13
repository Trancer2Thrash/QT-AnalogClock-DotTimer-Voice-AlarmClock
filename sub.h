#ifndef SUB_H
#define SUB_H

#include <QMainWindow>
#include<QPushButton>
//
#include<QLCDNumber>
#include<QTimerEvent>
#include<QTime>
#include<QDebug>
#include <QTextToSpeech>
#include <QMouseEvent>

namespace Ui {
class sub;
}

class sub : public QMainWindow
{
    Q_OBJECT

public:
    explicit sub(QWidget *parent = nullptr);
    ~sub();
    void sentsignal();//发送close信号
    void timerEvent(QTimerEvent *);//定时器
private slots:
    void on_StartBtn_clicked();
    void on_StopBtn_clicked();
signals:
    //
    void mysignal();
private:
    Ui::sub *ui;
    QPushButton b1;//切换按钮
    QPushButton c1;//关闭按钮
    //
    QLCDNumber *lcd;
    int clickedflag = 0;
    int showflag = 1;
    QString systimetext;
    QString current_color;//颜色
    float Opacity; //透明度控制
    //定义一个播报
    QTextToSpeech speech;
    QTextToSpeech::State state;//五中枚举类型 1说话 0准备
    //鼠标
    int mouse_flag=0;
    int mouse_x;
    int mouse_y;
    //鼠标按下
    void mousePressEvent(QMouseEvent *ev);
    //鼠标移动
    void mouseMoveEvent(QMouseEvent *ev);
};

#endif // SUB_H
