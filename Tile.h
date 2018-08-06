#ifndef TILE_H
#define TILE_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>


class Tile: public QObject, public QGraphicsPixmapItem
{
public:
    //Constructors and destructors
    Tile(QGraphicsItem *parent=0);
    Tile(char t, int x_coord, int y_coord, QGraphicsItem *parent=0);
    ~Tile();

    //Public variables
    bool massive;
    bool peg;
    bool bonus;

    //Public member functions
    void clear();

private:
    //Private variables
    char type;
    int x;
    int y;

    //Private member functions
    void layout();
};

#endif // TILE_H
