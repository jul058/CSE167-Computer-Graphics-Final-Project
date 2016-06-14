//
//  Cylinder.hpp
//  cse167_hw2
//
//  Created by Simon on 5/3/16.
//  Copyright © 2016 Simon. All rights reserved.
//



#include <stdio.h>
#include "Geode.hpp"
using namespace std;
class Cylinder:public Geode
{
    void update(glm::mat4 C);
};

