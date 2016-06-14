//
//  Sound.cpp
//  HW2
//
//  Created by Junxi Li on 5/27/16.
//  Copyright © 2016 Junxi Li. All rights reserved.
//

#include "Sound.hpp"
using namespace irrklang;

//ISoundEngine *SoundEngine = irrklang::createIrrKlangDevice(irrklang::ESOD_AUTO_DETECT, irrklang::ESEO_DEFAULT_OPTIONS, 0, "IRRKLANG_SDK_VERSION");
ISoundEngine *SoundEngine = createIrrKlangDevice();
Sound::Sound(const char* input){
    play = false;
    time=0;
    //sound_path = input;
    //SoundEngine->play2D(input);
    SoundEngine->play2D("/Users/Frankie/CSE167/Final/Wind.wav", true);
}

void Sound::init(){
    SoundEngine->play2D("/Users/Frankie/CSE167/Final/Wind.wav");
}

void Sound::pain_sound(){
    if(play && time < 2){
        SoundEngine->play2D("/Users/Frankie/CSE167/Final/Pain.wav");
        time++;
    }
}