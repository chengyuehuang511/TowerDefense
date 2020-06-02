#ifndef BULLET_H
#define BULLET_H
#include <QImage>
#include <QPainter>
#include <QMainWindow>
#include "mainwindow.h"
#include "enemy.h"
#include <QPropertyAnimation>
class Enemy;
class MainWindow;

class Bullet:public QObject
{
    Q_OBJECT//所有应用QT槽的类都需要声明
    Q_PROPERTY(QPoint m_currentPos READ currentPos WRITE setCurrentPos)
public:
    Bullet(QPoint startPos, QPoint targetPoint, int damage, Enemy *target,
           MainWindow *game, const QPixmap &pic = QPixmap(":/image/bullet1.png"));
    void draw(QPainter *painter) const;
    void move();
    void setCurrentPos(QPoint pos);
    QPoint currentPos() const;
public slots:
    void hitTarget();
private:
    const QPoint m_startPos;
    const QPoint m_targetPos;
    QPoint m_currentPos;
    QPixmap m_pic;
    Enemy *m_target;
    MainWindow *m_game;
    int m_damage;
};

#endif // BULLET_H
