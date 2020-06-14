#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "towerposition.h"
#include "tower.h"
#include "waypoint.h"
#include "enemy.h"
#include "utility.h"
#include <QList>
#include "QMouseEvent"
#include "QPainter"
#include <QPoint>
#include <QPixmap>
#include "QTimer"
#include "bullet.h"
#include <QFont>
#include "audioplayer.h"
#include <QContextMenuEvent>
//#include "iostream"

class Tower;
class TowerPosition;
class WayPoint;
class Enemy;
class Bullet;
class Font;
class AudioPlayer;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void getHpDamage(int damage=1);
    void removedEnemy(Enemy *enemy);
    void removedBullet(Bullet *bullet);
    void removedTower(Tower *tower);
    void addBullet(Bullet *bullet);
    QList<Enemy *> enemyList() const;
    void doGameOver();

    bool canBuyTower()const
    {
        if(m_playerGold>=TowerCost)
            return true;
        return false;
    }
    void awardGold(int points)
    {
        m_playerGold+=points;
        update();
    }
    void drawWave(QPainter *painter);
    void drawHP(QPainter *painter);
    void drawPlayerGold(QPainter *painter);
    bool m_gameEnded;
    AudioPlayer* audioPlayer() const{return m_audioPlayer;}
public slots:
    void updateMap();
    void changetowermode1(){m_audioPlayer->playSound(PushButtonSound);TowerMode=1;}
    void changetowermode2(){m_audioPlayer->playSound(PushButtonSound);TowerMode=2;}
    void changetowermode3(){m_audioPlayer->playSound(PushButtonSound);TowerMode=3;}
private:
    Ui::MainWindow *ui;
    QList<TowerPosition> m_towerpositions;
    QList<Tower *> m_towers;
    QList<WayPoint *> m_waypoints;
    QList<Enemy *> m_enemyList;
    QList<Bullet *> m_bulletList;
    void loadTowerPositions1();
    void addWayPoints1();
    bool loadWave();
    int m_waves;
    int m_playerHP;
    int m_playerGold;
    int TowerCost;
    int TowerUpCost;
    bool m_gameWin;
    AudioPlayer* m_audioPlayer;
    int TowerMode;
};

#endif // MAINWINDOW_H
