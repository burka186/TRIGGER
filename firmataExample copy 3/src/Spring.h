//
//  Spring.h
//  firmataExample
//
//  Created by Ava Burke on 12/8/14.
//  Adapted from Charlie Whitney's Spring code, created on 11/11/13
//

#pragma once
#include "ofMain.h"
#include "NoiseObjects.h"

class Spring {
public:
    void set(ofPoint center, NoiseObjects *_s2, float _k, float _restLength);
    void applyForce(ofPoint force);
    void update();
    void draw();
    
    void connect( Spring *s );
    
    ofPoint center;
    NoiseObjects *s2;
    float k, restLength;
    
};



