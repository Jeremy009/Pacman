#include "Health.h"
#include <QFont>


Health::Health(QGraphicsItem *parent): QGraphicsTextItem(parent)
{
    //Initialize the health to 3
    health = 3;

    //Draw the text
    setPlainText(QString("Lives: ● ● ●"));
    setDefaultTextColor(Qt::white);
    setFont(QFont("papyrus",18,750));
}

Health::~Health()
{
    /*does nothing*/
}

void Health::decrease()
{
    //Decrease the health
    health--;

    //Update the text
    if(health == 2) setPlainText(QString("Lives: ● ●"));
    else if(health == 1) setPlainText(QString("Lives: ●"));
    else if(health == 0) setPlainText(QString("Lives:"));
    //else /*game over*/
}

int Health::getHealth() const
{
    return health;
}

void Health::reset()
{
    //Decrease the health
    health = 3;

    //Draw the text
    setPlainText(QString("Lives: ● ● ●"));
    setDefaultTextColor(Qt::white);
    setFont(QFont("papyrus",18,750));
}

