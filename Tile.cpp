#include "Tile.h"


Tile::Tile(QGraphicsItem *parent):
    QGraphicsPixmapItem(parent), type('e'), x(0), y(0)
{
    massive = false;
    peg = false;
    bonus = false;
    layout();
}

Tile::Tile(char t, int x_coord, int y_coord, QGraphicsItem *parent):
    QGraphicsPixmapItem(parent), type(t), x(x_coord), y(y_coord)
{
    massive = false;
    peg = false;
    bonus = false;
    layout();
}

Tile::~Tile()
{
    /*Does nothing*/
}

void Tile::clear()
{
    type = 'e';
    massive = false;
    peg = false;
    bonus = false;
}

void Tile::layout()
{
    switch(type)
    {
        case '.':
        {
            setPixmap(QPixmap(":/Graphics/peg.png"));
            setPos(x,y);
            peg = true;
            break;
        }
        case '*':
        {
            setPixmap(QPixmap(":/Graphics/largePeg.png"));
            setPos(x,y);
            bonus = true;
            break;
        }
        case 'h':
        {
            setPixmap(QPixmap(":/Graphics/wallH.png"));
            setPos(x,y);
            massive = true;
            break;
        }
        case 'v':
        {
            setPixmap(QPixmap(":/Graphics/wallV.png"));
            setPos(x,y);
            massive = true;
            break;
        }
        case 'a':
        {
            setPixmap(QPixmap(":/Graphics/corner0.png"));
            setPos(x,y);
            massive = true;
            break;
        }
        case 'b':
        {
            setPixmap(QPixmap(":/Graphics/corner90.png"));
            setPos(x,y);
            massive = true;
            break;
        }
        case 'c':
        {
            setPixmap(QPixmap(":/Graphics/corner180.png"));
            setPos(x,y);
            massive = true;
            break;
        }
        case 'd':
        {
            setPixmap(QPixmap(":/Graphics/corner270.png"));
            setPos(x,y);
            massive = true;
            break;
        }
        case 'i':
        {
            setPixmap(QPixmap(":/Graphics/junctionVU.png"));
            setPos(x,y);
            massive = true;
            break;
        }
        case 'j':
        {
            setPixmap(QPixmap(":/Graphics/junctionHR.png"));
            setPos(x,y);
            massive = true;
            break;
        }
        case 'k':
        {
            setPixmap(QPixmap(":/Graphics/junctionVD.png"));
            setPos(x,y);
            massive = true;
            break;
        }
        case 'l':
        {
            setPixmap(QPixmap(":/Graphics/junctionHL.png"));
            setPos(x,y);
            massive = true;
            break;
        }
    }
}
