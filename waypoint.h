#ifndef WAYPOINT_H
#define WAYPOINT_H
#include <QImage>
#include <QPainter>
#include <QPoint>
#include <QMainWindow>
#include "mainwindow.h"

class WayPoint
{
public:
    WayPoint(QPoint pos);
    void setNextWayPoint(WayPoint *nextPoint);
    WayPoint* nextWayPoint() const;
    const QPoint pos() const;
    void draw(QPainter *painter) const;
private:
    QPoint m_pos;
    WayPoint * m_nextWayPoint;
};

#endif // WAYPOINT_H
