#include "menu.h"
#include "ui_menu.h"
#include "mybutton.h"
#include <QPushButton>
#include "mainwindow.h"

Menu::Menu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);
    this->setFixedSize(1500,1000);
    MyButton *theme1_btn=new MyButton("://image/play.png");
    MyButton *theme2_btn=new MyButton("://image/play.png");
    theme1_btn->setParent(this);
    theme2_btn->setParent(this);
    theme1_btn->move(310,750);
    theme2_btn->move(960,750);
    connect(theme1_btn,&MyButton::clicked,this,[=]()
    {
        MainWindow *mainwindow1=new MainWindow(1);//解决了主页和游戏两个页面同时进行的问题，只有点击了才能开始游戏
        this->close();
        mainwindow1->show();
        connect(mainwindow1,&MainWindow::chooseback,this,[=]()
        {
            mainwindow1->deleteLater();
            this->show();
        });
    });
    connect(theme2_btn,&MyButton::clicked,this,[=]()
    {
        MainWindow *mainwindow2=new MainWindow(2);
        this->close();
        mainwindow2->show();
        connect(mainwindow2,&MainWindow::chooseback,this,[=]()
        {
            mainwindow2->deleteLater();
            this->show();
        });
    });
}
void Menu::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,QPixmap(":/image/main.png"));
    painter.drawPixmap(130,250,QPixmap(":/image/theme1.png"));
    painter.drawPixmap(780,250,QPixmap(":/image/theme2.png"));
    painter.setPen(QPen(Qt::black));
    painter.setFont(QFont("Aria", 40, QFont::Black));
    painter.drawText(QRect(250,130,1000,1000),Qt::AlignHCenter,"X Moon's TowerDefence");
}

Menu::~Menu()
{
    delete ui;
}
