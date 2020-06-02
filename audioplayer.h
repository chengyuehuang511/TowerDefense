#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QtMultimedia/QMediaPlayer>
enum SoundType
{
    TowerPlaceSound,
    LifeLoseSound,
    LaserShootSound,
    EnemyDestroySound
};
class AudioPlayer:public QObject
{
    Q_OBJECT
public:
    explicit AudioPlayer(QObject *parent=0);
    void startBGM();
    void playSound(SoundType soundType);
private:
    QMediaPlayer *m_backgroundMusic;
};

#endif // AUDIOPLAYER_H
