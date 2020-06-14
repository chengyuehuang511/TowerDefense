#ifndef TOWER_H
#define TOWER_H
#include <QImage>
#include <QPainter>
#include <QMainWindow>
#include <QTimer>
#include "mainwindow.h"
#include "towerposition.h"
#include "bullet.h"
#include "enemy.h"
#include <QObject>
class MainWindow;
class Bullet;
class Enemy;

class Tower:public QObject
{
    Q_OBJECT
public:
    Tower(QPoint tow, MainWindow *game,const QPixmap &towpic=QPixmap(":/image/tower1.png"));
    void draw(QPainter *painter) const;
    void targetKilled();
    void attackEnemy();
    void chooseEnemyForAttack(Enemy *enemy);
    void removeBullet();
    void lostSightofEnemy();
    void checkEnemyInRange();
    void setTowerLevel(int level);
    bool containUpPoint(const QPoint &pos)const;
    bool containDeletePoint(const QPoint &pos)const;
    QPoint towerpos(){return m_tow;}
    int towerlevel(){return m_towerLevel;}
public slots:
    void shootWeapon();
protected:
    int m_towerType=1;
    int m_towerLevel;
    QPoint m_tow;
    QPixmap m_towpic;
    QPixmap m_towup;
    QPixmap m_towdelete;
    MainWindow *m_game;
    int m_attackRange;
    int m_damage;
    int m_fireRate;
    double m_rotationPic;
    Enemy *m_chooseEnemy;
    QTimer *m_fireRateTimer;
};

class Tower2:public Tower
{
    Q_OBJECT
public:
    Tower2(QPoint pos,MainWindow *game,const QPixmap &towpic2=QPixmap(":/image/tower2.png"));
    void chooseEnemyForAttack(Enemy *enemy);
    void setTowerLevel(int level);
public slots:
    void shootWeapon();
private:
    int m_towerType=2;
};

class Tower3:public Tower
{
    Q_OBJECT
public:
    Tower3(QPoint pos,MainWindow *game,const QPixmap &towpic3=QPixmap(":/image/tower3.png"));
    void chooseEnemyForAttack(Enemy *enemy);
    void setTowerLevel(int level);
public slots:
    void shootWeapon();
private:
    int m_towerType=3;
};

#endif // TOWER_H
