#include "Mainwindow.h"
#include "Gameboard.h"
#include <iostream>
#include <string>
#include <QApplication>
#include "SoundPlayer.h"

using namespace std;

int main(int argc, char *argv[])
{
    srand(time(NULL));
    SoundPlayer::initial();
    GameBoard mode;

    if(argc == 2 && (string)argv[1] == "CommandInput")
    {
        mode.GameSetCammand();
    }
    else if(argc == 4 && (string)argv[1] == "CommandFile")
    {
        mode.RunCommandFile(argv[2], argv[3]);
    }
    else
    {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        a.exec();
    }

    return 0;
}
