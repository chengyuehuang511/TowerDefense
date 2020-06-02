#include "tower.h"
#include "mainwindow.h"
#include <QMainWindow>
#include "towerposition.h"
#include<QPainter>
#include <QPixmap>
#include <QPoint>
#include <QTimer>

Tower::Tower(QPoint tow,MainWindow *game,const QPixmap &towpic):
    m_tow(tow),m_game(game),m_towpic(towpic),m_attackRange(200),
    m_damage(10),m_fireRate(500), m_rotationPic(0.0),m_chooseEnemy(NULL)
{
    m_fireRateTimer=new QTimer(this);
    connect(m_fireRateTimer,SIGNAL(timeout()),this,SLOT(shootWeapon()));
}

void Tower::draw(QPainter *painter) const
{
    painter->drawPixmap(m_tow.x(),m_tow.y(),m_towpic);
    painter->save();
    painter->setPen(Qt::white);
    QPoint offsetPoint(m_towpic.width()/2,m_towpic.height()/2);
    painter->drawEllipse(m_tow+offsetPoint,m_attackRange,m_attackRange);
    painter->translate(m_tow);
    painter->restore();
}
void Tower::attackEnemy()
{
    m_fireRateTimer->start(m_fireRate);
}
void Tower::chooseEnemyForAttack(Enemy *enemy)
{
    m_chooseEnemy=enemy;
    attackEnemy();
    m_chooseEnemy->getAttacked(this);
}
void Tower::shootWeapon()
{
    Bullet *bullet=new Bullet(m_tow,m_chooseEnemy->pos(),m_damage,m_chooseEnemy,m_game);
    bullet->move();
    m_game->addBullet(bullet);
}
void Tower::targetKilled()
{
    if(m_chooseEnemy)
        m_chooseEnemy=NULL;
    m_fireRateTimer->stop();
    m_rotationPic=0.0;
}
void Tower::lostSightofEnemy()
{
    m_chooseEnemy->gotLostSight(this);
    if(m_chooseEnemy)
        m_chooseEnemy=NULL;
    m_fireRateTimer->stop();
    m_rotationPic=0.0;
}
void Tower::checkEnemyInRange()
{
    if (m_chooseEnemy)
    {
        // 这种情况下,需要旋转炮台对准敌人
        // 向量标准化
        QVector2D normalized(m_chooseEnemy->pos()-m_tow);
        normalized.normalize();
        m_rotationPic=qRadiansToDegrees(qAtan2(normalized.y(),normalized.x()))-90;

        // 如果敌人脱离攻击范围
        if (!collisionWithCircle(m_tow,m_attackRange,m_chooseEnemy->pos(),1))
            lostSightofEnemy();
    }
    else
    {
        // 遍历敌人,看是否有敌人在攻击范围内
        QList<Enemy *> enemyList=m_game->enemyList();
        foreach (Enemy *enemy, enemyList)
        {
            if (collisionWithCircle(m_tow,m_attackRange,enemy->pos(),1))
            {
                chooseEnemyForAttack(enemy);
                break;
            }
        }
    }
}
