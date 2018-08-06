#include "Ghosts.h"
#include "Game.h"
#include <stdlib.h>

extern Game *game;

Ghosts::Ghosts(std::string c): type(c)
{
    //Ghost constructor. There are 4 different types of enemies
    if(type == "pink") setPixmap(QPixmap(":/Graphics/pinkU1.png"));
    else if(type == "red") setPixmap(QPixmap(":/Graphics/redU1.png"));
    else if(type == "blue") setPixmap(QPixmap(":/Graphics/blueU1.png"));
    else if(type == "yellow") setPixmap(QPixmap(":/Graphics/yellowU1.png"));

    //Set the ghost's initial parameters
    animationState = 1;
    nextDirection = 'U';
    connect(movementTimer,SIGNAL(timeout()),this,SLOT(prepareMove()));
    connect(movementTimer,SIGNAL(timeout()),this,SLOT(animate()));
}

void Ghosts::startMoving()
{
    movementTimer->start(1);
}

//void Ghosts::animate()
//{
//    if(animationState == 2 && direction == 'U')
//    {
//        if(type == "pink") setPixmap(QPixmap(":/Graphics/pinkU1.png"));
//        else if(type == "red") setPixmap(QPixmap(":/Graphics/redU1.png"));
//        else if(type == "blue") setPixmap(QPixmap(":/Graphics/blueU1.png"));
//        else if(type == "yellow") setPixmap(QPixmap(":/Graphics/yellowU1.png"));
//        animationState = 1;
//        return;
//    }
//    else if(animationState == 2 && direction == 'D')
//    {
//        if(type == "pink") setPixmap(QPixmap(":/Graphics/pinkD1.png"));
//        else if(type == "red") setPixmap(QPixmap(":/Graphics/redD1.png"));
//        else if(type == "blue") setPixmap(QPixmap(":/Graphics/blueD1.png"));
//        else if(type == "yellow") setPixmap(QPixmap(":/Graphics/yellowD1.png"));
//        animationState = 1;
//        return;
//    }
//    else if(animationState == 2 && direction == 'L')
//    {
//        if(type == "pink") setPixmap(QPixmap(":/Graphics/pinkL1.png"));
//        else if(type == "red") setPixmap(QPixmap(":/Graphics/redL1.png"));
//        else if(type == "blue") setPixmap(QPixmap(":/Graphics/blueL1.png"));
//        else if(type == "yellow") setPixmap(QPixmap(":/Graphics/yellowL1.png"));
//        animationState = 1;
//        return;
//    }
//    else if(animationState == 2 && direction == 'R')
//    {
//        if(type == "pink") setPixmap(QPixmap(":/Graphics/pinkR1.png"));
//        else if(type == "red") setPixmap(QPixmap(":/Graphics/redR1.png"));
//        else if(type == "blue") setPixmap(QPixmap(":/Graphics/blueR1.png"));
//        else if(type == "yellow") setPixmap(QPixmap(":/Graphics/yellowR1.png"));
//        animationState = 1;
//        return;
//    }
//    else if(animationState == 1 && direction == 'U')
//    {
//        if(type == "pink") setPixmap(QPixmap(":/Graphics/pinkU2.png"));
//        else if(type == "red") setPixmap(QPixmap(":/Graphics/redU2.png"));
//        else if(type == "blue") setPixmap(QPixmap(":/Graphics/blueU2.png"));
//        else if(type == "yellow") setPixmap(QPixmap(":/Graphics/yellowU2.png"));
//        animationState = 2;
//        return;
//    }
//    else if(animationState == 1 && direction == 'D')
//    {
//        if(type == "pink") setPixmap(QPixmap(":/Graphics/pinkD2.png"));
//        else if(type == "red") setPixmap(QPixmap(":/Graphics/redD2.png"));
//        else if(type == "blue") setPixmap(QPixmap(":/Graphics/blueD2.png"));
//        else if(type == "yellow") setPixmap(QPixmap(":/Graphics/yellowD2.png"));
//        animationState = 2;
//        return;
//    }
//    else if(animationState == 1 && direction == 'L')
//    {
//        if(type == "pink") setPixmap(QPixmap(":/Graphics/pinkL2.png"));
//        else if(type == "red") setPixmap(QPixmap(":/Graphics/redL2.png"));
//        else if(type == "blue") setPixmap(QPixmap(":/Graphics/blueL2.png"));
//        else if(type == "yellow") setPixmap(QPixmap(":/Graphics/yellowL2.png"));
//        animationState = 2;
//        return;
//    }
//    else if(animationState == 1 && direction == 'R')
//    {
//        if(type == "pink") setPixmap(QPixmap(":/Graphics/pinkR2.png"));
//        else if(type == "red") setPixmap(QPixmap(":/Graphics/redR2.png"));
//        else if(type == "blue") setPixmap(QPixmap(":/Graphics/blueR2.png"));
//        else if(type == "yellow") setPixmap(QPixmap(":/Graphics/yellowR2.png"));
//        animationState = 2;
//        return;
//    }

//}

void Ghosts::doMove()
{
    //The player doMove() private member slot is responsible for the
    //actual movement. The position is changer in units of
    //pixels per timeout. The amount of pixels with which we can change
    //the position has to be a divider of the tile size (20 pixels)
    if(direction == 'U') setPos(x(),y()-2);
    else if(direction == 'D') setPos(x(),y()+2);
    else if(direction == 'L') setPos(x()-2,y());
    else if(direction == 'R') setPos(x()+2,y());

    //The doMove function also takes care of animating the movement

    movementTracker++;

    if(movementTracker==10)
    {
        connect(movementTimer, SIGNAL(timeout()), this, SLOT(prepareMove()));
        disconnect(movementTimer, SIGNAL(timeout()), this, SLOT(doMove()));
        return;
    }
}

void Ghosts::prepareMove()
{
    //A lot of code for the Ghost move member function is similar to
    //Player move member function.
    //Get the ghost's current tile coordinates, and tile number
    qDebug() << "Checkpoint 1";
    int xTile = (x())/20;
    int yTile = (y())/20;
    int currentTileNumber = xTile + yTile*36 + 1;

    //The ghost object needs some info about it's surroundings. Check
    //if the next tiles are massive (=walls)
    qDebug() << "Checkpoint 2";
    bool UIsWall = game->level->isMassive(currentTileNumber-36);
    bool DIsWall = game->level->isMassive(currentTileNumber+36);
    bool LIsWall = game->level->isMassive(currentTileNumber-1);
    bool RIsWall = game->level->isMassive(currentTileNumber+1);

    //Check if the ghost is on exactly one tile and is not moving between
    //different tiles
    qDebug() << "Checkpoint 3";
    int reminder = int(x()) % 20 + int(y()) % 20;

    //Check that the ghost is at an intersection
    bool atIntersection = false;
    if(reminder == 0)
    {
        if((direction == 'U' && !LIsWall) || (direction == 'U' && !RIsWall)) atIntersection = true;
        else if((direction == 'D' && !LIsWall) || (direction == 'D' && !RIsWall)) atIntersection = true;
        else if((direction == 'L' && !UIsWall) || (direction == 'L' && !DIsWall)) atIntersection = true;
        else if((direction == 'R' && !UIsWall) || (direction == 'R' && !DIsWall)) atIntersection = true;
        else atIntersection = false;
    }

    //Show that the ghost is at an intersection
    if(atIntersection) qDebug() << atIntersection;

    //If the ghost is at an intersection, get the next direction.
    if(atIntersection)
    {
        bool invalidDirection = true;
        do
        {
            int randomDirection = rand() % 4;

            //Get a random nextdirection
            if(randomDirection == 0) nextDirection = 'U';
            else if(randomDirection == 1) nextDirection = 'D';
            else if(randomDirection == 2) nextDirection = 'L';
            else if(randomDirection == 3) nextDirection = 'R';

            //Check that the next random direction does not send the ghost
            //into a massive wall
            if(nextDirection == 'U' && UIsWall) invalidDirection = true;
            else if(nextDirection == 'D' && DIsWall) invalidDirection = true;
            else if(nextDirection == 'L' && LIsWall) invalidDirection = true;
            else if(nextDirection == 'R' && RIsWall) invalidDirection = true;
            else  invalidDirection = false;

            qDebug() << "stuck in loop" << randomDirection << invalidDirection;

        } while(invalidDirection);
    }

    //If the ghost gets to the teleporter, do the teleportation
    //The teleporters are always on the same tiles in every level
    //Their behavior is therefore hard-coded. Might want to change this...
    if(currentTileNumber == 509 && direction == 'L') setPos(600,280);
    else if(currentTileNumber == 545 && direction == 'L') setPos(600,300);
    else if(currentTileNumber == 536 && direction == 'R') setPos(100,280);
    else if(currentTileNumber == 572  && direction == 'R') setPos(100,300);



   direction = nextDirection;
   nextDirection = 'o';
    //If the player wants to move in a direction that is free (no walls)
    //and is not on a special tile or has already processed special tile,
    //do the actual moving. The actual movement has been delegated to
    //the private member slot doMove().
    movementTracker = 0;
    connect(movementTimer, SIGNAL(timeout()), this, SLOT(doMove()));
    disconnect(movementTimer, SIGNAL(timeout()), this, SLOT(prepareMove()));
    return;
}
