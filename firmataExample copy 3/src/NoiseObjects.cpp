#include "NoiseObjects.h"

//custom constructor: create new instance of NoiseObjects class
NoiseObjects::NoiseObjects(float mX, float mY) {
    
    //pass in position and set velocity /acceleration/damping
    pos.set(mX, mY, 0.0);
    vel.set((ofRandom(0, 1.0)), (ofRandom(0, 1.0)), 0.0);
    acc.set(0.0, 0.0, 0.0);
    damping = ofRandom(0.1, 0.3);
    
    //pull data from XML file (this used to be in the main setup loop but is much more efficient here within the class)
    noise.load("rage_single_words.xml");
    //set to parent node:
    noise.setTo("noise_objects");
    //how many children are there?
    numNoise = noise.getNumChildren();
    
    //pick a random index number between 0 and the number of nodes...
    int randNum = ofRandom(0, numNoise);
    noise.setToChild(randNum);
    
    //get the value of the selected node:
    text = noise.getValue();
    
    //load font
    fontSize = ofRandom(12, 30);
    TTF.loadFont("mono.ttf", fontSize);
    
    //random shade of grey...
    targetColor = ofRandom(150, 255);
    //start targetColor = to currentColor
    currentColor = targetColor;
    
    //randomize resistance (resistance will corrolate to how 'stubborn' each
    //noise object is to being removed when you "punch" (or, for these purposes, click)
    resistance = ofRandom(0.02, 0.08);

}


void NoiseObjects::update() {
    
    //update position
    vel += acc;
    vel *= (1.0 - damping);
    pos += vel;
    
    //reset force after each loop
    resetForce();
    
}

void NoiseObjects::render() {
    
    //set color to current color
    ofSetColor(currentColor);
    
    //draw to screen
    TTF.drawString(text, pos.x, pos.y);

}

//this is where I'm simulating how I want the noise objects to react when there is a "punch"
void NoiseObjects::addForce(ofPoint _force) {
    
    acc += _force;
    
    //resistance++;
    
}

void NoiseObjects::resetForce() {
    //reset acceleration to 0
    acc.set(0.0, 0.0, 0.0);
}

