#include "MovingObjects.h"
#include "Game.h"
#include <QString>
#include <math.h>


extern Game *game;

//The generic MovingObject member functions
void MovingObject::startMoving()
{
    movementTimer->start(1);
    animationTimer->start(50);
}

void MovingObject::stopMoving()
{
    movementTimer->stop();
    animationTimer->stop();
}

void MovingObject::doMove()
{
    //The doMove() member slot is responsible for the
    //actual movement. The position is changed in units of
    //pixels per timeout. The amount of pixels with which we can change
    //the position has to be a divider of the tile size (20 pixels)
    if(direction == 'U') setPos(x(),y()-speed);
    else if(direction == 'D') setPos(x(),y()+speed);
    else if(direction == 'L') setPos(x()-speed,y());
    else if(direction == 'R') setPos(x()+speed,y());
    movementTracker++;

    if(movementTracker == 20/speed)
    {
        //The movement tracker is used to determine whether the object is moving between
        //tiles, or is on exactly one tile. If it is moving, it continues to move. But if
        //it if on exactly one tile, the doMove function is no longer called. The prepareMove
        //function will be called instead and it will determine where to move to. Changing directions
        //can only be done if the object is on exactly one tile, and is done by the prepareMove function
        connect(movementTimer, SIGNAL(timeout()), this, SLOT(prepareMove()));
        disconnect(movementTimer, SIGNAL(timeout()), this, SLOT(doMove()));
        return;
    }
}





//The Player specific member functions
Player::Player()
{
    //Initialise all variables to their begin values
    speed = 2;
    movementTracker = 0;
    animationState = 1;
    direction = 'U';
    nextDirection = 'o';
    state = "alive";
    ghostsKilled = 0;
    numberOfPegsEaten = 0;
    sound = new QMediaPlayer;
    sound->setMedia(QUrl("qrc:Sounds/pacman_death.wav"));

    //Set the player image
    setPixmap(QPixmap(":/Graphics/pacman1.png"));

    //Set the timers to track the movement and animation,
    //and connect the timers to the functions handeling movement and animation
    movementTimer = new QTimer();
    animationTimer = new QTimer();
    connect(movementTimer,SIGNAL(timeout()),this,SLOT(prepareMove()));
    connect(animationTimer,SIGNAL(timeout()),this,SLOT(animate()));
}

Player::~Player()
{
    delete sound;
}

void Player::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Up:
        {
            nextDirection = 'U';
            break;
        }

        case Qt::Key_Down:
        {
            nextDirection = 'D';
            break;
        }

        case Qt::Key_Left:
        {
            nextDirection = 'L';
            break;
        }

        case Qt::Key_Right:
        {
            nextDirection = 'R';
            break;
        }
    }
}

void Player::resetNumberOfKills()
{
    ghostsKilled = 0;
}

void Player::increaseNumberOfKills()
{
    ghostsKilled++;
}

int Player::numberOfGhostsKilled() const
{
    return ghostsKilled;
}

void Player::dead()
{
    //Stop and die
    stopMoving();
    state = "dead";

    //Play the dying sound
    sound->play();

    //Play the dying animation
    animationTimer->start(100);
    animationState = 1;
}

void Player::reset()
{
    //This function makes the player alive again and sets the game to continue
    setPos(340,440);
    setPixmap(QPixmap(":/Graphics/pacman1.png"));
    state = "alive";
    animationState = 1;
    movementTracker = 0;
    direction = 'o';
    nextDirection = 'U';
}

void Player::ressurect()
{
    //This function makes the player alive again and sets the game to continue
    setPos(340,440);
    setPixmap(QPixmap(":/Graphics/pacman1.png"));
    state = "alive";
    animationState = 1;
    movementTracker = 0;
    direction = 'o';
    nextDirection = 'U';
    game->setMessage("Get ready!",289,306);
    QTimer::singleShot(2000, this, SLOT(startMoving()));
    QTimer::singleShot(2000, game, SLOT(clearMessage()));
}

void Player::animate()
{
    //This player member function is needed to allow the player to be animated. It is
    //poorly written, but it works. It is possible to implement the same functionality
    //with the use of less graphics and by implementing rotations and using a .gif,
    //instead of 'hacking' the animation by looping 3 different images. TO FIX
    if(state == "alive")
    {
    switch(animationState)
    {
        case 1:
        {
            if(direction == 'U') setPixmap(QPixmap(":/Graphics/pacman2U.png"));
            else if(direction == 'D') setPixmap(QPixmap(":/Graphics/pacman2D.png"));
            else if(direction == 'L') setPixmap(QPixmap(":/Graphics/pacman2L.png"));
            else if(direction == 'R') setPixmap(QPixmap(":/Graphics/pacman2R.png"));
            animationState = 2;
            break;
        }
        case 2:
        {
            if(direction == 'U') setPixmap(QPixmap(":/Graphics/pacman3U.png"));
            else if(direction == 'D') setPixmap(QPixmap(":/Graphics/pacman3D.png"));
            else if(direction == 'L') setPixmap(QPixmap(":/Graphics/pacman3L.png"));
            else if(direction == 'R') setPixmap(QPixmap(":/Graphics/pacman3R.png"));
            animationState = 3;
            break;
        }
        case 3:
        {
            if(direction == 'U') setPixmap(QPixmap(":/Graphics/pacman2U.png"));
            else if(direction == 'D') setPixmap(QPixmap(":/Graphics/pacman2D.png"));
            else if(direction == 'L') setPixmap(QPixmap(":/Graphics/pacman2L.png"));
            else if(direction == 'R') setPixmap(QPixmap(":/Graphics/pacman2R.png"));
            animationState = 4;
            break;
        }
        case 4:
        {
            setPixmap(QPixmap(":/Graphics/pacman1.png"));
            animationState = 1;
            break;
        }
    }
    }
    else if(state == "dead")
    {
        switch(animationState)
        {
            case 1:
            {
                setPixmap(QPixmap(":/Graphics/pacman1.png"));
                animationState = 2;
                break;
            }
            case 2:
            {
                setPixmap(QPixmap(":/Graphics/pacman2R.png"));
                animationState = 3;
                break;
            }
            case 3:
            {
                setPixmap(QPixmap(":/Graphics/pacman3R.png"));
                animationState = 4;
                break;
            }
            case 4:
            {
                setPixmap(QPixmap(":/Graphics/pacmanDead1.png"));
                animationState = 5;
                break;
            }
            case 5:
            {
                setPixmap(QPixmap(":/Graphics/pacmanDead2.png"));
                animationState = 6;
                break;
            }
            case 6:
            {
                setPixmap(QPixmap(":/Graphics/pacmanDead3.png"));
                animationState = 7;
                break;
            }
            case 7:
            {
                setPixmap(QPixmap(":/Graphics/pacmanDead4.png"));
                animationState = 8;
                break;
            }
            case 8:
            {
                setPixmap(QPixmap(":/Graphics/pacmanDead5.png"));
                animationState = 9;
                break;
            }
            case 9:
            {
                setPixmap(QPixmap(":/Graphics/pacmanDead6.png"));
                animationState = 10;
                break;
            }
            case 10:
            {
                setPixmap(QPixmap(":/Graphics/pacmanDead7.png"));
                animationState = 11;
                break;
            }
            case 11:
            {
                setPixmap(QPixmap(":/Graphics/pacmanDead8.png"));
                animationState = 12;
                break;
            }
            case 12:
            {
                setPixmap(QPixmap(":/Graphics/pacmanDead9.png"));
                animationState = 13;
                break;
            }
            case 13:
            {
                setPixmap(QPixmap(":/Graphics/pacmanDead10.png"));
                animationState = 14;
                break;
            }
            case 14:
            {
                setPixmap(QPixmap(":/Graphics/pacmanDead11.png"));
                animationState = 15;
                break;
            }
            case 15:
            {
                setPixmap(QPixmap(":/Graphics/pacmanDead12.png"));
                animationState = 16;
                break;
            }
        }
    }
}

void Player::prepareMove()
{
    //Allways focus on the player
    this->setFocus();
    //The prepare move member function slot runs through several verifications
    //to make sure that the player can move SMOOTHLY and in the right DIRECTION
    //without hitting a WALL.
    //It also BUFFERS the next move until the player is ABLE to change it's direction

    //First step it to check whether the game is still on. This should be extended to
    //some sort of levelClear function that advances to a next level...
    if(numberOfPegsEaten == game->level->getNumberOfPegs())
    {
        numberOfPegsEaten = 0;
        game->levelCleared();
    }
    else
    {


    //The level, or map, is divided in tiles that are 20 pixels wide and
    //20 pixels high. The first step is to determine in which tile number
    //the player is located. Each tile has a number, starting from 0, running along the
    //first row to 36, and then the lower row starts at 37 etc...
    //    0    1    2    3    4    5    ...
    //    37   38   39   40   41   42   ...
    //    ...
    //    ...                                1149   1150   1151   1152

    //Compute the current tile number of the reference point
    //which is the upper left corner of the player rectangle.
    int xTile = (x())/20;
    int yTile = (y())/20;
    int currentTileNumber = xTile + yTile*36 + 1;

    //The player object needs some info about it's surroundings. Check
    //if the next tiles are massive (= walls)
    bool UIsWall = game->level->tileAdresses[currentTileNumber-36]->massive;
    bool DIsWall = game->level->tileAdresses[currentTileNumber+36]->massive;
    bool LIsWall = game->level->tileAdresses[currentTileNumber-1]->massive;
    bool RIsWall = game->level->tileAdresses[currentTileNumber+1]->massive;

    //Check that the player wants to change direction and that
    //it is allowed to change direction
    //Reminder checks that the coordinates of the player object are
    //divisible by 20. If they both are, the reminder will be 0 and the
    //player is in exactly one tile. Which means that it is ALLOWED
    //to chance directions
    int reminder = int(x()) % 20 + int(y()) % 20;

    if(nextDirection != 'o' &&
       direction != nextDirection &&
       reminder == 0)
    {
        //The previous condition checked that
        //   a) the player has ENTERED the next direction in which to travel
        //   b) the wants to CHANGE direction
        //   c) the player is in a integer tile and CAN change direction
        //The next conditions check that the player will not hit a wall
        //by changing direction. Syntax chosen to be more read friendly
        if(nextDirection == 'U' && UIsWall){/*do nothing*/}
        else if(nextDirection == 'D' && DIsWall){/*do nothing*/}
        else if(nextDirection == 'L' && LIsWall){/*do nothing*/}
        else if(nextDirection == 'R' && RIsWall){/*do nothing*/}
        else
        {
            //After checking all conditions, change directions and empty nextDirection buffer
            direction = nextDirection;
            nextDirection = 'o';
        }
    }

    //If the player hits a wall, stop moving
    if(direction == 'U' && UIsWall) {animationTimer->stop(); return;}
    else if(direction == 'D' && DIsWall) {animationTimer->stop(); return;}
    else if(direction == 'L' && LIsWall) {animationTimer->stop(); return;}
    else if(direction == 'R' && RIsWall) {animationTimer->stop(); return;}
    else if(!animationTimer->isActive()) animationTimer->start(50);

    //If the player gets to the teleporter, do the teleportation
    //The teleporters are always on the same tiles in every level
    //Their behavior is therefore hard-coded. Might want to change this...
    if(currentTileNumber == 509 && direction == 'L') setPos(600,280);
    else if(currentTileNumber == 536 && direction == 'R') setPos(100,280);

    //If the player steps on a peg, the score will be updated and the peg
    //will dissapear, or on a bonus the ghost will be scared
    if(game->level->tileAdresses[currentTileNumber]->peg == true)
    {
        game->score->increase(10);
        scene()->removeItem(game->level->tileAdresses[currentTileNumber]);
        game->level->tileAdresses[currentTileNumber]->clear();
        numberOfPegsEaten++;
    }
    else if(game->level->tileAdresses[currentTileNumber]->bonus == true)
    {
        game->score->increase(100);
        scene()->removeItem(game->level->tileAdresses[currentTileNumber]);
        game->level->tileAdresses[currentTileNumber]->clear();

        for(auto T : game->enemies) T->isScared();

        numberOfPegsEaten++;
    }

    //If the player wants to move in a direction that is free (no walls)
    //and is not on a special tile or has already processed special tile,
    //do the actual moving. The actual movement has been delegated to
    //the private member slot doMove(). The movementTracker makes sure that the
    //function is called for as long as it takes to cross a tile. Once a tile
    //is crosses this (prepareMove) function will be called again and all info about
    //movement, pegs, changing directions etc. will be evaluated again
    movementTracker = 0;
    connect(movementTimer, SIGNAL(timeout()), this, SLOT(doMove()));
    disconnect(movementTimer, SIGNAL(timeout()), this, SLOT(prepareMove()));
    return;
}
}






//The ghost specific member functions
Ghost::Ghost(std::string t): type(t)
{
    //Initialise all variables to their begin values
    reverse = 0;
    speed = 2;
    movementTracker = 0;
    animationState = 1;
    direction = 'U';
    nextDirection = 'U';
    state = "scatter";
    sound = new QMediaPlayer;
    sound->setMedia(QUrl("qrc:Sounds/pacman_eatghost.wav"));
    scatterTime = 3000;
    chaseTime = 5000;

    //Set the ghost images... There are four kind of ghosts
    if(type == "pink") setPixmap(QPixmap(":/Graphics/pinkU1.png"));
    else if(type == "red") setPixmap(QPixmap(":/Graphics/redU1.png"));
    else if(type == "blue") setPixmap(QPixmap(":/Graphics/blueU1.png"));
    else if(type == "yellow") setPixmap(QPixmap(":/Graphics/yellowU1.png"));

    //Set the timers to track the movement and animation,
    //and connect the timers to the functions handeling movement and animation
    movementTimer = new QTimer();
    animationTimer = new QTimer();
    stateTimer = new QTimer();
    connect(movementTimer,SIGNAL(timeout()),this,SLOT(prepareMove()));
    connect(animationTimer,SIGNAL(timeout()),this,SLOT(animate()));
    connect(stateTimer,SIGNAL(timeout()),this,SLOT(chase()));
    stateTimer->start(6000);
}

Ghost::~Ghost()
{
    delete sound;
}

void Ghost::scatter()
{
    stateTimer->stop();
    stateTimer->disconnect();
    if(state == "chase") state = "scatter";
    connect(stateTimer,SIGNAL(timeout()),this,SLOT(chase()));
    stateTimer->start(scatterTime);
}

void Ghost::chase()
{
    stateTimer->stop();
    stateTimer->disconnect();
    if(state == "scatter") state = "chase";
    connect(stateTimer,SIGNAL(timeout()),this,SLOT(scatter()));
    stateTimer->start(chaseTime);
}

void Ghost::isScared()
{
    state = "scared";
    reverse = true;
    QTimer::singleShot(4500, this, SLOT(scaredEnding()));
    QTimer::singleShot(6000, this, SLOT(isNotScared()));
}

void Ghost::scaredEnding()
{
    animationState = 3;
}

void Ghost::isNotScared()
{
    game->player->resetNumberOfKills();
    animationState = 1;
    state = "scatter";
    reverse = true;
}

void Ghost::dead()
{
    //Play the dying sound
    sound->play();

    //Reset the ghost
    reset(3000);

    //Update the score incrementally
    int ghostsKilled = game->player->numberOfGhostsKilled();
    if(ghostsKilled == 0) game->score->increase(200);
    else if(ghostsKilled == 1) game->score->increase(400);
    else if(ghostsKilled == 2) game->score->increase(800);
    else if(ghostsKilled == 3) game->score->increase(1200);
    game->player->increaseNumberOfKills();
}

void Ghost::reset()
{
    reset(4500);
}

void Ghost::reset(int when)
{
    //Stop the moving and reset to begin values
    stopMoving();
    movementTracker = 0;
    animationState = 1;
    direction = 'U';
    nextDirection = 'R';
    state = "scatter";

    //Set the correct image and position
    if(type == "pink")
    {
        setPos(320,280);
        setPixmap(QPixmap(":/Graphics/pinkU1.png"));
    }
    else if(type == "blue")
    {
        setPos(340,280);
        setPixmap(QPixmap(":/Graphics/blueU1.png"));
    }
    else if(type == "yellow")
    {
        setPos(360,280);
        setPixmap(QPixmap(":/Graphics/yellowU1.png"));
    }
    else if(type == "red")
    {
        setPos(380,280);
        setPixmap(QPixmap(":/Graphics/redU1.png"));
    }

    //Give the player a little head start
    QTimer::singleShot(when, this, SLOT(startMoving()));
}

void Ghost::doMove()
{
    //First check that the ghost is not colliding with the player. If it is, the player
    //(or the ghost) has to die. Use the C+11 range based for loop
    QList<QGraphicsItem *> colliding = collidingItems();
    for(auto T : colliding)
    {
        if(typeid(*T) == typeid(Player)  && state != "scared") game->playerDied();
        else if((typeid(*T) == typeid(Player)  && state == "scared")) this->dead();
    }

    //Call the base class member function to do the rest
    MovingObject::doMove();
}

void Ghost::animate()
{
    //This cumbersome function takes care of animating the
    //movement of the ghosts. Using gifs and rotations would
    //significantly reduce the size of this function
         if(animationState == 1 && direction == 'U' && type == "pink" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/pinkU2.png"));
        animationState = 2;
        return;
    }
    else if(animationState == 2 && direction == 'U' && type == "pink" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/pinkU1.png"));
        animationState = 1;
        return;
    }
    else if(animationState == 1 && direction == 'D' && type == "pink" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/pinkD2.png"));
        animationState = 2;
        return;
    }
    else if(animationState == 2 && direction == 'D' && type == "pink" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/pinkD1.png"));
        animationState = 1;
        return;
    }
    else if(animationState == 1 && direction == 'L' && type == "pink" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/pinkL2.png"));
        animationState = 2;
        return;
    }
    else if(animationState == 2 && direction == 'L' && type == "pink" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/pinkL1.png"));
        animationState = 1;
        return;
    }
    else if(animationState == 1 && direction == 'R' && type == "pink" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/pinkR2.png"));
        animationState = 2;
        return;
    }
    else if(animationState == 2 && direction == 'R' && type == "pink" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/pinkR1.png"));
        animationState = 1;
        return;
    }
    else if(animationState == 1 && direction == 'U' && type == "blue" && (state == "chase" || state == "scatter"))
        {
            setPixmap(QPixmap(":/Graphics/blueU2.png"));
            animationState = 2;
            return;
        }
    else if(animationState == 2 && direction == 'U' && type == "blue" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/blueU1.png"));
        animationState = 1;
        return;
    }
    else if(animationState == 1 && direction == 'D' && type == "blue" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/blueD2.png"));
        animationState = 2;
        return;
    }
    else if(animationState == 2 && direction == 'D' && type == "blue" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/blueD1.png"));
        animationState = 1;
        return;
    }
    else if(animationState == 1 && direction == 'L' && type == "blue" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/blueL2.png"));
        animationState = 2;
        return;
    }
    else if(animationState == 2 && direction == 'L' && type == "blue" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/blueL1.png"));
        animationState = 1;
        return;
    }
    else if(animationState == 1 && direction == 'R' && type == "blue" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/blueR2.png"));
        animationState = 2;
        return;
    }
    else if(animationState == 2 && direction == 'R' && type == "blue" && (state == "chase" || state == "scatter"))
        {
            setPixmap(QPixmap(":/Graphics/blueR1.png"));
            animationState = 1;
            return;
        }
    else if(animationState == 1 && direction == 'U' && type == "yellow" && (state == "chase" || state == "scatter"))
        {
            setPixmap(QPixmap(":/Graphics/yellowU2.png"));
            animationState = 2;
            return;
        }
    else if(animationState == 2 && direction == 'U' && type == "yellow" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/yellowU1.png"));
        animationState = 1;
        return;
    }
    else if(animationState == 1 && direction == 'D' && type == "yellow" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/yellowD2.png"));
        animationState = 2;
        return;
    }
    else if(animationState == 2 && direction == 'D' && type == "yellow" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/yellowD1.png"));
        animationState = 1;
        return;
    }
    else if(animationState == 1 && direction == 'L' && type == "yellow" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/yellowL2.png"));
        animationState = 2;
        return;
    }
    else if(animationState == 2 && direction == 'L' && type == "yellow" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/yellowL1.png"));
        animationState = 1;
        return;
    }
    else if(animationState == 1 && direction == 'R' && type == "yellow" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/yellowR2.png"));
        animationState = 2;
        return;
    }
    else if(animationState == 2 && direction == 'R' && type == "yellow" && (state == "chase" || state == "scatter"))
        {
            setPixmap(QPixmap(":/Graphics/yellowR1.png"));
            animationState = 1;
            return;
        }
    else if(animationState == 1 && direction == 'U' && type == "red" && (state == "chase" || state == "scatter"))
        {
            setPixmap(QPixmap(":/Graphics/redU2.png"));
            animationState = 2;
            return;
        }
    else if(animationState == 2 && direction == 'U' && type == "red" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/redU1.png"));
        animationState = 1;
        return;
    }
    else if(animationState == 1 && direction == 'D' && type == "red" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/redD2.png"));
        animationState = 2;
        return;
    }
    else if(animationState == 2 && direction == 'D' && type == "red" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/redD1.png"));
        animationState = 1;
        return;
    }
    else if(animationState == 1 && direction == 'L' && type == "red" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/redL2.png"));
        animationState = 2;
        return;
    }
    else if(animationState == 2 && direction == 'L' && type == "red" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/redL1.png"));
        animationState = 1;
        return;
    }
    else if(animationState == 1 && direction == 'R' && type == "red" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/redR2.png"));
        animationState = 2;
        return;
    }
    else if(animationState == 2 && direction == 'R' && type == "red" && (state == "chase" || state == "scatter"))
    {
        setPixmap(QPixmap(":/Graphics/redR1.png"));
        animationState = 1;
        return;
    }
    else if(animationState == 1 && state == "scared")
    {
        setPixmap(QPixmap(":/Graphics/ScaredBlue1.png"));
        animationState = 2;
        return;
    }
    else if(animationState == 2 && state == "scared")
    {
        setPixmap(QPixmap(":/Graphics/ScaredBlue2.png"));
        animationState = 1;
        return;
    }
    else if(animationState == 3 && state == "scared")
    {
        setPixmap(QPixmap(":/Graphics/ScaredWhite1.png"));
        animationState = 4;
        return;
    }
    else if(animationState == 4 && state == "scared")
    {
        setPixmap(QPixmap(":/Graphics/ScaredWhite2.png"));
        animationState = 5;
        return;
    }
    else if(animationState == 5 && state == "scared")
    {
        setPixmap(QPixmap(":/Graphics/ScaredBlue1.png"));
        animationState = 6;
        return;
    }
    else if(animationState == 6 && state == "scared")
    {
        setPixmap(QPixmap(":/Graphics/ScaredBlue2.png"));
        animationState = 3;
        return;
    }
}

void Ghost::prepareMove()
{
    //First check that the ghost is not colliding with the player. If it is, the player
    //(or the ghost) has to die. Use the C+11 range based for loop
    QList<QGraphicsItem *> colliding = collidingItems();
    for(auto T : colliding)
    {
        if(typeid(*T) == typeid(Player)  && state != "scared")
        {
            game->playerDied();
            return;
        }
        else if((typeid(*T) == typeid(Player)  && state == "scared")) this->dead();

    }

    //A lot of code for the Ghost move member function is similar to
    //Player move member function.
    //Get the ghost's current tile coordinates, and tile number
    int xTile = (x())/20;
    int yTile = (y())/20;
    int currentTileNumber = xTile + yTile*36 + 1;

    //The ghost object needs some info about it's surroundings. Check
    //if the next tiles are massive (=walls)
    bool UIsWall = game->level->tileAdresses[currentTileNumber-36]->massive;
    bool DIsWall = game->level->tileAdresses[currentTileNumber+36]->massive;
    bool LIsWall = game->level->tileAdresses[currentTileNumber-1]->massive;
    bool RIsWall = game->level->tileAdresses[currentTileNumber+1]->massive;

    //Check if the ghost is on exactly one tile and not moving between
    //different tiles
    int reminder = int(x()) % 20 + int(y()) % 20;

    //Check that the ghost is at an intersection by looking whether there is an
    //opening in the wall on one of the ghost's sides
    bool atIntersection = false;
    if(reminder == 0)
    {
        if((direction == 'U' && !LIsWall) || (direction == 'U' && !RIsWall)) atIntersection = true;
        else if((direction == 'D' && !LIsWall) || (direction == 'D' && !RIsWall)) atIntersection = true;
        else if((direction == 'L' && !UIsWall) || (direction == 'L' && !DIsWall)) atIntersection = true;
        else if((direction == 'R' && !UIsWall) || (direction == 'R' && !DIsWall)) atIntersection = true;
        else atIntersection = false;
    }

    //If the ghost is at an intersection, get the next direction. This is now done randomly
    //Might want to write a smarter path tracking algorithm
    if(atIntersection && !reverse)
    {
        bool invalidDirection = true;
        if(state == "scatter" || state == "scared")
        {
            do
            {
                int randomDirection = rand() % 4;

                //Get a random nextdirection
                if(randomDirection == 0 && direction != 'D') nextDirection = 'U';
                else if(randomDirection == 1 && direction != 'U') nextDirection = 'D';
                else if(randomDirection == 2 && direction != 'R') nextDirection = 'L';
                else if(randomDirection == 3 && direction != 'L') nextDirection = 'R';

                //Check that the next random direction does not send the ghost
                //into a massive wall, and prevent the ghost from turning around
                if(nextDirection == 'U' && UIsWall) invalidDirection = true;
                else if(nextDirection == 'D' && DIsWall) invalidDirection = true;
                else if(nextDirection == 'L' && LIsWall) invalidDirection = true;
                else if(nextDirection == 'R' && RIsWall) invalidDirection = true;
                else  invalidDirection = false;


            } while(invalidDirection);
        }
        else if(state == "chase")
        {
            //Get the player's position
            int targetX = game->player->x();
            int targetY = game->player->y();

            if(type == "red") {/* do nothing */}
            else if(type == "pink") targetX+=20;
            else if(type == "blue") targetY-=40;
            else if(type == "yellow")
            {
                    targetX+=20;
                    targetY-=20;
            }

            double distance =  pow(this->x()-targetX,2) + pow(this->y()-targetY,2);
            double distanceU = pow(this->x()-targetX,2) + pow(this->y()-20-targetY,2);
            double distanceD = pow(this->x()-targetX,2) + pow(this->y()+20-targetY,2);
            double distanceL = pow(this->x()-20-targetX,2) + pow(this->y()-targetY,2);
            double distanceR = pow(this->x()+20-targetX,2) + pow(this->y()-targetY,2);

            if     (distance>distanceU && !UIsWall && direction != 'D') nextDirection = 'U';
            else if(distance>distanceD && !DIsWall && direction != 'U') nextDirection = 'D';
            else if(distance>distanceL && !LIsWall && direction != 'R') nextDirection = 'L';
            else if(distance>distanceR && !RIsWall && direction != 'L') nextDirection = 'R';
            else if(!UIsWall && direction != 'D') nextDirection = 'U';
            else if(!DIsWall && direction != 'U') nextDirection = 'D';
            else if(!LIsWall && direction != 'R') nextDirection = 'L';
            else if(!RIsWall && direction != 'L') nextDirection = 'R';
            else nextDirection = 'U';

        }
    }

    //If the ghost does have to reverse it's direction (because it entered the scared
    //state) do turn around
    else if(reminder == 0 && reverse)
    {
        reverse = false;
        if(direction == 'U') nextDirection = 'D';
        else if(direction == 'D') nextDirection = 'U';
        else if(direction == 'L') nextDirection = 'R';
        else if(direction == 'R') nextDirection = 'L';

    }

    //Make sure the ghost doesn't get trapped in the ghost house
    else if(UIsWall && LIsWall && DIsWall) nextDirection = 'R';
    else if(UIsWall && RIsWall && DIsWall) nextDirection = 'L';

    //If the ghost gets to the teleporter, do the teleportation
    //The teleporters are always on the same tiles in every level
    //Their behavior is therefore hard-coded. Might want to change this...
    if(currentTileNumber == 509 && direction == 'L') setPos(600,280);
    else if(currentTileNumber == 536 && direction == 'R') setPos(100,280);

    //After considering all conditions and restrictions on the ghost's movement,
    //the ghost can set it's new direction and can call the doMove function to actually move
    direction = nextDirection;

    //If the ghost wants to move in a direction that is free (no walls)
    //and is not on a special tile or has already processed special tile,
    //do the actual moving. The actual movement has been delegated to
    //the private member slot doMove().
    movementTracker = 0;
    if(state == "scared") speed = 1;
    else speed = 2;
    connect(movementTimer, SIGNAL(timeout()), this, SLOT(doMove()));
    disconnect(movementTimer, SIGNAL(timeout()), this, SLOT(prepareMove()));
    return;
}
