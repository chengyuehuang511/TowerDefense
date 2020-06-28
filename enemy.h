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
    Enemy(WayPoint *startWayPoint, MainWindow *game,const QPixmap &pic=QPixmap(":/image/enemy1.png"));
    virtual void draw(QPainter *painter) const;//虚函数重载
    void move();
    QPoint pos(){return m_pos;}
    void getRemoved();
    virtual void getDamage(int damage);
    void getAttacked(Tower *attacker);
    void gotLostSight(Tower *attacker);
    void slowDown();
    void slowDown2();
    bool canslow()
    {
        if(m_walkingSpeed==2.0)
            return false;
        return true;
    }
    void Ice(){m_isIced=true;}
    void isChecked(){m_check=true;}
    bool check(){return m_check;}
public slots:
    void doActivate(){m_active=true;}

protected:
    int m_maxHP;//最大血条
    int m_currentHP;//当前血条
    double m_walkingSpeed;//行走速度
    bool m_active;//能否移动
    bool m_check=false;
    QPoint m_pos;
    QPixmap m_pic;
    WayPoint *m_destinationWayPoint;
    MainWindow *m_game;//最后敌人进入基地或被打死时调用移除函数
    double m_rotationPic;//旋转图片
    QList<Tower *>	m_attackedTowersList;
    bool m_isIced=false;
};
class Enemy2:public Enemy
{
    Q_OBJECT
public:
    Enemy2(WayPoint *startWayPoint, MainWindow *game, const QPixmap &pic=QPixmap("://image/enemy2.png"));
    void getDamage(int damage);
    void draw(QPainter *painter) const;
};
class Enemy3:public Enemy
{
    Q_OBJECT
public:
    Enemy3(WayPoint *startWayPoint, MainWindow *game, const QPixmap &pic=QPixmap("://image/enemy3.png"));
    void getDamage(int damage);
    void draw(QPainter *painter) const;
private:
    bool m_speedup;
};
class Enemy4:public Enemy
{
    Q_OBJECT
public:
    Enemy4(WayPoint *startWayPoint, MainWindow *game, const QPixmap &pic=QPixmap("://image/enemy4.png"));
    void getDamage(int damage);
    void draw(QPainter *painter) const;
};

#endif // ENEMY_H
