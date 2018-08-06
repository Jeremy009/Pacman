#ifndef HEALTH_H
#define HEALTH_H

#include <QGraphicsTextItem>


class Health: public QGraphicsTextItem
{
public:
    //Constructors and destructors.
    Health(QGraphicsItem * parent = 0);
    ~Health();

    //Public member functions
    void decrease();
    int getHealth() const;
    void reset();

private:
    //Private variables
    int health;
};

#endif // HEALTH_H
