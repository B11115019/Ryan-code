
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <qrightclickbutton.h>
#include <QMainWindow>
#include <QTimer>
#include"Gameboard.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void switchPage();
private slots:
    void printGameState();
    void printGameAnswer();
    void printGameBoard();
    void load1Button();
    void load2Button();
    void load3Button();
    void startGame();
    void setBombAmount();
    void setFlagAmount();
    void setOpenBlankAmount();
    void setRemainBlankAmount();
    void leftClicked();
    void rightClicked();
    void middleClicked();
    void replayMode();
    void quitMode();
    void volumeAdjust();
    void changeColor();
private:
    Ui::MainWindow *ui;
    GameBoard game;
    QList<QRightClickButton*> button;
    void makeNewBoard();
    void expand(QRightClickButton *button, vector<vector<char>> &playingBoard);
    void getRowCol(int &row, int &col, QRightClickButton *button);
    void initialize();
    int mainHeight, mainWidth;
    int count = 0;
    void bombAppear(vector<vector<char>> countBoard);
    QPalette orginColor;

    QString color[18] = {"black", "white", "red", "green", "blue", "cyan", "magenta",
                       "yellow", "darkRed", "darkGreen", "darkBlue", "darkCyan", "darkMagenta",
                       "darkYellow", "gray", "darkGray", "lightGray", "purple"};

    QTimer *timer = new QTimer(this);
    int colorIndex=0;
};

#endif // MAINWINDOW_H
