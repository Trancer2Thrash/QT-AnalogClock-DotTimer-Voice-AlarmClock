#include "clocktime.h"
#include "ui_clocktime.h"
#include <QString>
#include<windows.h>
ClockTime::ClockTime(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClockTime)
{
    ui->setupUi(this);
    this->init();
    connect(ui->btn_switch,&QPushButton::clicked,this,&ClockTime::on_btn_switch_clicked);
    this->setStyleSheet("background:#EEE8AA");
    this->setWindowFlags(Qt::FramelessWindowHint);      //设置为无边框窗口
    this->setWindowOpacity(0.8);
    //置顶
    ::SetWindowPos(HWND(this->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

    this->installEventFilter(this);//捕获鼠标
}

ClockTime::~ClockTime()
{
    delete this->pTimer;
    delete ui;
}
//初始化
void ClockTime::init()
{
    //默认显示
    this->ui->lcd_play->display("00:00:00:000");
    this->pTimer = new QTimer;
    //绑定定时器的信号
    connect(this->pTimer,SIGNAL(timeout()),this,SLOT(updateDisplay()));
}
//开始
void ClockTime::on_btn_start_clicked()
{
    //获取当前时间
    this->baseTime = this->baseTime.currentTime();
    this->pTimer->start(1);

    //重置状态
    if(this->ui->btn_stop->text() != "停止")
    {
        this->ui->btn_stop->setText("停止");
    }
    if(this->ui->btn_pause->text() != "暂停")
    {
        this->ui->btn_pause->setText("暂停");
    }
}
//结束
void ClockTime::on_btn_stop_clicked()
{
    if(this->ui->btn_stop->text() == "停止")
    {
        this->ui->btn_stop->setText("清空");
        this->pTimer->stop();
    }
    else if(this->ui->btn_stop->text() == "清空")
    {
        this->ui->btn_stop->setText("停止");
        this->ui->tb_display->clear();
        this->ui->lcd_play->display("00:00:00:000");
    }

}
//暂停
void ClockTime::on_btn_pause_clicked()
{
    static QTime pauseTime;
    if(this->ui->btn_pause->text() == "暂停")
    {
        pauseTime = QTime::currentTime();
        this->ui->btn_pause->setText("继续");
        this->pTimer->stop();
    }
    else if(this->ui->btn_pause->text() == "继续")
    {
        //进行差值计算
        QTime cut = QTime::currentTime();
        int t = pauseTime.msecsTo(cut);
        this->baseTime = this->baseTime.addMSecs(t);
        this->ui->btn_pause->setText("暂停");
        this->pTimer->start();
    }
}
//打点
void ClockTime::on_btn_point_clicked()
{
    //将timeStr添加到列表
    this->ui->tb_display->append(this->timeStr);
}
//更新时间
void ClockTime::updateDisplay()
{
    /*
     * 1.点击开始后获取到当前的时间并且赋值给baseTime
     * 2.启动定时器,间隔1s
     * 3.槽函数中再次获取当前的时间currTime
     * 4.计算两个时间的差值t
     * 5.声明一个showTime对象给他加上t的差值
     * 6.格式化后设置显示
     */
    QTime currTime = QTime::currentTime();
    int t = this->baseTime.msecsTo(currTime);
    QTime showTime(0,0,0,0);
    showTime = showTime.addMSecs(t);
    this->timeStr = showTime.toString("hh:mm:ss:zzz");
    this->ui->lcd_play->display(timeStr);
}
//切换窗口
void ClockTime::on_btn_switch_clicked()
{
    emit mysignal1();
}

bool ClockTime::eventFilter(QObject *watched, QEvent *event)
{

    if (event->type() == QEvent::MouseButtonPress) {

        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        mouse_flag2 = 1;
        mouse_x2 = mouseEvent->x();
        mouse_y2 = mouseEvent->y();
        return true;
    } else if (event->type() == QEvent::MouseMove && mouse_flag2 == 1) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        int x = this->x() + mouseEvent->x();
        int y = this->y() + mouseEvent->y();
        setGeometry(x - mouse_x2, y - mouse_y2, 727, 614);
        return true;
    }
    return QMainWindow::eventFilter(watched, event);
}
