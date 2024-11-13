#include "clock.h"
#include "ui_clock.h"

Clock::Clock(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Clock)
{
    ui->setupUi(this);
    this->init();
}

Clock::~Clock()
{
    delete this->pTimer;
    delete ui;
}

//初始化
void Clock::init()
{
    //默认显示
    this->ui->lcd_play->display("00:00:00:000");

    this->pTimer = new QTimer;
    //绑定定时器的信号
    connect(this->pTimer,SIGNAL(timeout()),this,SLOT(updateDisplay()));
}

//开始
void Clock::on_btn_start_clicked()
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
void Clock::on_btn_stop_clicked()
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
void Clock::on_btn_pause_clicked()
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
void Clock::on_btn_point_clicked()
{
    //将timeStr添加到列表
    this->ui->tb_display->append(this->timeStr);
}
//更新时间
void Clock::updateDisplay()
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
