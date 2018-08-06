#include "Score.h"
#include <QFont>


Score::Score(QGraphicsItem *parent): QGraphicsTextItem(parent)
{
    //Initialize the score to 0
    score = 0;

    //Draw the text
    setPlainText(QString("Score: ") + QString::number(score));
    setDefaultTextColor(Qt::white);
    setFont(QFont("papyrus",18,750));
}

Score::~Score()
{
    /*Does nothing*/
}

void Score::increase(int howMuch)
{
    score += howMuch;
    setPlainText(QString("Score: ") + QString::number(score));
}

int Score::getScore() const
{
    return score;
}

void Score::reset()
{
    //Initialize the score to 0
    score = 0;

    //Draw the text
    setPlainText(QString("Score: ") + QString::number(score));
    setDefaultTextColor(Qt::white);
    setFont(QFont("papyrus",18,750));
}
