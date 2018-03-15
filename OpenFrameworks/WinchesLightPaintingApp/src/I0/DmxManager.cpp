/*
 *  DmxManager.cpp
 *  Winches Light Painting App
 *
 *  Created by Imanol Gomez on 14/03/18.
 *
 */


#include "DmxManager.h"
#include "AppManager.h"

const int DmxManager::NUM_CHANNELS_PER_WINCH = 7;

DmxManager::DmxManager(): Manager(), m_panicState(false)
{
    //Intentionally left empty
}

DmxManager::~DmxManager()
{
   ofLogNotice() << "DmxManager::destructor";
   this->exit();
}


//--------------------------------------------------------------

void DmxManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    this->setupDmx();
    ofLogNotice() <<"DmxManager::initialized" ;
}



void DmxManager::setupDmx()
{
    int dmxPort = AppManager::getInstance().getSettingsManager().getDmxPort();
    if(dmxPort<0){
        ofLogNotice() <<"DmxManager::setupSerial << Autoconnecting dmx port";
        m_dmx.autoconnect(255);
    }
    else{
        ofLogNotice() <<"DmxManager::setupSerial << Connecting dmx to port " << dmxPort;
        m_dmx.connect(dmxPort,255);
    }
}



void DmxManager::update()
{
    if(m_panicState){
        return;
    }
    
    m_dmx.update();
}


void DmxManager::onSetPosition(int _id, float percentage)
{
    if(m_panicState){
        return;
    }
    
    unsigned short int positionDmx = (int) ofMap(percentage, 0.0, 100.0, 0.0, sizeof(unsigned short int), true);
    
    ofLogNotice() <<"DmxManager::onSetPosition << unsigned short int  level: " << positionDmx;;
    
    unsigned char positionHiLevel = positionDmx / sizeof(unsigned char) ;
    unsigned char positionLoLevel = positionDmx % sizeof(unsigned char) ;
    
    auto numWinches = AppManager::getInstance().getWinchesManager().getNumWinches();
    int positionHiChannel = DmxChannels::PositionHi + _id*numWinches;
    int positionLowChannel = DmxChannels::PositionLo + _id*numWinches;
    
    m_dmx.setLevel(positionHiChannel, positionHiLevel);
    m_dmx.setLevel(positionLowChannel, positionLoLevel);
    ofLogNotice() <<"DmxManager::onSetPosition << Set channel Hi: " << positionHiChannel << ", level " << positionHiLevel;
    ofLogNotice() <<"DmxManager::onSetPosition << Set channel Hi: " << positionLowChannel << ", level " << positionLoLevel;
}

void DmxManager::onSetSpeed(int _id, float percentage)
{
    if(m_panicState){
        return;
    }
    
    unsigned char speedLevel = (int) ofMap(percentage, 0.0, 100.0, 0.0, sizeof(unsigned char), true);
    auto numWinches = AppManager::getInstance().getWinchesManager().getNumWinches();
    int speedChannel = DmxChannels::Speed + _id*numWinches;
    m_dmx.setLevel(speedChannel, speedLevel);
    ofLogNotice() <<"DmxManager::onSetSpeed << Set channel: " << speedChannel << ", level " << speedLevel;
}

void DmxManager::setPanic()
{
    m_dmx.clear();
    m_dmx.update();
}

void DmxManager::exit()
{
    this->setPanic();
    ofSleepMillis(300);
}

void DmxManager::onPanicChange(bool& value)
{
    m_panicState = value;
    if(m_panicState){
        this->setPanic();
    }
}
