#include <QDir>
#include "SoundPlayer.h"

QSoundEffect SoundPlayer::bgmPlayer = QSoundEffect();
QList<QString> SoundPlayer::musicList = QList<QString>();

void SoundPlayer::playBgm(int index)
{
    bgmPlayer.setSource(QUrl::fromLocalFile(musicList.at(index)));
    bgmPlayer.play();
}

void SoundPlayer::initial()
{
    QDir dir;
    bgmPlayer.setLoopCount(QSoundEffect::Infinite);
    musicList.append(dir.absolutePath() + "/enterMusic.wav");
    musicList.append(dir.absolutePath() + "/enter2bgm.wav");
    musicList.append(dir.absolutePath() + "/explosion.wav");
}

void SoundPlayer::adjustVolume(int volume)
{
    bgmPlayer.setVolume(volume/100.f);
}

void SoundPlayer::stopBgm()
{
    bgmPlayer.setLoopCount(1);
}

void SoundPlayer::startBgm()
{
    bgmPlayer.setLoopCount(QSoundEffect::Infinite);
}
