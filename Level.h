#ifndef LEVEL_H
#define LEVEL_H

#include "Tile.h"
#include <map>
#include <QObject>
#include <QGraphicsPixmapItem>


class Level: public QObject, public QGraphicsPixmapItem
{
    //Include the Q_OBJECT macro to make the slots function properly
    Q_OBJECT

public:
    //Constructors and destructors. The default constructor can be called
    //and will delegate to the level-based constructor with a value of 1
    Level();
    ~Level();

    //Public variables: The tileAdress map is a map that linkt the
    //number of the tile to the adress of the tile object. That way
    //we have access to every tile object (stored on the HEAP)!
    std::map<int, Tile*> tileAdresses;

    //Public member functions
    void build(int currentLevel);
    int getNumberOfPegs() const;

private:
    //Private variables
    int numberOfPegs;
};


//The different Tile types are:
//'e'                   empty tile
//'.' '*'               small, bonus peg
//'h' 'v'               horizontal, vertical wall
//'a' 'b' 'c' 'd'       corners
//'i' 'j' 'k' 'l'       junctions

#endif // LEVEL_H
