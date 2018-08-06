#ifndef SCORE_H
#define SCORE_H

#include <QGraphicsTextItem>


class Score: public QGraphicsTextItem
{
public:
    //Constructors and destructors
    Score(QGraphicsItem * parent=0);
    ~Score();

    //Public member functions
    void increase(int howMuch);
    int getScore() const;
    void reset();

private:
    //Private variables
    int score;
};

#endif // SCORE_H
