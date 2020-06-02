#include "audioplayer.h"
#include <QDir>
#include <QMediaPlaylist>

AudioPlayer::AudioPlayer(QObject *parent):
    QObject(parent),m_backgroundMusic(NULL)
{
    QUrl backgroundMusicUrl=QUrl("qrc:/music/bgm.mp3");
    m_backgroundMusic=new QMediaPlayer(this);
    QMediaPlaylist *backgroundMusicList=new QMediaPlaylist();
    QMediaContent media(backgroundMusicUrl);
    backgroundMusicList->addMedia(media);
    backgroundMusicList->setCurrentIndex(0);
    backgroundMusicList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    m_backgroundMusic->setPlaylist(backgroundMusicList);
}
void AudioPlayer::startBGM()
{
    if(m_backgroundMusic)
        m_backgroundMusic->play();
}
void AudioPlayer::playSound(SoundType soundType)
{
    static const QUrl mediasUrls[]=
    {
        QUrl("qrc:/music/plant.mp3"),
        QUrl("qrc:/music/wave--.wav"),
        QUrl("qrc:/music/shoot.mp3"),
        QUrl("qrc:/music/enemy_die.mp3"),
    };
    static QMediaPlayer player;
    player.setMedia(mediasUrls[soundType]);
    player.play();
}
