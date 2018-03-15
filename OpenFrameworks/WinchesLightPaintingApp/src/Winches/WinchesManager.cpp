/*
 *  WinchesManager.cpp
 *  Winches Light Painting App
 *
 *  Created by Imanol Gomez on 15/03/18.
 *
 */


#include "WinchesManager.h"
#include "AppManager.h"

const int WinchesManager::NUM_WINCHES = 6;


WinchesManager::WinchesManager(): Manager()
{
    //Intentionally left empty
}

WinchesManager::~WinchesManager()
{
   ofLogNotice() << "WinchesManager::destructor";
}


//--------------------------------------------------------------

void WinchesManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    ofLogNotice() <<"WinchesManager::initialized" ;
}



void WinchesManager::update()
{

}

