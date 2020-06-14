#ifndef ENEMY_H
#define ENEMY_H
#include <QImage>
#include <QPainter>
#include <QMainWindow>
#include "mainwindow.h"
#include "waypoint.h"
#include "tower.h"

class MainWindow;
class WayPoint;
class Tower;

class Enemy:public QObject
{
    Q_OBJECT
public:
    Enemy(WayPoint *startWayPoint, MainWindow *game,const QPixmap &pic);
    void draw(QPainter *painter) const;
    void move();
    QPoint pos(){return m_pos;}
    void getRemoved();
    void getDamage(int damage);
    void getAttacked(Tower *attacker);
    void gotLostSight(Tower *attacker);
    void slowDown();
    bool canslow()
    {
        if(m_walkingSpeed==2.0)
            return false;
        return true;
    }
public slots:
    void doActivate(){m_active=true;}

protected:
    int m_maxHP;//最大血条
    int m_currentHP;//当前血条
    double m_walkingSpeed;//行走速度
    bool m_active;//能否移动
    QPoint m_pos;
    QPixmap m_pic;
    WayPoint *m_destinationWayPoint;
    MainWindow *m_game;//最后敌人进入基地或被打死时调用移除函数
    double m_rotationPic;//旋转图片
    QList<Tower *>	m_attackedTowersList;

};
class Enemy2:public Enemy
{
    Q_OBJECT
public:
    Enemy2(WayPoint *startWayPoint, MainWindow *game, const QPixmap &pic=QPixmap("://image/enemy2.png"));
    void getDamage(int damage);
};
class Enemy3:public Enemy
{
    Q_OBJECT
public:
    Enemy3(WayPoint *startWayPoint, MainWindow *game, const QPixmap &pic=QPixmap("://image/enemy3.png"));
    void getDamage(int damage);
private:
    bool m_speedup;
};

#endif // ENEMY_H
