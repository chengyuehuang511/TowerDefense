#include "mainwindow.h"
#include "ui_mainwindow.h"
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
using namespace std;
class TowerPosition;
class Tower;
class Enemy;
class Font;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_audioPlayer = new AudioPlayer(this);
    m_audioPlayer->startBGM();
    QMediaPlayer * player = new QMediaPlayer;
    player->setVolume(30);
    player->play();
    ui->setupUi(this);
    loadTowerPositions1();
    addWayPoints1();
    m_waves=0;
    m_playerHP=5;
    m_playerGold=1000;
    TowerCost=300;
    TowerUpCost=120;
    loadWave();
    m_gameWin=false;
    m_gameEnded=false;
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
}
void MainWindow::updateMap()
{
    foreach(Enemy *enemy,m_enemyList)
        enemy->move();
    foreach (Tower *tower,m_towers)
        tower->checkEnemyInRange();
    update();
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint pressPos = event->pos();
    auto it=m_towerpositions.begin();
    while(it!=m_towerpositions.end())
    {
        if(canBuyTower()&&it->containPoint(pressPos)&&!it->hasTower()&&TowerMode==1)//点击的点在坑内且没有塔
        {
            m_audioPlayer->playSound(TowerPlaceSound);
            m_playerGold-=TowerCost;
            it->setHasTower();
            Tower *tower = new Tower(it->centerPos(), this);
            m_towers.push_back(tower);
            update();
            break;
        }
        if(canBuyTower()&&it->containPoint(pressPos)&&!it->hasTower()&&TowerMode==2)//点击的点在坑内且没有塔
        {
            m_audioPlayer->playSound(TowerPlaceSound);
            m_playerGold-=TowerCost;
            it->setHasTower();
            Tower *tower = new Tower2(it->centerPos(), this);
            m_towers.push_back(tower);
            update();
            break;
        }
        if(canBuyTower()&&it->containPoint(pressPos)&&!it->hasTower()&&TowerMode==3)//点击的点在坑内且没有塔
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
            if(m_playerGold>=TowerUpCost&&attacker->towerlevel()<2&&attacker->containUpPoint(pressPos)&&it->hasTower())
            {
                m_audioPlayer->playSound(TowerUpgradeSound);
                m_playerGold-=TowerUpCost;
                attacker->setTowerLevel(2);
                update();
                break;
            }
            if(attacker->containDeletePoint(pressPos)&&it->hasTower())
            {
                m_audioPlayer->playSound(TowerDeleteSound);
                m_playerGold+=200;
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
    painter->setPen(QPen(Qt::red));
    painter->setFont(QFont("Aria", 30, QFont::Black));
    painter->drawText(QRect(1200,38,500,70),QString("WAVE:%1").arg(m_waves+1));
}
void MainWindow::drawHP(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(200,38,500,70),QString("HP:%1").arg(m_playerHP));
}
void MainWindow::drawPlayerGold(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(650,38,500,70),QString("GOLD:%1").arg(m_playerGold));
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
    QPixmap cachePix(":/image/map1.jpg");
    QPainter cachePainter(&cachePix);
    drawWave(&cachePainter);
    drawHP(&cachePainter);
    drawPlayerGold(&cachePainter);
    foreach (const TowerPosition &towerPos, m_towerpositions)
        towerPos.draw(&cachePainter);
    MyButton *btn=new MyButton(":/image/tower_up.png");
    btn->setParent(this);
    btn->move(50,50);
    foreach(const Tower *tower,m_towers)
        tower->draw(&cachePainter);
    foreach(const WayPoint *waypoint,m_waypoints)
        waypoint->draw(&cachePainter);
    foreach(const Enemy *enemy,m_enemyList)
        enemy->draw(&cachePainter);
    foreach (const Bullet *bullet, m_bulletList)
        bullet->draw(&cachePainter);
    QPainter painter(this);
    painter.drawPixmap(0,0,cachePix);
    if(m_gameEnded||m_gameWin)
    {
        QString text=m_gameEnded ? "YOU LOST!!!":"YOU WIN!!!";
        QPainter painter(this);
        painter.setPen(QPen(Qt::red));
        painter.setFont(QFont("Aria", 30, QFont::Black));
        painter.drawText(rect(),Qt::AlignCenter,text);
        return;
    }
}
bool MainWindow::loadWave()
{
    if(m_waves>=6)
        return false;
    WayPoint *startWayPoint=m_waypoints.back();

    const QPixmap &pic1=QPixmap(":/image/enemy1.png");
    const QPixmap &pic2=QPixmap(":/image/enemy2.png");
    const QPixmap &pic3=QPixmap(":/image/enemy3.png");
    if(m_waves==0||m_waves==1)
    {
        int enemyStartInterval[]={1000,2500,4500,7000,10000,13500};
        for(int i=0;i<6;++i)
        {
            Enemy *enemy=new Enemy(startWayPoint,this,pic1);
            m_enemyList.push_back(enemy);
            QTimer::singleShot(enemyStartInterval[i],enemy,SLOT(doActivate()));
        }
    }
    else if(m_waves==2||m_waves==3)
    {
        int enemyStartInterval[]={1000,2500,4000,5500,7000,8500,10000,11500,13000,14500};
        for(int i=0;i<10;++i)
        {
            Enemy *enemy=new Enemy(startWayPoint,this,pic2);
            m_enemyList.push_back(enemy);
            QTimer::singleShot(enemyStartInterval[i],enemy,SLOT(doActivate()));
        }
    }
    else
    {
        int enemyStartInterval[]={1000,2000,3000,4000,5000,6000,7000,8000,9000,10000,11000,12000,13000,14000};
        for(int i=0;i<14;++i)
        {
            Enemy *enemy=new Enemy(startWayPoint,this,pic3);
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
        if(!loadWave())
        {
            m_gameWin=true;
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



