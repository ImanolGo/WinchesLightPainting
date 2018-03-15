/*
 *  WinchesLightPaintingApp.cpp
 *  Winches Light Painting App
 *
 *  Created by Imanol Gomez on 14/03/18.
 *
 */


#include "AppManager.h"

#include "WinchesLightPaintingApp.h"

//--------------------------------------------------------------
void WinchesLightPaintingApp::setup(){
    AppManager::getInstance().setup();
}

//--------------------------------------------------------------
void WinchesLightPaintingApp::update(){
    AppManager::getInstance().update();
}

//--------------------------------------------------------------
void WinchesLightPaintingApp::draw(){
    AppManager::getInstance().draw();
}

void WinchesLightPaintingApp::exit()
{
    ofLogNotice() <<"WinchesLightPaintingApp::exit";
    AppManager::getInstance().exit();
}

//--------------------------------------------------------------
void WinchesLightPaintingApp::keyPressed(int key){

}

//--------------------------------------------------------------
void WinchesLightPaintingApp::keyReleased(int key){

}

//--------------------------------------------------------------
void WinchesLightPaintingApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void WinchesLightPaintingApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void WinchesLightPaintingApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void WinchesLightPaintingApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void WinchesLightPaintingApp::windowResized(int w, int h)
{
    AppManager::getInstance().getLayoutManager().windowResized(w,h);
}

//--------------------------------------------------------------
void WinchesLightPaintingApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void WinchesLightPaintingApp::dragEvent(ofDragInfo dragInfo)
{
}
