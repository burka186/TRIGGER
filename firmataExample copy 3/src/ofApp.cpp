#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(0);
    
    //punch measurement stuff here ---------------------------
    punch = false;
    removeSome = false;
    center.set(ofGetWidth()/2, ofGetHeight()/2);
    
    //VISUAL STUFF HERE ---------------------------------------
    noiseCollection = vector<NoiseObjects>();
    
    //create 500 initial NoiseObjects at random positions in the center of the screen
    //and push these back to the noiseCollection vector
    for (int i = 0; i < 500; i++) {
        pos.x = ofRandomWidth();
        pos.y = ofRandomHeight();
        
        NoiseObjects newNoise(pos.x, pos.y);
        noiseCollection.push_back(newNoise);
    }
    
    //connect springs to particles and push these back to the springList vector
    for(int i = 0; i < noiseCollection.size(); i++) {
        Spring newSpring;
        float dist = ofDist(center.x, center.y, noiseCollection[i].pos.x, noiseCollection[i].pos.y);
        newSpring.set(center, &noiseCollection[i], noiseCollection[i].resistance, dist);
        springList.push_back(newSpring);
    }
    
    //SOUND STUFF HERE ----------------------------------------
    
    int bufferSize = 512;
    sampleRate = 44100;
    phase = 0;
    phaseAdder = 0.0f;
    phaseAdderTarget = 0.0f;
    targetVolume = 0.9f;
    currentVolume = targetVolume;
    
    lAudio.assign(bufferSize, 0.0);
    rAudio.assign(bufferSize, 0.0);
    
    soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);
    
    smoothReading = 0.;
    
    //ARDUINO STUFF HERE -----------------------------
    
    potValue = "analog pin:";

    //connect to arduino
	ard.connect("/dev/tty.usbserial-DA00T1YT", 57600);
	
	// listen for arduino to be ready. when it is ready, call setupArduino() function
	ofAddListener(ard.EInitialized, this, &ofApp::setupArduino);
	bSetupArduino	= false;	// flag so we setup arduino when its ready, you don't need to touch this :)
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //ARDUINO: -------------------------------------------------
    updateArduino();
    
    //PUNCH: ---------------------------------------------------
    getAverage();
    
    //VISUAL: --------------------------------------------------
    
    //update however many objects there are in noiseCollection
    for (int i = 0; i < noiseCollection.size(); i++) {
        
        //apply force if there's a punch
        if(punch) {
            ofPoint direction = center - noiseCollection[i].pos;
            direction.normalize();
            ofPoint force = direction * (diff / 3); //this makes the amount moved proportional to how hard the punch was
            noiseCollection[i].addForce(-force);
            noiseCollection[i].update();
        }
        else {
            noiseCollection[i].resetForce();
            punch = false;
            // and/or just update all the noise objects
            noiseCollection[i].update();
        }
    }
    
    //if there was a punch, reset to false each frame
    //if(punch) {
        //punch = false;
    //}
    
    
    
    //update springs
    for(int i = 0; i < noiseCollection.size(); i++) {
        springList[i].update();
    }
    
    //if you need to remove some of the words from the screen:
    if (removeSome) {
        int i = noiseCollection.size() - 1;
        int min = noiseCollection.size() - 20; //this means 10 will be removed each time
        cout << "noise collection size" << min << endl;
        //while in range, erase the noise objects and their corresponding springs from the vectors
        while (i > min && i >= 0) {
            noiseCollection.erase(noiseCollection.begin() + i);
            springList.erase(springList.begin() + i);
            i--;
        }
        
        //set removeSome back to false
        removeSome = false;
    }

    
    //SOUND: ---------------------------------------------------
    //current volume is always trying to get back to target volume every loop (easing)
    currentVolume += (targetVolume - currentVolume) * 0.05;
    
    //make sure they don't go below 0
    currentVolume = ofClamp(currentVolume, 0., 1.0);
    targetVolume = ofClamp(targetVolume, 0., 1.0);

}

//--------------------------------------------------------------
void ofApp::getAverage(){
    float sum = 0.;
    //set initial values to make sure they will be replaced by the incoming readings
    float highest = 0.;
    float lowest = 1000.;
    
    //replace dummy highest and lowest readings with actual readings from within the vector of allReadings
    for(int i = 0; i < allReadings.size(); i++){
        sum += allReadings[i];
        if(allReadings[i] < lowest){
            lowest = allReadings[i];
        }
        if(allReadings[i] > highest){
            highest = allReadings[i];
        }
    }
    
    //calculate the difference between the highest and lowest reading within the current vector of data

    diff = highest - lowest;
    
    //this is the punch threshold
    float threshold = 100.;
    
    //if there's a punch...
    if (diff > threshold) {
        
        //as punch counter increases, volume -= 0.01f each time
        //current volume is affected directly by a punch (goes down a lot) but target volume goes
        //down by 0.1. current volume is trying to go back to target volume in update
        cout << "punch" << endl;
        
        //record that there was a punch and that words should be removed
        punch = true;
        removeSome = true;
        
        //play with these for sensitivity
        currentVolume -= 0.05f;
        targetVolume -= 0.01f;
    
    }
    
    //for loop through noiseObjects vector, if diff > that object.resistance, then do something to the object
    
    smoothReading = sum / allReadings.size();
    cout << "smoothed reading: " << smoothReading << endl;
}

//--------------------------------------------------------------
void ofApp::setupArduino(const int & version) {
	
	// remove listener because we don't need it anymore
	ofRemoveListener(ard.EInitialized, this, &ofApp::setupArduino);
    
    // it is now safe to send commands to the Arduino
    bSetupArduino = true;
    
    // print firmware name and version to the console
    ofLogNotice() << ard.getFirmwareName(); 
    ofLogNotice() << "firmata v" << ard.getMajorFirmwareVersion() << "." << ard.getMinorFirmwareVersion();

    // set pin A0 to analog input
    ard.sendAnalogPinReporting(0, ARD_ANALOG);
	
    // Listen for changes on the digital and analog pins
    ofAddListener(ard.EAnalogPinChanged, this, &ofApp::analogPinChanged);    
}

//--------------------------------------------------------------
void ofApp::updateArduino(){

	// update the arduino, get any data or messages.
    // the call to ard.update() is required
	ard.update();
	
	// do not send anything until the arduino has been set up
	if (bSetupArduino) {

	}

}


//--------------------------------------------------------------
void ofApp::analogPinChanged(const int & pinNum) {
    // do something with the analog input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    
    ardReading = ard.getAnalog(pinNum);
    
    potValue = "analog pin: " + ofToString(pinNum) + " = " + ofToString(ardReading);
    
    allReadings.push_back(ardReading);
    
    //if the allReadings vector > 5, erase the oldest reading
    //play around with vector size to change sensitivity
    if(allReadings.size() > 8){
        allReadings.erase(allReadings.begin());
    }
}


//--------------------------------------------------------------
void ofApp::draw(){

    
    //VISUAL: ----------------------------------------------
    //draw however many objects there are in noiseCollection
    for (int i = 0; i < noiseCollection.size(); i++) {
        noiseCollection[i].render();
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    pan = 0.5f;
    float leftScale = 1 - pan;
    float rightScale = pan;
    
    //this creates the static sound
    for (int i = 0; i < bufferSize; i++) {
        lAudio[i] = output[i*nChannels ] = ofRandom(0, 1) * currentVolume * leftScale;
        lAudio[i] = output[i*nChannels ] = ofRandom(0, 1) * currentVolume * rightScale;
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}