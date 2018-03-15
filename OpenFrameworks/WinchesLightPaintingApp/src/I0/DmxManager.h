/*
 *  DmxManager.h
 *  Winches Light Painting App
 *
 *  Created by Imanol Gomez on 14/03/18.
 *
 */


#pragma once

#include "ofMain.h"
#include "Manager.h"
#include "ofxDmx.h"

//========================== class DmxManager =======================================
//==============================================================================
/** \class DmxManager DmxManager.h
 *	\brief class for managing DMX communication
 *	\details It connects to an Enttec USB pro and sends serial-dmx commands
 */


enum DmxChannels { PositionHi = 1, PositionLo = 2, Speed = 3,  MaxTop = 4,  MaxBottom = 5, MoveUp = 6, MoveDown = 7 };


class DmxManager: public Manager
{
     static const int NUM_CHANNELS_PER_WINCH;
    
public:
    //! Constructor
    DmxManager();

    //! Destructor
    virtual ~DmxManager();

    //! setups DMX
    void setup();

    void update();

    void onSetPosition(int _id, float percentage);
    
    void onSetSpeed(int _id, float percentage);

    void onPanicChange(bool& value);
    
    int getNumChannelsPerWinch() const {return NUM_CHANNELS_PER_WINCH;}

private:
    
    void setupDmx();
    
    void setPanic();
    
    void exit();
    
private:
    
	ofxDmx   m_dmx;
    bool     m_panicState;
    int      m_dmxStartingChannel;
};

