
#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include<iostream>
#include<string>
#include<vector>

using namespace std;

enum GameStatus
{
    standby,
    playing,
    gameover
};


class GameBoard
{
private:
    int row, col;
    int bomb, flagAmount, BlankAmount, remainAmount;
    vector<vector<char>> board, playingBoard, count;
    static int status;
    bool load;
public:
    GameBoard();
    void setGameBoard();
    void setGameBoard(double rate);
    void setGameBoard(string file);
    void RunCommandFile(string fileInput, string fileOutput);
    void GameSetCammand();
    void PrintGameAnswer();
    void CountMine(int x, int y, vector<vector<char>> &count);
    void PrintGameBoard();
    bool pressLeftclick(int posX, int posY);
    void pressRightclick(int posX, int posY);
    void expand(int posX, int posY);
    void initialize();

    static int getStatus();
    static void setStatus(int status);

    void setRowColBomb(int row, int col, int bomb);
    void setRowColBomb(int row, int col);

    bool getLoad();
    int getBomb();
    int getflagAmount();
    int getBlankAmount();
    int getremainAmount();
    int getRow();
    int getCol();
    //void getinitialize();

    vector<vector<char>> getCountBoard();
    vector<vector<char>> getPlayingBoard();
};

#endif // GAMEBOARD_H
