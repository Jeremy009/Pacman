#include "Game.h"
#include "QInputDialog"
#include <math.h>
#include <qdebug.h>
#include <QDebug>



Game::Game()
{
    //Start the game by creating the scene in which everything will take place
    //The pacman clone has a FIXED size so it is allowed to hard code the
    //dimensions of the scene

    //Experiment with variable size
    //int width = ;
    //int height = ;
    //int numberOfTiles = width
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,720,640);

    //Add a view so that we may see what we're doing
    view = new QGraphicsView(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(720,640);
    view->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
    view->show();


    //Make a new game button
    newGameButton = new QPushButton();
    newGameButton->setFlat(true);
    newGameButton->setText("Play");
    newGameButton->setFont(QFont("papyrus",30,750));
    newGameButton->setGeometry(QRect(QPoint(250, 300),QSize(200, 50)));

    //Format the button
    QPalette pal = newGameButton->palette();
    pal.setColor(QPalette::Button, QColor(Qt::black));
    pal.setColor(QPalette::ButtonText, QColor(Qt::white));
    newGameButton->setAutoFillBackground(true);
    newGameButton->setPalette(pal);
    newGameButton->update();

    //Add and connect the button
    scene->addWidget(newGameButton);
    connect(newGameButton,SIGNAL(clicked()),this,SLOT(newGame()));


    //Make a quit button
    quitButton = new QPushButton();
    quitButton->setVisible(false);
    quitButton->setFlat(true);
    quitButton->setText("Quit");
    quitButton->setFont(QFont("papyrus",30,750));
    quitButton->setGeometry(QRect(QPoint(400, 400),QSize(200, 50)));

    //Format the button
    quitButton->setAutoFillBackground(true);
    quitButton->setPalette(pal);
    quitButton->update();

    //Add and connect the button
    scene->addWidget(quitButton);
    connect(quitButton,SIGNAL(clicked()),this,SLOT(quit()));


    //Set some pointer values to NULL to begin. We do this so that when starting a new game we do not accidently delete
    //some random objects in memory
    message= NULL;
    score  = NULL;
    inputName = NULL;
    for(auto T : scoresToDisplay) T = NULL;
}

Game::~Game()
{
    //The game destructor cleans up everything before closing down
    delete player;
    delete level;
    delete score;
    delete health;
    delete view;
    delete sound;
    delete message;
    delete newGameButton;
    for(auto T : scoresToDisplay) delete T;
    for(auto T : enemies) delete T;
    scene->clear();
    delete scene;
}

void Game::newGame()
{
    //Clean up all remaining object from previous games
    if(message != NULL) delete message;
    if(score != NULL) delete score;
    for(auto T : scoresToDisplay) if(T!=NULL) delete T;

    //Remove the play button
    newGameButton->setVisible(false);
    quitButton->setVisible(false);
    view->clearFocus();

    //Set the first level
    currentLevel = 1;

    //Make a new level
    level = new Level();
    scene->addItem(level);
    level->build(currentLevel);

    //Make the (4) enemy ghosts that will interfere with the player
    enemies.push_back(new Ghost("pink"));
    enemies.push_back(new Ghost("blue"));
    enemies.push_back(new Ghost("yellow"));
    enemies.push_back(new Ghost("red"));
    for(unsigned long i=0; i<enemies.size(); i++)
    {
        scene->addItem(enemies[i]);
        enemies[i]->setPos(320+20*i,280);
    }

    //Make a player object that can move over the board
    player = new Player();
    scene->addItem(player);
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
    player->setPos(340,440);

    //Add the score object
    score = new Score;
    score->setPos(90,0);
    scene->addItem(score);

    //Add the health object
    health = new Health;
    health->setPos(535,0);
    scene->addItem(health);

    //Initialise the message object
    message = new QGraphicsTextItem;

    //Begin the game with the opening tune and message
    sound = new QMediaPlayer();
    sound->setMedia(QUrl("qrc:Sounds/pacman_beginning.wav"));
    sound->play();
    setMessage("Get ready!",289,306);
    QTimer::singleShot(4400,this,SLOT(clearMessage()));

    //Once the opening tune is finished, we connect the signal emitted by the opening
    //tune to the slots that make the player and ghosts move
    connect(sound,SIGNAL(stateChanged(QMediaPlayer::State)),player,SLOT(startMoving()));
    for(auto T : enemies)
    {
        connect(sound,SIGNAL(stateChanged(QMediaPlayer::State)),T,SLOT(startMoving()));
    }
}

void Game::playerDied()
{
    //The playerDied() member function asks every object that has to
    //be updated when the player dies, to do the needed changes. It also
    //checks whether the game is not over yet. The game is over when the player
    //has no lifes left
    health->decrease();
    if(health->getHealth() >= 0)
    {
        player->dead();
        for(auto T : enemies) T->reset();
        QTimer::singleShot(1800, player, SLOT(ressurect()));
    }
    else
    {
        player->dead();
        for(auto T : enemies) T->stopMoving();
        QTimer::singleShot(1800,this,SLOT(gameOver()));
    }
}

void Game::levelCleared()
{
    //What should we do here:
    //-reset the player
    //-reset the ghost
    //-reset the level
    //-reset the timers
    //-change scatter and chase times
    //-continue

    //Make all the moving objects stop
    player->stopMoving();
    for(auto T : enemies) T->stopMoving();

    //Set a new level board
    currentLevel++;
    scene->removeItem(level);
    delete level;
    level = new Level();
    scene->addItem(level);
    level->build(currentLevel);

    //Set all the moving objects back to where they should start
    for(auto T : enemies) T->reset(1650);
    player->reset();

    //Change difficulty
    for(auto T : enemies)
    {
        T->setScatterTime(3000-currentLevel*250);
        T->setChaseTime(5000+currentLevel*250);
    }

    //Set a new level message
    //Begin the game with the opening tune and message
    std::string levelText = "Level " + std::to_string(currentLevel) + "!";
    disconnect(sound,0,0,0);
    sound->setMedia(QUrl("qrc:Sounds/pacman_intermission.wav"));
    sound->play();
    setMessage(levelText,308,306);
    QTimer::singleShot(2200,this,SLOT(clearMessage()));

    //Once the opening tune is finished, we connect the signal emitted by the opening
    //tune to the slots that make the player and ghosts move
    connect(sound,SIGNAL(stateChanged(QMediaPlayer::State)),player,SLOT(startMoving()));
}

void Game::gameOver()
{
    //Clear the scene
    scene->removeItem(level);
    for(auto T : enemies) scene->removeItem(T);
    scene->removeItem(player);
    scene->removeItem(health);

    delete level;
    for(auto T : enemies) delete T;
    for(auto T : enemies) enemies.pop_back();
    delete player;
    delete health;

    //Display the score
    score->setPos(310,300);
    setMessage("Game Over", 280,265);

    //Show the buttons
    quitButton->setVisible(true);
    quitButton->setGeometry(QRect(QPoint(400, 450),QSize(200, 50)));
    newGameButton->setVisible(true);
    newGameButton->setText("New game");
    newGameButton->setGeometry(QRect(QPoint(100, 450),QSize(200, 50)));

    //Deal with the scores
    manageScore();
}

void Game::setMessage(std::string whatToDisplay, int x, int y)
{
    //The setMessage member function allows to display a message on the scene.
    message->setPlainText(QString::fromStdString(whatToDisplay));
    message->setDefaultTextColor(Qt::white);
    message->setFont(QFont("papyrus",30,750));
    message->setPos(x,y);
    scene->addItem(message);
}

void Game::clearMessage()
{
    //Removes a message from the scene
    scene->removeItem(message);
}

void Game::manageScore()
{
    //Get rid of old score items
    while (!scoresToDisplay.empty()) scoresToDisplay.pop_back();

    //Set vectors to contain the info
    std::vector<QString> names;
    std::vector<int> scores;

    //Open the highscores file
     QFile file("highscores.csv");
     if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
     {
         qDebug()<<"Error: Could not open file";
     }

     //If opened: read the file
     else
     {
         //Initialise a counter to keep track of the number of loops
         int counter = 0;

         //Make a loop that reads each line and stores them in a vector
         while (!file.atEnd())
         {
             //Read each line
             QByteArray line = file.readLine();

             //Some variables needed to operate the loop
             QString name = "";
             QString scoreString = "";
             bool secondPart = false;

             //Examine each char in the line
             for(char& c : line)
             {
                 if(c == ';') secondPart = true;
                 if(c != ';' && !secondPart) name+=c;
                 if(c != ';' && secondPart) scoreString+=c;
             }

             //Append the names and scores to the vectors holding that info
             names.push_back(name);
             scores.push_back(scoreString.toInt());

             //Display the info
             QGraphicsTextItem *nameTextItem;
             nameTextItem = new QGraphicsTextItem();
             nameTextItem->setPlainText(name);
             QGraphicsTextItem *scoreTextItem;
             scoreTextItem = new QGraphicsTextItem;
             scoreTextItem->setPlainText(scoreString);
             nameTextItem->setDefaultTextColor(Qt::white);
             nameTextItem->setFont(QFont("papyrus",30,750));
             scoreTextItem->setDefaultTextColor(Qt::white);
             scoreTextItem->setFont(QFont("papyrus",30,750));
             nameTextItem->setPos(250,40+40*counter);
             scoreTextItem->setPos(380,40+40*counter);
             scene->addItem(nameTextItem);
             scene->addItem(scoreTextItem);
             scoresToDisplay.push_back(nameTextItem);
             scoresToDisplay.push_back(scoreTextItem);

             counter++;
         }

         if(counter < 4)
         {
             QTextStream outStream(&file);
             outStream << "xxxxx;000\nxxxxx;000\nxxxxx;000\nxxxxx;000\nxxxxx;000";
         }

         //Check if there is a higshscore
         bool highscoreBeaten = false;
         counter = 0;
         int firstItemToShift = 6;

         for(auto T : scores)
         {
             if(score->getScore() > T)
             {
                 highscoreBeaten = true;
                 firstItemToShift = counter;
                 break;
             }
             else counter++;
         }

         //If there is a highscore, display a lineedit field to catch the player's name
         if(highscoreBeaten)
         {
             //Remove the last highscore score and name which are no longer worthy of being shown
             delete scoresToDisplay.back();
             scoresToDisplay.pop_back();
             delete scoresToDisplay.back();
             scoresToDisplay.pop_back();

             //Shift the needed scores on the display
             int i = 0;
             for(auto T : scoresToDisplay)
             {
                 if(i >= firstItemToShift*2) T->setPos(T->x(),T->y()+40);
                 i++;
             }

             //Make a editable line edit field
             inputName = new QLineEdit;
             inputName->setPlaceholderText("Name");
             inputName->move(250,40+40*firstItemToShift);
             inputName->setMaxLength(8);
             inputName->setMaximumWidth(50);
             inputName->setFixedWidth(100);
             inputName->setFont(QFont("papyrus",30,750));
             inputName->setFrame(false);

             QPalette *palette = new QPalette();
             palette->setColor(QPalette::Text,Qt::white);
             palette->setColor(QPalette::Base,Qt::black);
             inputName->setPalette(*palette);

             QGraphicsProxyWidget* pProxyWidget = scene->addWidget(inputName);

             //Display the player's score
             QGraphicsTextItem *scoreTextItem;
             scoreTextItem = new QGraphicsTextItem;
             scoreTextItem->setPlainText(QString::number(score->getScore()));
             scoreTextItem->setDefaultTextColor(Qt::white);
             scoreTextItem->setFont(QFont("papyrus",30,750));
             scoreTextItem->setPos(380,inputName->y());
             scene->addItem(scoreTextItem);
             scoresToDisplay.push_back(scoreTextItem);
             scoresToDisplay.push_back(scoreTextItem);

             connect(inputName,SIGNAL(returnPressed()),this,SLOT(saveScore()));
        }
     file.close();
     newGameButton->disconnect();
     connect(newGameButton,SIGNAL(clicked()),this,SLOT(newGame()));
    }
}

void Game::saveScore()
{
    inputName->disconnect();
    inputName->hide();

    QGraphicsTextItem *name;
    name = new QGraphicsTextItem;
    name->setPlainText(inputName->text());
    name->setDefaultTextColor(Qt::white);
    name->setFont(QFont("papyrus",30,750));
    name->setPos(inputName->x(),inputName->y());
    scene->addItem(name);
    scoresToDisplay[scoresToDisplay.size()-2] = name;

    std::vector<int> scores;
    std::vector<QString> names;

    int counter = 0;
    for(auto T : scoresToDisplay)
    {
        if(counter%2 == 0) names.push_back(T->toPlainText());
        else scores.push_back((T->toPlainText()).toInt());
        counter++;
    }

    for(unsigned long i = 0; i<scores.size()-1; i++)
    {
        for(unsigned long j= 0; j<scores.size()-1; j++)
        {
            if(scores[j]<=scores[j+1])
            {
                int temp = scores[j];
                scores[j] = scores[j+1];
                scores[j+1] = temp;

                QString tempString = names[j];
                names[j] = names[j+1];
                names[j+1] = tempString;
            }
        }
    }


    //Open the highscores file
     QFile file("/Users/Jeremy/Desktop/highscores.csv");
     if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
     {
         qDebug()<<"Error: Could not open file";
     }

     //If opened: write to the file
     else
     {
        file.resize(0);
        QTextStream outStream(&file);
        QString temp = "";
        for(unsigned long m=0; m<scores.size(); m++)
        {
            temp+=names[m]+';'+QString::number(scores[m])+'\n';
            outStream << temp;
            temp="";
        }
     }
     file.close();
}

void Game::quit()
{
    exit(0);
}


