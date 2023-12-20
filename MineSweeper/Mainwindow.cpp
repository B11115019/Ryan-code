
#include "Mainwindow.h"
#include "ui_Mainwindow.h"
#include <iostream>
#include <ctime>
#include "Gameboard.h"
#include <QString>
#include <vector>
#include <QMessageBox>
#include "SoundPlayer.h"
#include <QDir>
#include <QMovie>
#include <QtCore/QThread>
#include <QElapsedTimer>
#include <QTimer>

using namespace std;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->playingPage->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(0);

    SoundPlayer::playBgm(MusicName::BCGL);
    mainHeight = this->height();
    mainWidth = this->width();
    ui->filePathTextEdit->setText("board1.txt");

    connect(ui->PrintGameStateButton1, SIGNAL(pressed()), this, SLOT(printGameState()));
    connect(ui->PrintGameAnswerButton1, SIGNAL(pressed()), this, SLOT(printGameAnswer()));
    connect(ui->PrintGameBoardButton1, SIGNAL(pressed()), this, SLOT(printGameBoard()));
    connect(ui->boardTypeComboBox, SIGNAL(currentIndexChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
    connect(ui->Load1Button, SIGNAL(pressed()), this, SLOT(load1Button()));
    connect(ui->load2Button, SIGNAL(pressed()), this, SLOT(load2Button()));
    connect(ui->load3Button, SIGNAL(pressed()), this, SLOT(load3Button()));
    connect(ui->startGameButton, SIGNAL(pressed()), this, SLOT(startGame()));
    connect(ui->PrintGameStateButton2, SIGNAL(pressed()), this, SLOT(printGameState()));
    connect(ui->PrintGameAnswerButton2, SIGNAL(pressed()), this, SLOT(printGameAnswer()));
    connect(ui->PrintGameBoardButton2, SIGNAL(pressed()), this, SLOT(printGameBoard()));
    connect(ui->musiclSlider, SIGNAL(valueChanged(int)), this, SLOT(volumeAdjust()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::switchPage()
{
    ui->playingPage->setCurrentIndex(1);
    SoundPlayer::playBgm(MusicName::TiTi);
    setBombAmount();
    setFlagAmount();
    setOpenBlankAmount();
    setRemainBlankAmount();
}

void MainWindow::printGameState()
{
    if(GameBoard::getStatus() == GameStatus::standby)
    {
        cout << "<Print GameState> : Standby" << endl;
        ui->gameStateLabel->setText("Standby");
    }
    else if(GameBoard::getStatus() == GameStatus::playing)
    {
        cout << "<Print GameState> : playing" << endl;
        ui->gameState1Label->setText("Playing");
    }
    else if(GameBoard::getStatus() == GameStatus::gameover)
    {
        cout << "<Print GameState> : gameover" << endl;
        ui->gameStateLabel->setText("Gameover");
    }
}

void MainWindow::printGameAnswer()
{
    game.PrintGameAnswer();
}

void MainWindow::printGameBoard()
{
    game.PrintGameBoard();
}

void MainWindow::load1Button()
{
    string file="";
    file = ui->filePathTextEdit->toPlainText().toStdString();
    game.setGameBoard(file);
}

void MainWindow::load2Button()
{
    int row=0,col=0,bombAmount=0;

    row = ui->rowsAmountSpinBox1->value();
    col = ui->colsAmountSpinBox1->value();
    bombAmount = ui->bombAmountSpinBox->value();

    game.setRowColBomb(row, col, bombAmount);
    game.setGameBoard();
}

void MainWindow::load3Button()
{
    int row=0,col=0;
    double rate=0;

    row = ui->rowsAmountSpinBox2->value();
    col = ui->colsAmountSpinBox2->value();
    rate = ui->bombRateSpinBox->value();

    game.setRowColBomb(row, col);
    game.setGameBoard(rate);
}

void MainWindow::startGame()
{
    bool loadStatus = game.getLoad();

    if(!loadStatus)
    {
        cout << "<StartGame> : Failed" << endl;
    }
    else
    {
        int flag = 0;
        cout << "<StartGame> : Success" << endl;
        flag = GameStatus::playing;

        game.setStatus(flag);
        switchPage();
        makeNewBoard();
    }
}

void MainWindow::setBombAmount()
{
    ui->bombAmountLabel->setText(QString::number(game.getBomb()));
}

void MainWindow::setFlagAmount()
{
    ui->flagAmountLabel->setText(QString::number(game.getflagAmount()));
}

void MainWindow::setOpenBlankAmount()
{
    ui->openBlankAmountLabel->setText(QString::number(game.getBlankAmount()));
}

void MainWindow::setRemainBlankAmount()
{
    ui->remainBlankAmountLabel->setText(QString::number(game.getremainAmount()));
}

void MainWindow::leftClicked()
{
    QRightClickButton *Button = qobject_cast<QRightClickButton*>(sender());  //????
    int row=0, col=0;

    getRowCol(row, col, Button);
    game.pressLeftclick(row, col);

    vector<vector<char>> countBoard, playingBoard;
    countBoard = game.getCountBoard();
    playingBoard = game.getPlayingBoard();

    bool endFlag=0;
    if((countBoard[row][col] == 'X' && playingBoard[row][col] != 'f') || (playingBoard[row][col] == '?' && countBoard[row][col] == 'X'))
    {
        endFlag=1;
        cout << "You lose the game" << endl;
        game.setStatus(GameStatus::gameover);

        bombAppear(countBoard);

        //set little window****
        QMessageBox window;
        window.setWindowTitle("Minesweeper");
        window.setText("You lose the game");

        SoundPlayer::stopBgm();
        SoundPlayer::playBgm(MusicName::bom);

        window.setStandardButtons(QMessageBox::Retry | QMessageBox::Discard);
        int returnValue = window.exec();

        switch(returnValue)
        {
        case QMessageBox::Retry:
            replayMode();
            break;
        case QMessageBox::Discard:
            quitMode();
            break;
        }
        //*****
    }
    else if(countBoard[row][col] == '0' && playingBoard[row][col] != 'f')
    {
        expand(Button, playingBoard);
    }
    else if(playingBoard[row][col] != 'f')
    {
        QChar temp = countBoard[row][col];
        Button->setIcon(QIcon());

        if(temp != '0') Button->setText(temp);

        setOpenBlankAmount();
        setRemainBlankAmount();
    }

    if(game.getremainAmount() == 0 && !endFlag)
    {
        cout << "You win the game" << endl;
        game.setStatus(GameStatus::gameover);

        QMessageBox window;
        window.setWindowTitle("Minesweeper");
        window.setText("You win the game");
        window.setStandardButtons(QMessageBox::Retry | QMessageBox::Discard);

        int returnValue = window.exec();

        switch(returnValue)
        {
        case QMessageBox::Retry:
            replayMode();
            break;
        case QMessageBox::Discard:
            quitMode();
            break;
        }
    }
}

void MainWindow::rightClicked()
{
    int num = 0;
    QDir dir;

    QRightClickButton *button = qobject_cast<QRightClickButton*>(sender());  //????
    int row=0, col=0;
    getRowCol(row, col, button);
    game.pressRightclick(row, col);

    vector<vector<char>> playingBoard = game.getPlayingBoard();

    switch(playingBoard[row][col])
    {
    case'f':
        num = rand()%2;
        cout << "num = " << num << endl;

        if(num) button->setIcon(QIcon(dir.absolutePath() + "/Taiwan.png"));
        else button->setIcon(QIcon(dir.absolutePath() + "/China.png"));

        setFlagAmount();
        break;
    case'?':
        button->setIcon(QIcon(dir.absolutePath() + "/middle.png"));
        setFlagAmount();
        break;
    case'#':
        button->setIcon(QIcon());
        break;
    default:
        break;
    }
}

void MainWindow::middleClicked()
{
    int random = 0;

    if(count == 4)
    {
        //速度變快問題未解決
        connect(timer, SIGNAL(timeout()), this, SLOT(changeColor()));
        timer->start(500);
        count = 0;
    }
    else
    {
        count++;
        random = rand()%18;
        ui->playingPage->setStyleSheet("border: 2px solid " + color[random] + ";");
    }
}

void MainWindow::replayMode()
{
    ui->playingPage->setCurrentIndex(0);
    game.initialize();
    ui->playingPage->setPalette(orginColor);
    ui->playingPage->setStyleSheet("border: 2px solid rgba (0);");

    SoundPlayer::startBgm();
    SoundPlayer::playBgm(MusicName::BCGL);

    qDeleteAll(ui->boardWidget->findChildren<QRightClickButton*>());
    setRemainBlankAmount();
    setBombAmount();
    setFlagAmount();
    setOpenBlankAmount();

    this->setFixedHeight(mainHeight);
    this->setFixedWidth(mainWidth);
}

void MainWindow::quitMode()
{
    cout << "<Quit> : Success" << endl;
    this->close();
}

void MainWindow::volumeAdjust()
{
    SoundPlayer::adjustVolume(ui->musiclSlider->value());
}

void MainWindow::changeColor()
{
    if(colorIndex == 10)
    {
        colorIndex = 0;
        ui->playingPage->setStyleSheet("border: 2px solid rgba (0) ;");
        timer->stop();
        return;
    }

    int random = 0;
    random = rand() % 18;
    ui->playingPage->setStyleSheet("border: 2px solid " + color[random] + ";");
    colorIndex++;
    cout << colorIndex << endl;
}

void MainWindow::makeNewBoard()
{
    ui->boardWidget->setFixedSize(game.getCol()*30, game.getRow()*30);

    button = QList<QRightClickButton*>();

    for(int i=0;i<game.getRow();i++)
    {
        for(int j=0;j<game.getCol();j++)
        {
            QRightClickButton* Button = new QRightClickButton(ui->boardWidget);

            Button->setFixedSize(30,30);
            Button->setObjectName(to_string(i) + " " + to_string(j));
            Button->setText(" ");

            connect(Button, SIGNAL(pressed()), this, SLOT(leftClicked()));
            connect(Button, SIGNAL(setRightClicked()), this, SLOT(rightClicked()));
            connect(Button, SIGNAL(setMiddleClicked()), this, SLOT(middleClicked()));

            ui->boardLayout->addWidget(Button, i, j);
            button.append(Button);
        }
    }

    ui->scrollArea->setWidget(ui->boardWidget);
}

void MainWindow::expand(QRightClickButton *Button, vector<vector<char>> &playingBoard)
{
    //QDir dir;
    int totalRow = game.getRow();
    int totalCol = game.getCol();

    for(int i = 0;i < totalRow;i++)
    {
        for(int j = 0;j < totalCol;j++)
        {
            if(playingBoard[i][j] != '#' && playingBoard[i][j] != 'f' && playingBoard[i][j] != '?')
            {
                QChar temp = playingBoard[i][j];

                if(temp != '0') button.at(i * totalCol + j)->setText(temp);
                else
                {
                    button.at(i * totalCol + j)->setStyleSheet("background-color:pink");
                }

                setOpenBlankAmount();
                setRemainBlankAmount();
            }
        }
    }
}

void MainWindow::getRowCol(int &row, int &col, QRightClickButton *button)
{
    int index=0;
    string str = button->objectName().toStdString();

    for(int i=0;i<str.size();i++)
    {
        if(str[i] == ' ')
        {
            index=i+1;
            break;
        }

        row = row*10 + str[i] - '0';
    }

    for(int i=index;i<str.size();i++)
    {
        col = col*10 + str[i] - '0';
    }
}

void MainWindow::bombAppear(vector<vector<char>> countBoard)
{
    QDir dir;
    int totalRow = game.getRow();
    int totalCol = game.getCol();

    for(int i = 0;i < totalRow;i++)
    {
        for(int j = 0;j < totalCol;j++)
        {
            QChar temp = countBoard[i][j];

            if(temp == 'X')
            {
                button.at(i * totalRow + j)->setIcon(QIcon(dir.absolutePath() + "/183377.png"));
                /*QHBoxLayout* feature = new QHBoxLayout(button.at(i * totalRow + j));
                QLabel *featureLabel = new QLabel();
                QMovie *animation = new QMovie();

                animation->setFileName(dir.absolutePath() + "/BombExplosion.gif");
                featureLabel->setMovie(animation);
                animation->start();

                feature->addWidget(featureLabel);*/
            }

            setOpenBlankAmount();
            setRemainBlankAmount();
        }
    }
}
