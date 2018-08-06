#include "Level.h"
#include "Tile.h"
#include <QFile>


Level::Level() : numberOfPegs(0){}

Level::~Level()
{
    //Does nothing...

    //Remark that the Tile class has the option to set a parent widget.
    //When a widget (Tile) has a parent (Level), the child gets deleted
    //WHENEVER THE PARENT IS DELETED!
    //Since in this implementation, Level is the Parent of Tile, it is
    //not needed to delete manually every Tile!
}

void Level::build(int currentLevel)
{
    //Make the char vector that holds the tile types
    int counter = 0;
    std::vector<char> types;

    //Open the correct level file according to the current level. The
    //first real level is 1. The 'lower' ones are test levels.
    QFile file;
    if(currentLevel == 0) file.setFileName(":/Levels/level0.csv");
    else if(currentLevel == 1) file.setFileName(":/Levels/level1.csv");
    else if(currentLevel == 2) file.setFileName(":/Levels/level2.csv");
    else file.setFileName(":/Levels/level1.csv");

    //Check that the file has been found and opened successfully in read mode.
    //If not opened: fill the level with empty tiles.
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        for(int i=0; i<1152; i++) types.push_back('e');
    }

    //If opened: read the file and make the types-vector. Also count
    //the number of pegs in this level
    else
    {
        while (!file.atEnd())
        {
            QByteArray line = file.readLine();
            for(char& c : line)
            {
                if(c!=';' && c!='n' && c!='\n') types.push_back(c);
                if(c == '.' || c == '*') numberOfPegs++;
            }
        }
    }

    //Read what tile has to go where
    for(int y=0; y<32; y++)
    {
        for(int x=0; x<36; x++)
        {
            //Make a new tile object
            Tile *tile = new Tile(types[counter],x*20,y*20,this);
            tileAdresses[counter+1] = tile;

            //Update the counter
            counter++;
        }
    }
}

int Level::getNumberOfPegs() const
{
    return numberOfPegs;
}
