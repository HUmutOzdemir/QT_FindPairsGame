#include "findpairs.h"
#include <QtGui>
#include <unordered_set>
#include <QString>
#include <ctime>

//Takes a int milisec as parameter and gives a delay that many miliseconds to program.
//When delay time all signals of app are blocked.
void FindPairs::delay(int milisec){
    buttonGroup.blockSignals(true);
    reset->blockSignals(true);
    QTime finishTime= QTime::currentTime().addMSecs(milisec);
    while (QTime::currentTime() < finishTime){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    buttonGroup.blockSignals(false);
    reset->blockSignals(false);
}
//Generates 12 random pairs and locates random positions in cards array.
void FindPairs::randomizeCards(){
    std::unordered_set<char> picked;
    bool isFull [4][6];
    memset(isFull,false,sizeof(isFull));
    std::srand(time(nullptr));
    while(picked.size()<12){
        char letter = std::rand() %26 + 65;
        if(picked.find(letter) != picked.end() || letter=='X')
            continue;
        picked.insert(letter);
        int addedPoint=0;
        while(addedPoint<2){
            int x = std::rand() %4;
            int y = std::rand() %6;
            if(!isFull[x][y]){
                cards[x][y] = letter;
                isFull[x][y] = true;
                addedPoint++;
            }
        }
    }

}
//SLOT of reset button. When reset button is pressed then 2 LCD display reseted to 0
//and randomize the cards. At the end shows the letters at cards 1 seconds, after that
//turns them to X and enables them.
void FindPairs::resetClick(){
    randomizeCards();
    for(int i=0; i<24; i++){
        button[i]->setText(QString::fromStdString(cards[i/6][i%6]));
        button[i]->setEnabled(false);
        button[i]->setFlat(false);
    }
    numPair.display(0);
    numTries.display(0);
    currentState=FirstClick;
    prevButton=-1;
    delay(1000);
    for(int i=0; i<24; i++){
        button[i]->setText("X");
        button[i]->setEnabled(true);
    }

}
//Overloaded show function. After showing the the app emit a clicked signal of reset button.
void FindPairs::show(){
   QDialog::show();
   emit reset->clicked();
}
//Main function of the program. It is a slot of button group.
//Function runs with respect to the current state of program.
//If it is the Fist Click it displays the string of clicked button and waits for another click.
//If it is Second Click it contols the 2 cards match. If match then it hides 2 button after 0.5 seconds delay.
void FindPairs::cardClick(int cardNum){
    int i = cardNum/6;
    int j = cardNum%6;
    if(currentState==FirstClick){
        button[cardNum]->setText(QString::fromStdString(cards[i][j]));
        button[cardNum]->setEnabled(false);
        prevButton = cardNum;
        currentState=SecondClick;
    }else{
        button[cardNum]->setText(QString::fromStdString(cards[i][j]));
        button[cardNum]->setEnabled(false);
        numTries.display(numTries.intValue()+1);
        currentState=FirstClick;
        delay(500);
        int prev_i = prevButton/6;
        int prev_j = prevButton%6;
        if(cards[i][j]==cards[prev_i][prev_j]){
            numPair.display(numPair.intValue()+1);
            button[cardNum]->setFlat(true);
            button[cardNum]->setText("");
            button[prevButton]->setFlat(true);
            button[prevButton]->setText("");
            prevButton=-1;
        }else{
            button[cardNum]->setText("X");
            button[cardNum]->setEnabled(true);
            button[prevButton]->setText("X");
            button[prevButton]->setEnabled(true);
            prevButton=-1;
        }
        if(numPair.intValue()==12){
            delay(500);
            emit reset->clicked();
        }
    }
}
//Constructor pf the application.
FindPairs::FindPairs(QWidget *parent):QDialog(parent){

    currentState = State::FirstClick;
    prevButton = -1;

    //Layout of Pair and its LCD.
    QHBoxLayout *pairLayout = new QHBoxLayout;
    labelPair =  new QLabel("Pairs");
    numPair.setSegmentStyle(QLCDNumber::Flat);
    pairLayout->addWidget(labelPair);
    pairLayout->addWidget(&numPair);

    //Layout of Tries and its LCD.
    QHBoxLayout *triesLayout = new QHBoxLayout;
    labelTries = new QLabel("Tries");
    numTries.setSegmentStyle(QLCDNumber::Flat);
    triesLayout->addWidget(labelTries);
    triesLayout->addWidget(&numTries);

    reset = new QPushButton("Reset");
    QObject::connect(reset,SIGNAL(clicked()),this,SLOT(resetClick()));

    //Combines ' layout and the reset button.
    QHBoxLayout *upperLayout = new QHBoxLayout;
    upperLayout->addLayout(pairLayout);
    upperLayout->addLayout(triesLayout);
    upperLayout->addWidget(reset);
    //Adds all card button to Grid Layout.
    QGridLayout *grid = new QGridLayout();
    for(int i=0; i<4; i++){
        for(int j=0; j<6; j++){
            button[6*i+j] = new QPushButton("X");
            button[6*i+j]->setEnabled(true);
            grid->addWidget(button[6*i+j],i,j);
            buttonGroup.addButton(button[6*i+j],6*i+j);
        }
    }

    QObject::connect(&buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(cardClick(int)));
    //Combines biggest 2 layouts in one main layout.
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(upperLayout);
    mainLayout->addLayout(grid);

    setLayout(mainLayout);

    setWindowTitle("Find Pairs!!");
    setFixedHeight(sizeHint().height());

}
