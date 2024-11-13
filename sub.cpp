#include "sub.h"
#include "ui_sub.h"
#include <windows.h>
#include<QDebug>
sub::sub(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::sub)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/new/prefix1/OIP-C.jpg"));//设置图标
    this->setWindowTitle("Alarm Clock");//设置窗口标题
    this->move(100,200);

    b1.setParent(this);//指按钮定父对象
    b1.setText("Move clock->");//设置按钮内容
    b1.setGeometry(0,100,140,40);//设置按钮位置
    //设置按钮风格
    b1.setStyleSheet("QPushButton{background-color: rgba(205,214,216,0);color:rgb(0,0,0);}");
    //连接信号与槽，当点击按钮的时候跳转到发送信号函数，主窗口接收，再执行changeback()函数，即实现了跳回主窗口
    connect(&b1,&QPushButton::clicked,this,&sub::sentsignal);

    //关闭按钮
    c1.setParent(this);//指定按钮父对象
    c1.setGeometry(-27,130,160,40);//设置按钮位置 (0,0)位置开始显示160*40的大小
    c1.setText("Close->");//设置按钮内容
    //设置按钮样式，rgba前三个参数同rgb，最后一个参数表示不透明度，0~1之间
    c1.setStyleSheet("QPushButton{background-color: rgba(205,214,216,0);color:rgb(0,0,0);}");
    //松开时关闭
    connect(&c1,&QPushButton::released,this,&sub::close);

    //调整ui界面
    this->setFixedSize(570,300);
    ui->TimeEdit->setStyleSheet("border: 1px solid whitle;");
    ui->TimeEdit->resize(250,75);
    ui->TimeEdit->move(300,20);
    ui->StartBtn->resize(80,40);
    ui->StartBtn->move(300,115);
    ui->StopBtn->resize(80,40);
    ui->StopBtn->move(300+ui->StartBtn->width()+20,115);
    ui->StopBtn->setEnabled(false);
    ui->MsgEdit->resize(this->width()-40,90);
    ui->MsgEdit->move(20,this->height()-20-90);
    ui->MsgEdit->setPlaceholderText("请在这里输入提醒消息");
    ui->TimeEdit->setDisplayFormat("HH:mm:ss");

    lcd = new QLCDNumber(this);
    lcd->resize(250,75);
    lcd->move(20,20);
    lcd->setSegmentStyle(QLCDNumber::Flat);
    //边框白色实线 文本深灰色 背景棕色
    lcd->setStyleSheet("border: 1px solid white; color: #333333; background-color: #BDB76B;");
    lcd->setDigitCount(8);
    //启动定时器 一秒刷新
    startTimer(1000);
    //无边框
    setWindowFlags(Qt::FramelessWindowHint);
    Opacity = 0.8; // 透明度设置
    current_color = "background:#EEE8AA"; //当前背景色设置
    this->setStyleSheet(current_color);                 //设置背景颜色
    this->setWindowFlags(Qt::FramelessWindowHint);      //设置为无边框窗口
    this->setWindowOpacity(Opacity);                    //设置不透明度
    //置顶
    ::SetWindowPos(HWND(this->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

}

sub::~sub()
{
    delete ui;
}
void sub::sentsignal()//发送信号
{
    emit mysignal();
}

//重写定时器事件
void sub::timerEvent(QTimerEvent *)
{
    //1.获取系统时间
    QTime systime = QTime::currentTime();
    //2.将系统时间转换为字符串
    systimetext = systime.toString("HH:mm:ss");

    if(clickedflag == 1 )
    {
        if(speech.state()!=1)//1状态是speak
        {
            if(ui->TimeEdit->text()==systimetext)
            {
                qDebug()<<ui->MsgEdit->toPlainText();
                speech.say(ui->MsgEdit->toPlainText());

            }
        }
    }
    if(showflag)//冒号一秒钟闪烁一次
    {
        systimetext[2] = ':';
        systimetext[5] = ':';
        showflag = false;
    }
    else
    {
        systimetext[2] = ' ';
        systimetext[5] = ' ';
        showflag = true;
    }
    //将时间字符串展示到LCD中
    lcd->display(systimetext);
}

void sub::on_StartBtn_clicked()
{
    clickedflag = 1;
    //开始后禁用输入时间
    ui->TimeEdit->setEnabled(false);
    ui->MsgEdit->setEnabled(false);
    ui->StartBtn->setEnabled(false);
    ui->StopBtn->setEnabled(true);
}
void sub::on_StopBtn_clicked()
{
    clickedflag = 0;
    ui->TimeEdit->setEnabled(true);
    ui->MsgEdit->setEnabled(true);
    ui->StartBtn->setEnabled(true);
    ui->StopBtn->setEnabled(false);
}

//鼠标按下
void sub::mousePressEvent(QMouseEvent *ev)
{
    //获取初始坐标
    mouse_flag=1;
    mouse_x=ev->x();
    mouse_y=ev->y();
}
//鼠标移动
void sub::mouseMoveEvent(QMouseEvent *ev)
{
    if(mouse_flag==1)//与上面联动 当鼠标按下 改变坐标
    {
        int x=this->x()+ev->x();
        int y=this->y()+ev->y();
        //后面两个数字的大小要与窗口大小一致，否则鼠标点击后窗口大小会变
        setGeometry(x-mouse_x,y-mouse_y,600,450);
        //当前坐标减去初始坐标 获得相对坐标
    }
}










