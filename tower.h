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
public slots:
    void shootWeapon();
private:
    QPoint m_tow;
    QPixmap m_towpic;
    MainWindow *m_game;
    int m_attackRange;
    int m_damage;
    int m_fireRate;
    double m_rotationPic;
    Enemy *m_chooseEnemy;
    QTimer *m_fireRateTimer;
};

#endif // TOWER_H
