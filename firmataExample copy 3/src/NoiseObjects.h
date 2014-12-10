#pragma once
#include "ofMain.h"

class NoiseObjects {
    //when you create the object (an instance of the class) for the first time, you feed it information about what you're doing
public:
    
    //constructor
    NoiseObjects(float mX, float mY);
    
    //update position of current noiseObjects
    void update();
    //draw current noiseObjects to screen
    void render();
    //apply physics to noiseObjects
    void addForce(ofPoint _force);
    void resetForce();
    
    //position velocity and acceleration vectors (points have x, y, and z)
    ofPoint pos;
    ofPoint vel;
    ofPoint acc;
    float damping;
    
    //how easy are the words to destroy?
    float resistance;
    
    //text stuff:
    string text;
    ofColor currentColor;
    ofColor targetColor;
    ofTrueTypeFont TTF;
    int fontSize;
    
    
    
    //XML file
    ofXml noise;
    
    //number of nodes in XML file
    int numNoise;
};
