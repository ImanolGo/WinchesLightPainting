/*
 *  KeyboardManager.cpp
 *  Winches Light Painting App
 *
 *  Created by Imanol Gomez on 14/03/18.
 *
 */

#include "KeyboardManager.h"
#include "AppManager.h"


KeyboardManager::KeyboardManager(): Manager()
{
    //Intentionally left empty
}

KeyboardManager::~KeyboardManager()
{
   ofLogNotice() << "KeyboardManager::destructor";
}


//--------------------------------------------------------------

void KeyboardManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    ofLogNotice() <<"KeyboardManager::initialized" ;
    this->addListeners();
}

void KeyboardManager::addListeners()
{
    ofRegisterKeyEvents(this); // this will enable the InteractiveVisual class to listen to the key events.
}

void KeyboardManager::keyPressed(ofKeyEventArgs &e)
{
    int key = e.key;
    
    //ofLogNotice() <<"KeyboardManager::keyPressed-> " + ofToString(key);
    
  
    if(key == 'g' || key == 'G') {
        AppManager::getInstance().getGuiManager().toggleGui();
    }
    
    else if(key == OF_KEY_RIGHT) {
        AppManager::getInstance().getTimeLineManager().playNext();
    }
    
    else if(key == OF_KEY_LEFT) {
        AppManager::getInstance().getTimeLineManager().playPrevious();
    }
    
    else if(key == 'r' || key == 'R') {
        AppManager::getInstance().getTimeLineManager().reset();
    }
    
    else if(key == 'f' || key == 'F')
    {
        AppManager::getInstance().getTimeLineManager().playForward();
    }
    
    else if(key == 'b' || key == 'B')
    {
        AppManager::getInstance().getTimeLineManager().playBackwards();
    }
    
    else if(key == 's' || key == 'S')
    {
        AppManager::getInstance().getTimeLineManager().stop();
    }
    
    else if(key == 'p' || key == 'P')
    {
        AppManager::getInstance().getGuiManager().onPanicToggleChange(true);
    }
    
}

void KeyboardManager::keyReleased(ofKeyEventArgs &e)
{
    int key = e.key;
    
    //ofLogNotice() <<"KeyboardManager::keyReleased-> " + ofToString(key);
    
}










