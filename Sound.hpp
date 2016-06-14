//
//  Sound.hpp
//  HW2
//
//  Created by Junxi Li on 5/27/16.
//  Copyright © 2016 Junxi Li. All rights reserved.
//

#ifndef Sound_hpp
#define Sound_hpp

#include <ik_ISoundEngine.h>
#include <irrKlang.h>
#include <string>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "shader.h"
#include "Window.h"

class Sound{
 public:
    bool play;
    int time;
    Sound(const char*);
    void pain_sound();
    void init();
    const char* sound_path;
};

#endif /* Sound_hpp */
