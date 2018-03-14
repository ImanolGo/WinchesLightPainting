/*
 *  ofApp.cpp
 *  Winches Calibration App
 *
 *  Created by Imanol Gomez on 14/03/18.
 *
 */



#include "ofApp.h"

#define NUM_WINCHES 6
#define NUM_CHANNELS 7
#define NUM_DMX_CHANNELS 64

void ofApp::setup() {

    dmx.autoconnect(NUM_DMX_CHANNELS);
    dmx.clear();
    dmx.update();
    
    winch.set("Winch", 1, 1, NUM_WINCHES);
    initChannel.set("InitChannel", 1, 1, 64);
    
    for(int i=0; i<NUM_CHANNELS; i++){
        ofParameter<int> parameter;
        string name = "Channel " + ofToString(i+1);
        parameter.set(name, 0, 0, 255);
        channels.push_back(parameter);
    }
    
   
    panel.setName("GUI");
    panel.setup();
    panel.setDefaultHeight(30);
    panel.setDefaultWidth(350);
    
    
    ofxButton * button = new ofxButton();
    button->setup("PANIC BUTTON");
    button->addListener(this, &ofApp::panicButton);
    panel.add(button);
    
    
    panel.add(winch);
    panel.add(initChannel);
    for(int i=0; i<channels.size(); i++){
         panel.add(channels[i]);
    }
    
    
    

}

void ofApp::update()
{
    initChannel = 1 + NUM_CHANNELS*(winch-1);
    for(int i=0; i<channels.size(); i++){
        dmx.setLevel(initChannel + i, channels[i]);
    }
    
	dmx.update();
    
    
}

void ofApp::draw() {

    if(dmx.isConnected()){
        ofBackground(10, 90, 10);
    }
    else{
        ofBackground(90, 10, 10);
    }
    
    for(int i=0; i<channels.size(); i++){
        ofSetColor(channels[i],channels[i],channels[i], 150);
        float w =  ofGetWidth() / channels.size();
        ofDrawRectangle(w*i, 0, w, ofGetHeight());
    }
    

    panel.draw();
}

void ofApp::panicButton()
{
    for(int i=0; i<channels.size(); i++){
        channels[i] = 0;
    }
    
    
    for(int i=0; i<NUM_DMX_CHANNELS; i++){
        dmx.setLevel(1+i, 0);
    }
    
    dmx.clear();
    
    dmx.update();
    
    winch = 1;
    initChannel = 0;
    
}
