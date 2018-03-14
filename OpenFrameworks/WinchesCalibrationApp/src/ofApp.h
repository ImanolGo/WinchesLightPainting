/*
 *  ofApp.h
 *  Winches Calibration App
 *
 *  Created by Imanol Gomez on 14/03/18.
 *
 */




#pragma once

#include "ofMain.h"
#include "ofxDmx.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofxDmx dmx;
	int level;
    
    void panicButton();
    
    ofxPanel panel;
    
    ofParameter<int> winch;
    ofParameter<int> initChannel;
    vector< ofParameter<int>> channels;
    
    ofParameter<bool> autoCycle;
    
};
