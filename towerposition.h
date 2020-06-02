#ifndef TOWERPOSITION_H
#define TOWERPOSITION_H
#include <QImage>
#include <QPainter>
#include <QMainWindow>

class TowerPosition
{
public:
    TowerPosition(QPoint pos,const QPixmap &pospic=QPixmap(":/image/tow_pos.png"));
    void draw(QPainter *painter) const;
    void setHasTower(bool hastower = true);
    bool hasTower() const;
    const QPoint centerPos() const;
    bool containPoint(const QPoint &pos) const;
private:
    QPoint m_pos;
    QPixmap m_pospic;
    bool m_hastower;
};

#endif // TOWERPOSITION_H
