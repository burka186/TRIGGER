#pragma once

#include "ofMain.h"
#include "NoiseObjects.h"
#include "Spring.h"
#include "ofEvents.h"


class ofApp : public ofBaseApp{

public:

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);

	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    
    //visuals: ----------------------------------------------
    
    //vector of NoiseObjects to be drawn to screen
    vector <NoiseObjects> noiseCollection;
    //vector of springs
    vector<Spring> springList;
    
    ofPoint pos;
    ofPoint center;
    bool punch;
    bool removeSome;

    //sound: ------------------------------------------------
    void audioOut(float * input, int bufferSize, int nChannels);
    
    ofSoundStream soundStream;
    
    float pan;
    int sampleRate;
    float volume;
    
    vector <float> lAudio;
    vector <float> rAudio;
    
    float targetFrequency;
    float phase;
    float phaseAdder;
    float phaseAdderTarget;
    
    float targetVolume;
    float currentVolume;
    float diff;
    
    //-------------------------------------------------
    
    //arduino:----------------------------------------
    
	ofArduino	ard;
	bool		bSetupArduino; // flag variable for setting up arduino once
    
    
    private:
    
    void setupArduino(const int & version);
    void digitalPinChanged(const int & pinNum);
    void analogPinChanged(const int & pinNum);
	void updateArduino();
    void getAverage(); // average the incoming readings from the arduino
    
    vector<int> allReadings; //store all readings in a vector
    float smoothReading; //smoothed reading after averaging (duh)
    
    int ardReading; //raw reading from arduino
    string potValue;

};

