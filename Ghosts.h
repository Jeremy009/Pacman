#ifndef GHOSTS_H
#define GHOSTS_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QObject>
#include <string>


class Ghosts: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Ghosts(std::string c = "pink");
private:
    int movementTracker;
    int animationState;
    std::string type;
    char direction;
    char nextDirection;
    QTimer *movementTimer;
public slots:
    void startMoving();
private slots:
    void animate();
    void doMove();
    void prepareMove();

};

#endif // GHOSTS_H
