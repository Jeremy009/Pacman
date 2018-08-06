#ifndef MOVINGOBJECTS_H
#define MOVINGOBJECTS_H

#include <QKeyEvent>
#include <QTimer>
#include <QMediaPlayer>
#include "Score.h"


//Moving objects are quite similar. It makes sense to define an
//abstract base class and to let the different movingObject classes
//inherit from the base class.

class MovingObject: public QObject, public QGraphicsPixmapItem
{
    //Include the Q_OBJECT macro to make the slots function properly
    Q_OBJECT

protected:
    //Every moving object needs the following variables. Use protected
    //to make sure subclasses have access to said variables
    int speed;
    int movementTracker;
    int animationState;
    char direction;
    char nextDirection;
    QTimer *movementTimer;
    QTimer *animationTimer;
    QMediaPlayer *sound;
    std::string state;
        //State depends on subclass type.
        //For player: dead or alive
        //For ghost: scatter, chase, scaredEnding, dead

public slots:
    //Every moving object needs public functions to initiate it's movement, to
    //stop it's movement or to reset it. Furthermore the function needs to be able
    //to respond to signals.
    void startMoving();
    void stopMoving();
    virtual void reset() = 0;

protected slots:
    //The following private slots enable the logic
    //behind the moving. Some can or must be overloaded or extended
    virtual void doMove();
    virtual void animate() = 0;
    virtual void prepareMove() = 0;
};




class Player: public MovingObject
{
    //Include the Q_OBJECT macro to make the slots function properly
    Q_OBJECT

public:
    //Constructor and destructor
    Player();
    ~Player();

    //Public member functions
    void keyPressEvent(QKeyEvent *event);
    void resetNumberOfKills();
    void increaseNumberOfKills();
    int numberOfGhostsKilled() const;

private:
    int ghostsKilled;
    int numberOfPegsEaten;

public slots:
    void reset();
    void dead();

private slots:
    void ressurect();
    void animate();
    void prepareMove();
};




class Ghost: public MovingObject
{
    //Include the Q_OBJECT macro to make the slots function properly
    Q_OBJECT

public:
    //Constructor and destructor
    Ghost(std::string t = "pink");
    ~Ghost();

    //Public member functions
    void isScared();
    void setScatterTime(int x){scatterTime=x;};
    void setChaseTime(int x){chaseTime=x;};

private:
    //Private variables
    bool reverse;
    int scatterTime; //in ms
    int chaseTime; //in ms
    std::string type;
    QTimer *stateTimer;

    //Private member functions
    void dead();

public slots:
    void reset();
    void reset(int when);

private slots:
    void scatter();
    void chase();
    void isNotScared();
    void scaredEnding();
    void doMove(); //This is an extentions of the MovingObject doMove function
    void animate();
    void prepareMove();
};

#endif // MOVINGOBJECTS_H
