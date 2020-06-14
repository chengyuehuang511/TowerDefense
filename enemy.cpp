#include "enemy.h"
#include "utility.h"
#include "mainwindow.h"
#include <QMainWindow>
#include <QtMath>
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QMatrix>
#include <QVector2D>
class MainWindow;

Enemy::Enemy(WayPoint *startWayPoint, MainWindow *game, const QPixmap &pic)
    :QObject(0),
      m_pos(startWayPoint->pos()),
      m_pic(pic)
{
    m_maxHP=40;
    m_currentHP=40;
    m_active=false;
    m_walkingSpeed=5.0;
    m_destinationWayPoint=startWayPoint->nextWayPoint();
    m_rotationPic=0.0;
    m_game=game;
}
Enemy2::Enemy2(WayPoint *startWayPoint, MainWindow *game, const QPixmap &pic)
    :Enemy(startWayPoint,game,pic)
{
    m_walkingSpeed=6.0;
    m_maxHP=50;
    m_currentHP=m_maxHP;
    m_active=false;
    m_destinationWayPoint=startWayPoint->nextWayPoint();
    m_rotationPic=180;
    m_game=game;
}
Enemy3::Enemy3(WayPoint *startWayPoint, MainWindow *game, const QPixmap &pic)
    :Enemy(startWayPoint,game,pic)
{
    m_walkingSpeed=6.0;
    m_maxHP=60;
    m_speedup=false;
    m_currentHP=m_maxHP;
    m_active=false;
    m_destinationWayPoint=startWayPoint->nextWayPoint();
    m_rotationPic=0.0;
    m_game=game;
}
void Enemy3::getDamage(int damage)
{
    Enemy::getDamage(damage);
    if(m_speedup==false)
    {
        m_speedup=true;
        m_walkingSpeed=1.5*m_walkingSpeed;
    }
}
void Enemy::draw(QPainter *painter) const
{
    if(!m_active)
        return;
    static const int Health_Bar_Width=50;
    painter->save();
    QPoint healthBarPoint=m_pos+QPoint(-Health_Bar_Width/2-35,-m_pic.height()/3);
    //绘制血条
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    QRect healthBarBackRect(healthBarPoint,QSize(Health_Bar_Width, 2));
    painter->drawRect(healthBarBackRect);
    painter->setBrush(Qt::green);
    QRect healthBarRect(healthBarPoint, QSize((double)m_currentHP/m_maxHP*Health_Bar_Width,2));
    painter->drawRect(healthBarRect);
    //绘制偏转坐标，由中心+偏移=左上
    static const QPoint offsetPoint(-m_pic.width()/2,-m_pic.height()/2);
    painter->translate(m_pos);
    painter->rotate(m_rotationPic);
    //绘制敌人
    painter->drawPixmap(offsetPoint,m_pic);
    painter->restore();
}
void Enemy::move()
{
    if(!m_active||m_game->m_gameEnded)
        return;
    if(CollisionWithCircle(m_pos,1,m_destinationWayPoint->pos(),1))
    {
        //敌人抵达了一个航点
        if(m_destinationWayPoint->nextWayPoint())
        {
            //还有下一个航点
            m_pos=m_destinationWayPoint->pos();
            m_destinationWayPoint=m_destinationWayPoint->nextWayPoint();
        }
        else
        {
            m_game->getHpDamage();
            m_game->removedEnemy(this);
            return;
        }
    }
    QPoint targetPoint=m_destinationWayPoint->pos();
    double movementSpeed=m_walkingSpeed;
    QVector2D normalized(targetPoint-m_pos);
    normalized.normalize();
    m_pos=m_pos+normalized.toPoint()*movementSpeed;
    m_rotationPic=qRadiansToDegrees(qAtan2(normalized.y(),normalized.x()));
}
void Enemy::getRemoved()
{
    if(m_attackedTowersList.empty())
        return;
    foreach(Tower *attacker,m_attackedTowersList)
        attacker->targetKilled();
    m_game->removedEnemy(this);
}
void Enemy::getDamage(int damage)
{
    m_game->audioPlayer()->playSound(LaserShootSound);
    m_currentHP-=damage;
    if(m_currentHP<=0)
    {
        m_game->audioPlayer()->playSound(EnemyDestroySound);
        m_game->awardGold(200);
        getRemoved();
    }
}
void Enemy::slowDown()
{
    m_game->audioPlayer()->playSound(LaserShootSound);
    if(m_walkingSpeed>=2.0)
        m_walkingSpeed=2.0;
}
void Enemy::getAttacked(Tower *attacker)
{
    m_attackedTowersList.push_back(attacker);
}
void Enemy::gotLostSight(Tower *attacker)
{
    m_attackedTowersList.removeOne(attacker);
}
