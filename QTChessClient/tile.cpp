#include "tile.h"
#include "validation.h"

#include <thread>

#include <chrono>

validation *valid = new validation();

extern int count,turn;
extern QWidget *myWidget;
extern Tile *click1;
extern Tile *tile[8][8];
extern int player_color;
int tileNum, tileNum_click, pieceColor;
char pieceName;
bool send_to_server;

int explore[60];

void validate(Tile *temp,int c);
void disOrange();

void Tile::mousePressEvent(QMouseEvent *event)
{
    validate(this,++count);
}

void Tile::display(char elem)
{
    this->pieceName=elem;

    if(this->pieceColor && this->piece)
    {
        switch(elem)
        {
            case 'P': this->setPixmap(QPixmap(":/Images/pawn_white.svg"));
                      break;
            case 'R': this->setPixmap(QPixmap(":/Images/rook_white.svg"));
                      break;
            case 'H': this->setPixmap(QPixmap(":/Images/knight_white.svg"));
                      break;
            case 'K': this->setPixmap(QPixmap(":/Images/king_white.svg"));
                      break;
            case 'Q': this->setPixmap(QPixmap(":/Images/queen_white.svg"));
                      break;
            case 'B': this->setPixmap(QPixmap(":/Images/bishop_white.svg"));
                      break;
        }
    }

    else if(this->piece)
    {
        switch(elem)
        {
        case 'P': this->setPixmap(QPixmap(":/Images/pawn_black.svg"));
                  break;
        case 'R': this->setPixmap(QPixmap(":/Images/rook_black.svg"));
                  break;
        case 'H': this->setPixmap(QPixmap(":/Images/knight_black.svg"));
                  break;
        case 'K': this->setPixmap(QPixmap(":/Images/king_black.svg"));
                  break;
        case 'Q': this->setPixmap(QPixmap(":/Images/queen_black.svg"));
                  break;
        case 'B': this->setPixmap(QPixmap(":/Images/bishop_black.svg"));
                  break;
        }
    }
    else
        this->clear();
}

void validate(Tile *temp, int c)
{
    int retValue,i;

    if(c==1)
    {
        if(temp->piece && (temp->pieceColor==turn) && player_color == turn)
        {
            //explore[max++]=temp->tileNum;
            retValue=valid->chooser(temp);

            if(retValue)
            {
                click1= new Tile();
                temp->setStyleSheet("QLabel {background-color: green;}");
                click1=temp;
            }
            else
            {
                temp->setStyleSheet("QLabel {background-color: red;}");
                count=0;
            }
        }
        else
        {
            count=0;
        }
    }

    else
    {

        if(temp->tileNum==click1->tileNum)
        {
            click1->tileDisplay();
            disOrange();
            max=0;
            count=0;
        }

        for(i=0;i<max;i++)
        {
            if(temp->tileNum==explore[i])
            {
                click1->piece=0;
                temp->piece=1;

                temp->pieceColor=click1->pieceColor;
                temp->pieceName=click1->pieceName;

                click1->display(click1->pieceName);
                temp->display(click1->pieceName);

                click1->tileDisplay();
                temp->tileDisplay();

                retValue=valid->check(click1);

                disOrange();

                max=0;

                turn=(turn+1)%2;
                count=0;

                tileNum = temp->tileNum;
                tileNum_click = click1->tileNum;
                pieceColor = temp->pieceColor;
                pieceName = temp->pieceName;

                send_to_server = true;
            }

            else
                count=1;
        }
    }
}

void Tile::tileDisplay()
{
    if(this && this->tileColor)
        try{
            this->setStyleSheet("QLabel {background-color: rgb(120, 120, 90);}:hover{background-color: rgb(170,85,127);}");
    }catch(...){}
        //this->setStyleSheet("background-color:DimGray; hover{background-color: IndianRed}");
    else{
    this->setStyleSheet("QLabel {background-color: rgb(211, 211, 158);}:hover{background-color: rgb(170,95,127);}");
    }
       // this->setStyleSheet("QLabel {background-color:Gainsboro}; hover{background-color: IndianRed}");
       // this->setStyleSheet("QLabel {background-color: rgb(211, 211, 158);}:hover{background-color: rgb(170,95,127);}");
}

void disOrange()
{
    int i;

    for(i=0;i<max;i++)
        tile[explore[i]/8][explore[i]%8]->tileDisplay();

}

void fill_data() {
    for(int i = 0;i < 8; ++i) {
        for(int j=0;j<8;j++) {
            Q_ASSERT(tile[i] && tile[j] && tile[i][j]);
            if (tile[i][j]->tileNum == tileNum) {
                tile[i][j]->piece=1;

                tile[i][j]->pieceColor=pieceColor;
                tile[i][j]->pieceName=pieceName;
                try {
                 tile[i][j]->display(pieceName);
                 if(tile[i][j]){
//                     tile[i][j]->tileDisplay();
                 }
                 } catch (...) {
                     qDebug() <<"Exception";

                 }

                std::this_thread::sleep_for(std::chrono::milliseconds((100)));
                disOrange();
            } else if (tile[i][j]->tileNum == tileNum_click) {
             tile[i][j]->piece=0;
              tile[i][j]->display(pieceName);

//             tile[i][j]->tileDisplay();
             disOrange();
            }
          }
        }

       turn=(turn+1)%2;

}
