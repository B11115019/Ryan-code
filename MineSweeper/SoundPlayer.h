
#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include<QSoundEffect>

enum MusicName
{
  BCGL,
  TiTi,
  bom
};

class SoundPlayer
{
public:
    static void playBgm(int index);
    static void initial();
    static void adjustVolume(int volume);
    static void stopBgm();
    static void startBgm();
private:
    static QSoundEffect bgmPlayer;
    static QList<QString> musicList;

};

#endif // SOUNDPLAYER_H
