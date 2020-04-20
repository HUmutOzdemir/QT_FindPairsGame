#ifndef FINDPAIRS_H
#define FINDPAIRS_H

#include <QDialog>
#include <QButtonGroup>
#include <QLabel>
#include <QLCDNumber>
#include <QGridLayout>
#include <QPushButton>

class FindPairs : public QDialog{

    Q_OBJECT

public:
    FindPairs(QWidget *parent=nullptr);
    void randomizeCards();
    void delay(int milisec);
    void show();
private slots:
    void resetClick();
    void cardClick(int cardNum);
private:
    // States of program enum.
    enum State{
        FirstClick,
        SecondClick
    };
    QWidget window;
    //Button group that stores all card buttons.
    QButtonGroup buttonGroup;
    //Card button array.
    QPushButton  *button[24];
    //Pointer of Reset Button.
    QPushButton  *reset;
    //Label only contains "Pairs:" string.
    QLabel *labelPair;
    //Label only contains "Tries:" string.
    QLabel *labelTries;
    //2 LCD number that displays the number of tries and number of pairs founded.
    QLCDNumber numPair;
    QLCDNumber numTries;
    //A string array stores the values of cards.
    std::string cards[4][6];
    // Stores the current state of program. States are FirstClick and SecondClick.
    State currentState;
    // Index of the previous clicked button.
    int prevButton;
};

#endif // FINDPAIRS_H
