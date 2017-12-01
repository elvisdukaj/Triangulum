#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <map>
#include <string>
#include <SFML/Audio.hpp>
#include "SoundId.h"

class AudioManager {
public:
    AudioManager();

    void init();
    void playSound(SoundId id);
    void setMusicVolume(int vol);
    void setSoundVolume(int vol);

private:
    void loadSoundEffect(const std::string& fileName, SoundId id);
    void loadMusic(const std::string& fileName);
    void play(const sf::SoundBuffer& soundBuffer);

private:
    using SoundBufferMap = std::map<SoundId, sf::SoundBuffer>;
    using SoundList = std::vector<sf::Sound>;

private:
    sf::Music m_music;
    SoundBufferMap m_soundMap;
    SoundList m_soundList;
    size_t m_soundListIndex;
    int m_soundVol;
};

#endif // AUDIOMANAGER_H
