#ifndef GAME_H
#define GAME_H

#include "MovingObjects.h"
#include "Level.h"
#include "Health.h"
#include "Score.h"
#include <QGraphicsView>
#include <QMediaPlayer>
#include <QPushButton>
#include "QInputDialog"


class Game: public QGraphicsView
{
    //Include the Q_OBJECT macro to make the slots function properly
    Q_OBJECT

public:
    //Constructors and destructors. The default constructor can be called
    //and will delegate to the level-based constructor with a value of 1
    Game();
    ~Game();

    //Variables needed to run the game class
    Player *player;
    Level *level;
    Score *score;
    Health *health;
    std::vector<Ghost*> enemies;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QMediaPlayer *sound;

    //Public member functions that allow the game to run
    void playerDied();
    void gameWon();

private:
    int currentLevel;
    QGraphicsTextItem *message;
    QPushButton *newGameButton;
    QPushButton *quitButton;
    std::vector<QGraphicsTextItem*> scoresToDisplay;
    QLineEdit *inputName;

public slots:
    //Public slots that enable the game to recieve signalsw
    void newGame();
    void levelCleared();
    void gameOver();
    void setMessage(std::string whatToDisplay, int x, int y);
    void clearMessage();
    void manageScore();
    void saveScore();
    void quit();
};

#endif // GAME_H
