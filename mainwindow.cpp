#include "mainwindow.h"
#include "menu.h"
#include "QPainter"
#include <QMouseEvent>
#include <QObject>
#include <QTimer>
#include <QtGlobal>
#include <QMessageBox>
#include <QtDebug>
#include <QString>
#include <QFont>
#include "mybutton.h"
#include "menu.h"
using namespace std;
class TowerPosition;
class Tower;
class Enemy;
class Font;

MainWindow::MainWindow(int _scene,QWidget *parent) :
    QMainWindow(parent)
{
    m_towers.clear();
    m_enemyList.clear();
    m_bulletList.clear();
    m_towerpositions.clear();
    m_waypoints.clear();
    this->setFixedSize(1500,1000);
    scene=_scene;
    m_audioPlayer = new AudioPlayer(this);
    m_audioPlayer->startBGM();
    QMediaPlayer * player = new QMediaPlayer;
    player->setVolume(30);
    player->play();
    //ui->setupUi(this);
    if(scene==1)
    {
        loadTowerPositions1();
        addWayPoints1();
    }
    else
    {
        loadTowerPositions2();
        addWayPoints2();
    }
    m_waves=0;
    m_playerHP=5;
    m_playerGold=1000;

    if(scene==1)
        loadWave1();
    else
        loadWave2();
    m_gameWin=false;
    m_gameEnded=false;
    m_gamePaused=false;
    TowerMode=1;

    MyButton *towbtn1=new MyButton(":/image/tower1_button.png");
    towbtn1->setParent(this);
    towbtn1->move(40,850);
    connect(towbtn1,&MyButton::clicked,this,&MainWindow::changetowermode1);
    MyButton *towbtn2=new MyButton(":/image/tower2_button.png");
    towbtn2->setParent(this);
    towbtn2->move(40+87,850);
    connect(towbtn2,&MyButton::clicked,this,&MainWindow::changetowermode2);
    MyButton *towbtn3=new MyButton(":/image/tower3_button.png");
    towbtn3->setParent(this);
    towbtn3->move(40+87*2,850);
    connect(towbtn3,&MyButton::clicked,this,&MainWindow::changetowermode3);

    QTimer *timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateMap()));
    timer->start(30);

    MyButton *back_btn=new MyButton(":/image/menu.png");
    back_btn->setParent(this);
    back_btn->move(1250,900);
    connect(back_btn,&MyButton::clicked,this,[=](){
        m_audioPlayer->playSound(PushButtonSound);
        emit chooseback();
    });
    MyButton *continue_btn=new MyButton(":/image/continue.png");
    continue_btn->setParent(this);
    continue_btn->move(1250,830);
    connect(continue_btn,&MyButton::clicked,this,[=](){
        m_audioPlayer->playSound(PushButtonSound);
        this->m_gamePaused=false;
    });
    MyButton *pause_btn=new MyButton(":/image/pause.png");
    pause_btn->setParent(this);
    pause_btn->move(1250,760);
    connect(pause_btn,&MyButton::clicked,this,[=](){
        m_audioPlayer->playSound(PushButtonSound);
        this->m_gamePaused=true;
    });
}
void MainWindow::updateMap()
{
    if(m_gamePaused||m_gameEnded)
        return;
    foreach(Enemy *enemy,m_enemyList)
        enemy->move();
    foreach (Tower *tower,m_towers)
        tower->checkEnemyInRange();
    update();
}

void MainWindow::loadTowerPositions1()
{
    QPoint pos[] =
    {
        QPoint(50,650),
        QPoint(200,650),
        QPoint(135,250),
        QPoint(135,415),
        QPoint(425,500),
        QPoint(425,335),
        QPoint(560,335),
        QPoint(560,500),
        QPoint(560,720),
        QPoint(675,85),
        QPoint(850,200),
        QPoint(850,400),
        QPoint(850,600),
        QPoint(885,840),
        QPoint(1075,840),
        QPoint(990,435),
        QPoint(990,600),
        QPoint(1265,700),
    };
    int len	= sizeof(pos) / sizeof(pos[0]);
    for (int i = 0; i < len; ++i)
        m_towerpositions.push_back(pos[i]);
}
void MainWindow::loadTowerPositions2()
{
    QPoint pos[] =
    {
        QPoint(200,100),
        QPoint(400,100),
        QPoint(600,100),
        QPoint(800,100),
        QPoint(300,320),
        QPoint(500,320),
        QPoint(700,320),
        QPoint(500,550),
        QPoint(500,720),
        QPoint(700,720),
        QPoint(400,930),
        QPoint(900,720),
        QPoint(600,930),
        QPoint(800,930),
        QPoint(1000,930),
        QPoint(1000,320),
        QPoint(1000,470),
        QPoint(1000,620),
        QPoint(200,550),
        QPoint(200,720),
    };
    int len	= sizeof(pos) / sizeof(pos[0]);
    for (int i = 0; i < len; ++i)
        m_towerpositions.push_back(pos[i]);
}

void MainWindow::addWayPoints1()
{
    QPoint p[9]=
    {
        QPoint(1340,280),
        QPoint(1340,450),
        QPoint(1180,460),
        QPoint(1180,770),
        QPoint(775,770),
        QPoint(765,230),
        QPoint(330,240),
        QPoint(320,565),
        QPoint(10,565),
    };
    WayPoint *wayPoint[9];
    for(int i=0;i<9;i++)
    {
        wayPoint[i]=new WayPoint(p[i]);
        m_waypoints.push_back(wayPoint[i]);
    }
    for(int i=0;i<8;i++)
        wayPoint[i+1]->setNextWayPoint(wayPoint[i]);
}
void MainWindow::addWayPoints2()
{
    QPoint p[12]=
    {
        QPoint(1280,360),
        QPoint(1180,360),
        QPoint(1180,860),
        QPoint(420,860),
        QPoint(420,480),
        QPoint(750,480),
        QPoint(750,670),
        QPoint(950,670),
        QPoint(950,260),
        QPoint(200,260),
        QPoint(200,450),
        QPoint(10,450),
    };
    WayPoint *wayPoint[12];
    for(int i=0;i<12;i++)
    {
        wayPoint[i]=new WayPoint(p[i]);
        m_waypoints.push_back(wayPoint[i]);
    }
    for(int i=0;i<11;i++)
        wayPoint[i+1]->setNextWayPoint(wayPoint[i]);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(TowerMode==1)
    {
        TowerCost=100;
        TowerUpCost=50;
        TowerDeleteCompensation=50;
    }
    else if(TowerMode==2)
    {
        TowerCost=150;
        TowerUpCost=75;
        TowerDeleteCompensation=75;
    }
    else
    {
        TowerCost=200;
        TowerUpCost=100;
        TowerDeleteCompensation=100;
    }
    QPoint pressPos = event->pos();
    auto it=m_towerpositions.begin();
    while(it!=m_towerpositions.end())
    {
        if(canBuyTower()&&it->containPoint(pressPos)&&!it->hasTower()
                &&TowerMode==1)//点击的点在坑内且没有塔
        {
            m_audioPlayer->playSound(TowerPlaceSound);
            m_playerGold-=TowerCost;
            it->setHasTower();
            Tower *tower = new Tower(it->centerPos(), this);
            m_towers.push_back(tower);
            update();
            break;
        }
        if(canBuyTower()&&it->containPoint(pressPos)&&!it->hasTower()
                &&TowerMode==2)//点击的点在坑内且没有塔
        {
            m_audioPlayer->playSound(TowerPlaceSound);
            m_playerGold-=TowerCost;
            it->setHasTower();
            Tower *tower = new Tower2(it->centerPos(), this);
            m_towers.push_back(tower);
            update();
            break;
        }
        if(canBuyTower()&&it->containPoint(pressPos)&&!it->hasTower()
                &&TowerMode==3)//点击的点在坑内且没有塔
        {
            m_audioPlayer->playSound(TowerPlaceSound);
            m_playerGold-=TowerCost;
            it->setHasTower();
            Tower *tower = new Tower3(it->centerPos(), this);
            m_towers.push_back(tower);
            update();
            break;
        }
        foreach(Tower *attacker,m_towers)
        {
            if(m_playerGold>=TowerUpCost&&attacker->towerlevel()<2
                    &&attacker->containUpPoint(pressPos)&&it->hasTower())
            {
                m_audioPlayer->playSound(TowerUpgradeSound);
                m_playerGold-=TowerUpCost;
                attacker->setTowerLevel(2);
                update();
                break;
            }
            if(attacker->containDeletePoint(pressPos)&&it->centerPos()==attacker->towerpos())
            {
                m_audioPlayer->playSound(TowerDeleteSound);
                m_playerGold+=TowerDeleteCompensation;
                attacker->disconnectEnemyForAttack();
                removedTower(attacker);
                it->deleteTower();
                update();
                break;
            }
        }
        ++it;
    }

}

void MainWindow::drawWave(QPainter *painter)
{
    painter->setPen(QPen(Qt::black));
    painter->setFont(QFont("Aria", 25, QFont::Black));
    painter->drawPixmap(1150,38,200,70,QPixmap(":/image/wave.png"));
    painter->drawText(QRect(1280,44,500,70),QString("%1").arg(m_waves+1));
}
void MainWindow::drawHP(QPainter *painter)
{
    painter->setPen(QPen(Qt::black));
    painter->drawPixmap(150,38,200,70,QPixmap(":/image/hp.png"));
    painter->drawText(QRect(280,44,500,70),QString("%1").arg(m_playerHP));
}
void MainWindow::drawPlayerGold(QPainter *painter)
{
    painter->setPen(QPen(Qt::black));
    painter->drawPixmap(600,38,210,70,QPixmap(":/image/gold.png"));
    painter->drawText(QRect(670,42,500,70),QString("%1").arg(m_playerGold));
}



void MainWindow::paintEvent(QPaintEvent *)
{
//    QPainter painter(this);
//    painter.drawPixmap(0, 0, QPixmap(":/image/map1.jpg"));
//    foreach (const TowerPosition &towerPos, m_towerpositions)
//        towerPos.draw(& painter);
//    foreach(const Tower *tower,m_towers)
//        tower->draw(& painter);
//    foreach(const WayPoint *waypoint,m_waypoints)
//        waypoint->draw(&painter);
//    foreach(const Enemy *enemy,m_enemyList)
//        enemy->draw(&painter);
    QPixmap cachePix;
    if(scene==1)
        cachePix=QPixmap(":/image/map1.jpg");
    else
        cachePix=QPixmap(":/image/map2.jpg");
    QPainter cachePainter(&cachePix);
    drawWave(&cachePainter);
    drawHP(&cachePainter);
    drawPlayerGold(&cachePainter);
    foreach (const TowerPosition &towerPos, m_towerpositions)
        towerPos.draw(&cachePainter);
    foreach(const Tower *tower,m_towers)
        tower->draw(&cachePainter);
//        foreach(const WayPoint *waypoint,m_waypoints)
//            waypoint->draw(&cachePainter);
    foreach(const Enemy *enemy,m_enemyList)
        enemy->draw(&cachePainter);
    foreach (const Bullet *bullet, m_bulletList)
        bullet->draw(&cachePainter);
    QPainter painter(this);
    painter.drawPixmap(0,0,cachePix);
    if(m_gameWin)
    {
        QPainter painter(this);
        painter.drawPixmap(450,150,QPixmap(":/image/victory1.png"));
        painter.drawPixmap(610,280,QPixmap(":/image/victory2.png"));
    }
    else if(m_gameEnded)
    {
        QPainter painter(this);
        painter.drawPixmap(450,50,QPixmap(":/image/fail.png"));
    }
}
bool MainWindow::loadWave1()
{
    if(m_waves>=6)
        return false;
    WayPoint *startWayPoint=m_waypoints.back();

//    const QPixmap &pic1=QPixmap(":/image/enemy1.png");
//    const QPixmap &pic2=QPixmap(":/image/enemy2.png");
//    const QPixmap &pic3=QPixmap(":/image/enemy3.png");
    if(m_waves==0||m_waves==1)
    {
        int enemyStartInterval[]={1000,2500,4500,7000,10000,13500};
        for(int i=0;i<6;++i)
        {
            Enemy *enemy=new Enemy(startWayPoint,this);
            m_enemyList.push_back(enemy);
            QTimer::singleShot(enemyStartInterval[i],enemy,SLOT(doActivate()));
        }
    }
    else if(m_waves==2||m_waves==3)
    {
        int enemyStartInterval[]={1000,2500,4000,5500,7000,8500,10000,11500,13000,14500};
        for(int i=0;i<10;++i)
        {
            Enemy *enemy=new Enemy2(startWayPoint,this);
            m_enemyList.push_back(enemy);
            QTimer::singleShot(enemyStartInterval[i],enemy,SLOT(doActivate()));
        }
    }
    else
    {
        int enemyStartInterval[]={1000,2000,3000,4000,5000,6000,7000,8000,9000,10000,11000,12000,13000,14000};
        for(int i=0;i<14;++i)
        {
            Enemy *enemy=new Enemy3(startWayPoint,this);
            m_enemyList.push_back(enemy);
            QTimer::singleShot(enemyStartInterval[i],enemy,SLOT(doActivate()));
        }
    }
    return true;
}
bool MainWindow::loadWave2()
{
    if(m_waves>=8)
        return false;
    WayPoint *startWayPoint=m_waypoints.back();

//    const QPixmap &pic1=QPixmap(":/image/enemy1.png");
//    const QPixmap &pic2=QPixmap(":/image/enemy2.png");
//    const QPixmap &pic3=QPixmap(":/image/enemy3.png");
    if(m_waves==0||m_waves==1)
    {
        int enemyStartInterval[]={1000,2000,3000,4000,5000,6000,7000,8000};
        for(int i=0;i<8;++i)
        {
            Enemy *enemy=new Enemy(startWayPoint,this);
            m_enemyList.push_back(enemy);
            QTimer::singleShot(enemyStartInterval[i],enemy,SLOT(doActivate()));
        }
    }
    else if(m_waves==2||m_waves==3)
    {
        int enemyStartInterval[]={1000,2500,4000,5500,7000,8500,10000,11500,13000,14500,16000,17500};
        for(int i=0;i<12;++i)
        {
            Enemy *enemy=new Enemy2(startWayPoint,this);
            m_enemyList.push_back(enemy);
            QTimer::singleShot(enemyStartInterval[i],enemy,SLOT(doActivate()));
        }
    }
    else if(m_waves==4||m_waves==5)
    {
        int enemyStartInterval[]={1000,2000,3000,4000,5000,6000,7000,8000,9000,10000,11000,12000,13000,14000};
        for(int i=0;i<14;++i)
        {
            Enemy *enemy=new Enemy3(startWayPoint,this);
            m_enemyList.push_back(enemy);
            QTimer::singleShot(enemyStartInterval[i],enemy,SLOT(doActivate()));
        }
    }
    else
    {
        int enemyStartInterval[]={1000,2000,3000,4000,5000,6000,7000,8000,9000,10000,11000,12000,13000,14000,15000,16000};
        for(int i=0;i<16;++i)
        {
            Enemy *enemy=new Enemy4(startWayPoint,this);
            m_enemyList.push_back(enemy);
            QTimer::singleShot(enemyStartInterval[i],enemy,SLOT(doActivate()));
        }
    }
    return true;
}
void MainWindow::getHpDamage(int damage)
{
    m_audioPlayer->playSound(LifeLoseSound);
    m_playerHP-=damage;
    if(m_playerHP<=0)
        doGameOver();
}
void MainWindow::removedEnemy(Enemy *enemy)
{
    Q_ASSERT(enemy);
    m_enemyList.removeOne(enemy);
    delete enemy;
    if(m_enemyList.empty())
    {
        ++m_waves;
        if(scene==1)
        {
            if(!loadWave1())
            {
                m_gameWin=true;
            }
        }
        else
        {
            if(!loadWave2())
            {
                m_gameWin=true;
            }
        }
    }
}
void MainWindow::removedBullet(Bullet *bullet)
{
    Q_ASSERT(bullet);

    m_bulletList.removeOne(bullet);
    delete bullet;
}
//void MainWindow::stopBullet(Bullet *bullet)
//{
//    Q_ASSERT(bullet);

//    m_bulletList.removeOne(bullet);
//    bullet->stop();
//}
void MainWindow::removedTower(Tower *tower)
{
    Q_ASSERT(tower);
    m_towers.removeOne(tower);
    delete tower;
}
void MainWindow::addBullet(Bullet *bullet)
{
    Q_ASSERT(bullet);

    m_bulletList.push_back(bullet);
}
QList<Enemy *> MainWindow::enemyList() const//调取怪物表
{
    return m_enemyList;
}
void MainWindow::doGameOver()
{
    if(!m_gameEnded)
    {
        m_gameEnded=true;
    }
}



