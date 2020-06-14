#include "towerposition.h"
#include "mainwindow.h"

TowerPosition::TowerPosition(QPoint pos,const QPixmap &pospic):
    m_pos(pos),m_pospic(pospic),m_hastower(false)
{
}

void TowerPosition::draw(QPainter *painter) const
{
    painter->drawPixmap(m_pos.x(),m_pos.y(),m_pospic);
}

const QPoint TowerPosition::centerPos() const
{
    QPoint offsetPoint(m_pospic.width()/8,m_pospic.height()/(-1.5));
    return m_pos+offsetPoint;
}

bool TowerPosition::containPoint(const QPoint &pos) const
{
    bool isXInHere=m_pos.x()<pos.x()&&pos.x()<(m_pos.x()+m_pospic.width());
    bool isYInHere=m_pos.y()<pos.y()&&pos.y()<(m_pos.y()+m_pospic.height());
    return isXInHere&&isYInHere;
}
bool TowerPosition::hasTower()const
{
    return m_hastower;
}
void TowerPosition::setHasTower(bool hastower)
{
    m_hastower=hastower;
}
void TowerPosition::deleteTower(bool hastower)
{
    m_hastower=hastower;
}
