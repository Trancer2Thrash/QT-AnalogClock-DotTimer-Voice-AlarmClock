#include "mainwindow.h"
#include<QPainter>
#include<QPen>
#include<QTime>
#include<QTimer>
#include<QLabel>
#include<QPushButton>
#include<QLCDNumber>
#include<QMessageBox>
#include<windows.h>
#include<QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowIcon(QIcon(":/new/prefix1/v2-d858191577356128b31c88e186eea0db_r.jpg"));//设置图标
    QTimer *timer = new QTimer(this);//产生一个定时器
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));//关联定时器的信号与槽 一秒钟重绘
    timer->start(1000);//开始定时器，每一秒钟更新一次
    setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(1010,700);//窗口大小

    //lcd显示屏
    lcd=new QLCDNumber(this);//创建一个lcd液晶显示器
    lcd->setSegmentStyle(QLCDNumber::Filled);//设置显示器风格
    lcd->move(400,520);//移动显示器位置
    lcd->setDigitCount(8);//设置所显示的位数为8位
    lcd->resize(200,50);//设置显示器大小
    QTimer *timer1=new QTimer(this);//产生一个定时器
    timer1->start(1000);//开始定时器，每一秒钟更新显示时间
    connect(timer1,SIGNAL(timeout()),this,SLOT(showtime()));//关联定时器的信号与槽，1s到即更新显示时间
    showtime();//调用显示时间函数

    Opacity1 = 0.8; // 透明度设置
    current_color1 = "background:#EEE8AA"; //当前背景色设置
    this->setStyleSheet(current_color1);
    this->setWindowFlags(Qt::FramelessWindowHint);      //设置为无边框窗口
    this->setWindowOpacity(Opacity1);                    //设置不透明度
    //置顶
    ::SetWindowPos(HWND(this->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

    //添加按钮转换窗口sub
    b.setParent(this);//指定按钮父对象
    b.setGeometry(0,0,160,40);//设置按钮位置 (0,0)位置开始显示160*40的大小
    b.setText("Alarm Clock->");//设置按钮内容
    b.setStyleSheet("QPushButton{background-color: rgba(205,214,216,0);color:rgb(0,0,0);}");//设置按钮样式，rgba前三个参数同rgb，最后一个参数表示不透明度，0~1之间
    connect(&b,&QPushButton::released,this,&MainWindow::dealsub);//鼠标松开时触发，跳转到处理子窗口函数
    void(sub::*funsignal)()=&sub::mysignal; //定义函数指针
    connect(&w,funsignal,this,&MainWindow::changeback);//关联信号与槽，实现从子窗口返回到主窗口

    //添加按钮转换窗口ct
    f.setParent(this);//指定按钮父对象
    f.setGeometry(-18,120,160,40);
    f.setText("Timer->");//设置按钮内容
    f.setStyleSheet("QPushButton{background-color: rgba(205,214,216,0);color:rgb(0,0,0);}");//设置按钮样式，rgba前三个参数同rgb，最后一个参数表示不透明度，0~1之间
    connect(&f,&QPushButton::released,this,&MainWindow::dealsub1);//鼠标松开时触发，跳转到处理子窗口函数
    void(ClockTime::*funsignal1)()=&ClockTime::mysignal1; //定义函数指针
    connect(&ct,funsignal1,this,&MainWindow::changeback1);

    //关闭按钮
    c.setParent(this);//指定按钮父对象
    c.setGeometry(-19,30,160,40);
    c.setText("Close->");//设置按钮内容
    c.setStyleSheet("QPushButton{background-color: rgba(205,214,216,0);color:rgb(0,0,0);}");//设置按钮样式，rgba前三个参数同rgb，最后一个参数表示不透明度，0~1之间
    //松开时关闭
    connect(&c,&QPushButton::released,this,&MainWindow::close);


    //点击按钮换背景
    d.setParent(this);//指定按钮父对象
    d.setGeometry(-14,60,160,40);
    d.setText("Change->");//设置按钮内容
    d.setStyleSheet("QPushButton{background-color: rgba(205,214,216,0);color:rgb(0,0,0);}");
    connect(&d,&QPushButton::released,this,&MainWindow::setback);

    this->installEventFilter(this);//捕获鼠标


    //点击关闭音效
    e.setParent(this);//指定按钮父对象
    e.setGeometry(-16,90,160,40);
    e.setText("Sound->");//设置按钮内容
    e.setStyleSheet("QPushButton{background-color: rgba(205,214,216,0);color:rgb(0,0,0);}");
    connect(&e,&QPushButton::clicked,this,&MainWindow::soundoff);
}

MainWindow::~MainWindow()
{

}

void MainWindow::paintEvent(QPaintEvent *)
{
    static const QPoint hour[3] = {
        QPoint(14, 15),
        QPoint(-14, 15),
        QPoint(0, -110)
    };
    static const QPoint minute[3] = {
        QPoint(11, 13),
        QPoint(-11, 13),
        QPoint(0, -170)
    };
    static const QPoint second[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -210)
    };
    int size=qMin(width(),height());
    QTime time=QTime::currentTime();//获取系统当前时间
    QPainter p(this);//创建画家对象
    p.setRenderHint(QPainter::Antialiasing);//防止图形走样
    p.translate(width()/2,height()/2);//平移坐标系置中心
    p.scale(size/600.0,size/600.0);//缩放

    QBrush brush;//定义画刷
    brush.setColor(QColor(0, 0, 0,120));//设置画刷颜色 时针
    brush.setStyle(Qt::SolidPattern);//设置样式


    QPen pen;//定义画笔
    pen.setWidth(18);//设置画笔宽度
    pen.setColor(QColor(20, 20, 20, 0.22));//rgb设置颜色 表盘周围一圈
    pen.setStyle(Qt::SolidLine);//设置风格
    p.setPen(pen);//将画笔交给画家
    p.drawEllipse(QPoint(0,0),280,280);//画圆
    pen.setWidth(160);//设置画笔宽度
    p.setPen(pen);//将画笔交给画家
    p.drawEllipse(QPoint(0,0),160,160);//画圆
    //画时针
    p.setBrush(brush);//将画刷交给画家
    p.setPen(Qt::NoPen);
    p.save();//保存当下状态
    p.rotate(30.0*(time.hour()+time.minute()/60.0));//图形旋转，以原点为旋转中心，顺时针水平旋转对应时针的角度
    p.drawConvexPolygon(hour,3);//画时针这个凸多边形，第一个参数为所有的点，第二个参数为点的个数
    p.restore();//恢复上一次保存的结果，和save()成对出现

    //绘制小时线
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(5);
    pen.setColor(Qt::black);
    p.setPen(pen);
    for(int i=0;i<12;i++)
    {
        p.drawLine(0,268,0,276);//画小时线
        p.drawText(-5,-235,QString::number(i));//表明小时数
        p.rotate(30);//每画一次旋转30度
    }

    //画分针
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(112, 127, 127, 121)); //分针
    p.save();//保存当下状态
    p.rotate(6.0*(time.minute()+time.second()/60.0));//顺时针旋转至分针的位置
    p.drawConvexPolygon(minute,3);//画分针这个凸多边形，第一个参数为所有的点，第二个参数为点的个数
    p.restore();//恢复上一次保存的结果，和save()成对出现

    //绘制分钟线
    pen.setStyle(Qt::SolidLine);
    pen.setColor(QColor(0,0,0));
    pen.setWidth(1);
    p.setPen(pen);
    for(int i=0;i<60;i++)
    {
        if((i%5)!=0)
        p.drawLine(0,265,0,276);//5的倍数时不画，因为有小时线
        p.rotate(6);//每画一次旋转6度
    }

    //画秒线
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(255, 0, 0,100));
    p.save();
    p.rotate(6*time.second());//顺时针旋转至秒针的位置
    p.drawConvexPolygon(second, 3);//画秒针这个凸多边形，第一个参数为所有的点，第二个参数为点的个数
    p.restore();

    //画圆心
    p.setBrush(Qt::black);
    p.setPen(Qt::white);
    p.save();
    p.drawEllipse(QPoint(0,0),3,3);//画圆心
    p.restore();

    //表明上午还是下午
    p.setPen(Qt::black);
    QFont font1;//调整字体
    font1.setFamily("Microsoft YaHei");
    font1.setPointSize(20);//字体点大小
    font1.setItalic(true);//字体是否斜体
    p.setFont(font1);
    if(time.hour()>=12)
    p.drawText(-25,90,"PM");//画文本区
    else
    p.drawText(-20,90,"AM");//画文本区

    //期日显示
    QDate date = QDate::currentDate();
    QString date1=date.toString("yyyy-MM-dd");
    date1.remove(QChar('""'));
    p.setPen(Qt::black);
    QFont font;//调整字体
    font.setFamily("Microsoft YaHei");
    font.setPointSize(20);//字体点大小
    font.setItalic(true);//字体是否斜体
    p.setFont(font);
    p.drawText(-70,120,date1);

}


void MainWindow::showtime()
{
    QTime time1=QTime::currentTime();//获取当前时间
    QString text=time1.toString("HH:mm:ss");//定义时间显示格式
    if((time1.second()%2)==0)
    {
        text[2]=' ';
        text[5]=' ';//每2s冒号消失一次
    }
    lcd->display(text);//lcd显示时间
}

void MainWindow::dealsub()
{
    w.show();//显示子窗口
    this->hide();//主窗口隐藏
}

void MainWindow::changeback()
{
    w.hide();//子窗口隐藏
    this->show();//显示主窗口L
    w.close();
}

void MainWindow::dealsub1()
{
    ct.show();//显示子窗口
    this->hide();//主窗口隐藏
}

void MainWindow::changeback1()
{
    ct.hide();//子窗口隐藏
    this->show();//显示主窗口L
    ct.close();
}

void MainWindow::setback()
{
    //封装
    QMessageBox msgBox;
    msgBox.setWindowTitle("选项");
    msgBox.setText("请选择一个选项：");
    QPushButton *option1 = msgBox.addButton("壁纸1", QMessageBox::ActionRole);
    QPushButton *option2 = msgBox.addButton("壁纸2", QMessageBox::ActionRole);
    QPushButton *option3 = msgBox.addButton("纯色背景", QMessageBox::ActionRole);
    QPushButton *option4 = msgBox.addButton("默认背景", QMessageBox::ActionRole);
    msgBox.setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    //msgBox.move(0,60);
    msgBox.exec();

    if (msgBox.clickedButton() == option1)
    {
        this->setStyleSheet("background-color: rgb(245, 245, 245);"
                            "background-image: url(:/new/prefix1/3.jpg);");
        this->setWindowOpacity(0.9);
    }
    else if (msgBox.clickedButton() == option2)
    {
        // 处理选项2
        this->setStyleSheet("background-color: rgb(245, 245, 245);"
                            "background-image: url(:/new/prefix1/2.jpg);");

    }
    else if (msgBox.clickedButton() == option3)
    {
        this->setStyleSheet("background-color: rgb(245, 245, 245);");// 设置为白色
        this->setWindowOpacity(0.8);
    }
    else if (msgBox.clickedButton() == option4)
    {
        this->setStyleSheet(current_color1);
        this->setWindowOpacity(0.8);
    }
}


bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{

    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        mouse_flag1 = 1;
        mouse_x1 = mouseEvent->x();
        mouse_y1 = mouseEvent->y();
        return true;
    } else if (event->type() == QEvent::MouseMove && mouse_flag1 == 1) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        int x = this->x() + mouseEvent->x();
        int y = this->y() + mouseEvent->y();
        setGeometry(x - mouse_x1, y - mouse_y1, 1010, 700);
        return true;
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::soundoff()
{
    if (!m_isPlaying)
    {
        m_clocksound = new QSoundEffect(this);
        m_clocksound->setSource(QUrl::fromLocalFile(":/new/prefix1/1/clockm.wav"));
        m_clocksound->setLoopCount(QSoundEffect::Infinite);
        m_clocksound->setVolume(1);
        m_clocksound->play();
        m_isPlaying = true;
    }
    else
    {
        m_clocksound->stop();
        delete m_clocksound;
        m_isPlaying = false;
    }
}
