#ifndef TILE_H
#define TILE_H

#include <QLabel>
#include <QDebug>

class Tile: public QLabel
{
public:

    //Fields
    int tileColor,piece,pieceColor,row,col,tileNum;
    char pieceName;

    //int playerColor;

    //Constructors
    Tile(QWidget* pParent=0, Qt::WindowFlags f=0) : QLabel(pParent, f) {
         this->setStyleSheet("QLabel {background-color: rgb(120, 120, 90);}:hover{background-color: rgb(170,85,127);}");
    }
    Tile(const QString& text, QWidget* pParent = 0, Qt::WindowFlags f = 0) : QLabel(text, pParent, f){
         this->setStyleSheet("QLabel {background-color: rgb(120, 120, 90);}:hover{background-color: rgb(170,85,127);}");
    }

    //Methods
    void mousePressEvent(QMouseEvent *event);
    void display(char elem);
    void tileDisplay();
};

#endif // TILE_H
