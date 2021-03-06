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
    
    
    //ofLogNotice() <<"DmxManager::onSetPosition << id: " << _id << ", percentage: " << percentage;
    
     //auto numWinches = AppManager::getInstance().getWinchesManager().getNumWinches();
    
    unsigned short int positionDmx = (int) ofMap(percentage, 0.0, 100.0, 0.0, 255*255, true);
//
//    ofLogNotice() <<"DmxManager::onSetPosition << unsigned short int  level: " << positionDmx;
//    ofLogNotice() <<"DmxManager::onSetPosition << dmx num " << _id*NUM_CHANNELS_PER_WINCH;;
    
    unsigned char positionHiLevel = positionDmx / 255 ;
    unsigned char positionLoLevel = positionDmx % 255 ;
    
   
    int positionHiChannel = DmxChannels::PositionHi + _id*NUM_CHANNELS_PER_WINCH;
    int positionLowChannel = DmxChannels::PositionLo + _id*NUM_CHANNELS_PER_WINCH;
    
    m_dmx.setLevel(positionHiChannel, positionHiLevel);
    m_dmx.setLevel(positionLowChannel, positionLoLevel);
    //ofLogNotice() <<"DmxManager::onSetPosition << Set channel Hi: " << positionHiChannel << ", level " <<  (int)positionHiLevel;
    //ofLogNotice() <<"DmxManager::onSetPosition << Set channel Lo: " << positionLowChannel << ", level " << (int) positionLoLevel;
    
   // m_dmx.update();
}

void DmxManager::onSetSpeed(int _id, float percentage)
{
    if(m_panicState){
        return;
    }
    
    //ofLogNotice() <<"DmxManager::onSetSpeed << id: " << _id*NUM_CHANNELS_PER_WINCH << ", percentage: " << percentage;
    
    unsigned char speedLevel = (unsigned char) ofMap(percentage, 0.0, 100.0, 0.0, 255, true);
    auto numWinches = AppManager::getInstance().getWinchesManager().getNumWinches();
    int speedChannel = DmxChannels::Speed + _id*NUM_CHANNELS_PER_WINCH;
    m_dmx.setLevel(speedChannel, speedLevel);
   // ofLogNotice() <<"DmxManager::onSetSpeed << Set channel: " << speedChannel << ", level " << int(speedLevel);
    
    //m_dmx.update();
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
    m_dmx.disconnect();
}

void DmxManager::onPanicChange(bool& value)
{
    m_panicState = value;
    if(m_panicState){
        this->setPanic();
    }
}


void  DmxManager::onSendDmx(int channel, unsigned char level)
{
     ofLogNotice() <<"DmxManager::onSendDmx << Set channel: " << channel << ", level " << int(level);
    
     m_dmx.setLevel(channel, level);
     m_dmx.update();
}
