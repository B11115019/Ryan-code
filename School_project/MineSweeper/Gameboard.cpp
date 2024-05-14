#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include "Gameboard.h"

using namespace std;

//static variable
int GameBoard::status = 0;

//constructor
GameBoard::GameBoard()
{
    row=0;
    col=0;
    bomb=0;
    flagAmount = 0;
    status = GameStatus::standby;
    load = 0;
    BlankAmount = 0;
    remainAmount = 0;
}

//Load RandomCount <M> <N> <炸彈數量>
void GameBoard::setGameBoard()
{
    int randRow=0,randCol=0;
    int bombAmount = bomb;
    board = vector<vector<char>>(row, vector<char>(col,'O'));
    playingBoard = vector<vector<char>>(row, vector<char>(col,'#'));
    count = vector<vector<char>>(row, vector<char>(col ,'0'));

    if(bomb >= row * col || bomb < 0)
    {
        cout << "<Load RandomCount> : Failed" << endl;
        return;
    }
    else
    {
        cout << "<Load RandomCount> : Success" << endl;
    }

    remainAmount = (row * col) - bomb;

    while(bombAmount--)
    {
        randRow = rand()%row;
        randCol = rand()%col;
        if(board[randRow][randCol] != 'X') board[randRow][randCol] = 'X';
        else bombAmount++;
    }

    for(int i = 0;i < row; i++)
    {
        for(int j = 0;j < col;j++)
        {
            if(board[i][j] =='O') count[i][j] = '0';
            else count[i][j] = 'X';
        }
    }

    for(int i = 0;i < row; i++)
        for(int j = 0;j < col;j++)
            if(count[i][j] == 'X')
                CountMine(i, j, count);

    load = 1;
}

//Load RandomRate <M> <N> <炸彈生成機率>
void GameBoard::setGameBoard(double rate)
{
    board = vector<vector<char>>(row, vector<char>(col,'O'));
    playingBoard = vector<vector<char>>(row, vector<char>(col,'#'));
    count = vector<vector<char>>(row, vector<char>(col ,'0'));

    if(row <= 0 || col <= 0 || rate < 0 || rate > 1)
    {
        cout << "<Load RandomRate> : Failed" << endl;
        return;
    }
    else
    {
        cout << "<Load RandomRate> : Success" << endl;
    }

    rate *= 100;

    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            int temp = rand()%100;
            if(temp < rate)
            {
                board[i][j] = 'X';
                bomb++;
            }
            else board[i][j] = 'O';
        }
    }

    remainAmount = (row * col) - bomb;

    for(int i = 0;i < row; i++)
    {
        for(int j = 0;j < col;j++)
        {
            if(board[i][j] =='O') count[i][j] = '0';
            else count[i][j] = 'X';
        }
    }

    for(int i = 0;i < row; i++)
        for(int j = 0;j < col;j++)
            if(count[i][j] == 'X')
                CountMine(i, j, count);

    load = 1;
}

void GameBoard::setGameBoard(string file)
{
    ifstream in;
    in.open(file);

    if (in.fail())
    {
        cout << "<Load BoardFile " << file << ">" << " : Failed" << endl;
        return;
    }

    in >> row >> col;

    board = vector<vector<char>>(row, vector<char>(col,'O'));
    playingBoard = vector<vector<char>>(row, vector<char>(col,'#'));
    count = vector<vector<char>>(row, vector<char>(col ,'0'));

    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            in >> board[i][j];

            if(board[i][j] == 'X')
                bomb++;
        }
    }

    remainAmount = (row * col) - bomb;

    for(int i = 0;i < row; i++)
    {
        for(int j = 0;j < col;j++)
        {
            if(board[i][j] =='O') count[i][j] = '0';
            else count[i][j] = 'X';
        }
    }

    for(int i = 0;i < row; i++)
        for(int j = 0;j < col;j++)
            if(count[i][j] == 'X')
                CountMine(i, j, count);

    cout << "<Load BoardFile " << file << ">" << " : Success" << endl;

    load = 1;
    in.close();
}

//Load BoardFile <盤面檔相對路徑>
void GameBoard::RunCommandFile(string fileInput, string fileOutput)
{
    ofstream output(fileOutput);
    ifstream input(fileInput);

    streambuf *originInput = cin.rdbuf(input.rdbuf());
    streambuf *originOutput = cout.rdbuf(output.rdbuf());

    GameSetCammand();

    cin.rdbuf(originInput);
    cout.rdbuf(originOutput);

    output.close();
    input.close();
}

//execute cammand
void GameBoard:: GameSetCammand()
{
    string command = "",type = "";

    while(cin >> command)
    {
        if(command == "Print" && load)
        {
            string execute = "";
            cin >> execute;

            if(execute == "GameBoard")
            {
                PrintGameBoard();
            }
            else if(execute == "GameAnswer")
            {
                PrintGameAnswer();
            }
            else if(execute == "GameState")
            {
                if(status == GameStatus::standby) cout << "<Print GameState> : Standby" << endl;
                else if(status == GameStatus::playing) cout << "<Print GameState> : Playing" << endl;
                else if(status == GameStatus::gameover) cout << "<Print GameState> : GameOver" << endl;
            }
            else if(execute == "BombCount")
            {
                cout << "<Print BombCount> : " << bomb << endl;
            }
            else if(execute == "FlagCount")
            {
                cout << "<Print FlagCount> : " << flagAmount << endl;
            }
            else if(execute == "OpenBlankCount")
            {
                //BlankAmount = BlankCount();
                cout << "<Print OpenBlankCount> : " << BlankAmount << endl;
            }
            else if(execute == "RemainBlankCount")
            {
                cout << "<Print RemainBlankCount> : " << remainAmount << endl;
            }
            else
            {
                cout << "<" << command << "> : Failed" << endl;
            }
            continue;
        }
        else if(command == "Print" && !load)
        {
            string execute = "";
            cin >> execute;
            cout << "<Print" << " " << execute << "> : Failed" << endl;
            continue;
        }

        if(status == GameStatus::standby)
        {
            if(command == "Load")
            {
                cin >> type;

                if(type == "RandomCount")
                {
                    cin >> row >> col >> bomb;

                    setGameBoard();
                }
                else if(type == "RandomRate")
                {
                    double rate = 0;
                    cin >> row >> col >> rate;
                    setGameBoard(rate);
                }
                else if(type == "BoardFile")
                {
                    string file = "";

                    cin >> file;
                    setGameBoard(file);
                }
            }
            else if(command == "StartGame")
            {
                if(!load)
                {
                    cout << "<StartGame> : Failed" << endl;
                }
                else
                {
                    cout << "<StartGame> : Success" << endl;
                    status = GameStatus::playing;
                }
            }
            else
            {
                string input="";
                getline(cin, input);
                command+=input;
                cout << "<" << command << "> : Failed" << endl;
            }
        }
        else if(status == GameStatus::playing)
        {
            int posX = 0, posY = 0;

            if(command == "LeftClick")
            {
                cin >> posX >> posY;
                if(!pressLeftclick(posX, posY))
                {
                    cout << "You lose the game" << endl;
                    status = GameStatus::gameover;
                }
                else if(!remainAmount)
                {
                    cout << "You win the game" << endl;
                    status = GameStatus::gameover;
                }
            }
            else if(command == "RightClick")
            {
                cin >> posX >> posY;
                pressRightclick(posX, posY);
            }
            else
            {
                string input="";
                getline(cin, input);
                command+=input;
                cout << "<" << command << "> : Failed" << endl;
            }
        }
        else if(status == GameStatus::gameover)
        {
            if(command == "Quit")
            {
                cout << "<" << command << "> : Success" << endl;
                return;
            }
            else if(command == "Replay")
            {
                cout << "<" << command << "> : Success" << endl;
                initialize();
            }
            else
            {
                string input="";
                getline(cin, input);
                command+=input;
                cout << "<" << command << "> : Failed" << endl;
            }
        }
    }
}

void GameBoard::PrintGameAnswer()
{
    cout << "<Print GameAnswer> : " << endl;

    for(int i = 0;i < row; i++)
    {
        for(int j = 0;j < col;j++)
        {
            cout << count[i][j] << " ";
        }
        cout << endl;
    }
}

//count how far and number and store to vector count
void GameBoard::CountMine(int x, int y, vector<vector<char>> &count)
{
    for(int i = -1;i < 2; i++)
        for(int j = -1;j < 2;j++)
            if((x + i) >= 0 && (y + j) >= 0 && (x + i) < row && (y + j) < col && count[x+i][y+j] != 'X')
                count[x+i][y+j] += 1;
}

void GameBoard::PrintGameBoard()
{
    cout << "<Print GameBoard> : " << endl;

    for(int i = 0;i < row; i++)
    {
        for(int j = 0;j < col;j++)
        {
            cout << playingBoard[i][j] << " ";
        }
        cout << endl;
    }
}

bool GameBoard::pressLeftclick(int posX, int posY)
{
    if(posX < 0 || posX >= row || posY < 0 || posY >= col || playingBoard[posX][posY] == 'f' || isdigit(playingBoard[posX][posY]))
    {
        cout << "<LeftClick " << posX << " " << posY << "> : Failed" << endl;
        return true;
    }

    cout << "<LeftClick " << posX << " " << posY << "> : Success" << endl;

    if(count[posX][posY] == 'X')
    {
        return false;
    }
    else if(count[posX][posY] == '0')
    {
        expand(posX, posY);
    }
    else
    {
        playingBoard[posX][posY] = count[posX][posY];
        BlankAmount++;
        remainAmount--;
    }

    return true;
}

void GameBoard::pressRightclick(int posX, int posY)
{
    if(posX < 0 || posX >= row || posY < 0 || posY >= col || isdigit(playingBoard[posX][posY]))
    {
        cout << "<RightClick " << posX << " " << posY << "> : Failed" << endl;
        return;
    }

    cout << "<RightClick " << posX << " " << posY << "> : Success" << endl;

    switch(playingBoard[posX][posY])
    {
    case'#':
        playingBoard[posX][posY] = 'f';
        flagAmount++;
        break;
    case'f':
        playingBoard[posX][posY] = '?';
        flagAmount--;
        break;
    case'?':
        playingBoard[posX][posY] = '#';
        break;
    default:
        break;
    }
}

void GameBoard::expand(int posX, int posY)
{
    if(posX < 0 || posX >= row || posY < 0 || posY >= col || isdigit(playingBoard[posX][posY]) || playingBoard[posX][posY] == 'f')
    {
        return;
    }

    playingBoard[posX][posY] = count[posX][posY];
    BlankAmount++;
    remainAmount--;

    for(int i = -1;i < 2; i++)
    {
        for(int j = -1;j < 2;j++)
        {
            if((posX + i) >= 0 && (posY + j) >= 0 && (posX + i) < row && (posY + j) < col)
            {
                if(i != 0 || j != 0)
                {
                    if(count[posX + i][posY + j] == '0')
                    {
                        expand(posX + i, posY + j);
                    }
                    else
                    {
                        if(playingBoard[posX + i][posY + j] == '#')
                        {
                            playingBoard[posX + i][posY + j] = count[posX + i][posY + j];
                            BlankAmount++;
                            remainAmount--;
                        }
                    }
                }
            }
        }
    }
}

void GameBoard::initialize()
{
    row=0;
    col=0;
    bomb=0;
    flagAmount = 0;
    status = GameStatus::standby;
    load = 0;
    BlankAmount = 0;
    remainAmount = 0;
}

int GameBoard::getStatus()
{
    return status;
}

void GameBoard::setStatus(int status)
{
    GameBoard::status = status;
}

void GameBoard::setRowColBomb(int row, int col, int bomb)
{
    this->row = row;
    this->col = col;
    this->bomb = bomb;
}

void GameBoard::setRowColBomb(int row, int col)
{
    this->row = row;
    this->col = col;
}

bool GameBoard::getLoad()
{
    return load;
}

int GameBoard::getBomb()
{
    return this->bomb;
}

int GameBoard::getflagAmount()
{
    return this->flagAmount;
}

int GameBoard::getBlankAmount()
{
    return this->BlankAmount;
}

int GameBoard::getremainAmount()
{
    return this->remainAmount;
}

int GameBoard::getRow()
{
    return this->row;
}

int GameBoard::getCol()
{
    return this->col;
}

vector<vector<char> > GameBoard::getCountBoard()
{
    return count;
}

vector<vector<char> > GameBoard::getPlayingBoard()
{
    return playingBoard;
}
