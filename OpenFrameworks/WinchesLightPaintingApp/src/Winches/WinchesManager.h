/*
 *  WinchesManager.h
 *  Winches Light Painting App
 *
 *  Created by Imanol Gomez on 15/03/18.
 *
 */


#pragma once

#include "ofMain.h"
#include "Manager.h"
#include "ofxCsv.h"
#include "Winch.h"


//========================== class WinchesManager =======================================
//==============================================================================
/** \class WinchesManager WinchesManager.h
 *	\brief class for managing the winches position and speed
 *	\details It defines all the winches IDs, calculates positions and speed in DMX values
 *           according to the documentation
 */


class WinchesManager: public Manager
{
    static const int NUM_WINCHES;
    static const string POSITIONS_DATA_PATH;

public:
    //! Constructor
    WinchesManager();

    //! Destructor
    virtual ~WinchesManager();

    //! setup
    void setup();

    void update();
    
    void draw();

    int getNumWinches() const {return NUM_WINCHES;}
    
    int getNumPositions() const {return m_numPositions;}
    
    void setFrame(int index, float time);
    
    void stop();
    
    void reset();
    
    void onOffsetChange(float& value) {m_offset = value;}
    
    void onSpeedChange(float& value) {m_speed = value;}
    
    const ofVec2f& getDistanceRange() const {return m_distanceRange;}

private:
    
    bool readCsv();
    
    void initializePositions();
    
    void initializeRanges();
    
    void loadPositions();
    
    void setupWinches();
    
    void updateWinches();
    
    void drawWinches();
    
    float getSpeed(float currentPos, float prevPos, float time);
    
    void setWinch(int _id, float currentPos, float prevPos, float time);
    
    void sendDmx(int _id, float currentPos, float prevPos, float time);
    
    void startAnimation(int _id, float currentPos, float time );
    
private:
    
    typedef  vector<float>  PositionsVector;                ///< defines a vecotor of winch positions
    typedef  vector<string> FileNames;                ///< defines a vecotor of winch positions
    typedef  map<int, PositionsVector  >    PositionsMap;     ///< defines a map of PositionsVector attached to an ID
    typedef  map<int, ofPtr<Winch>  >       WinchMap;     ///< defines a map of Winch Smart Pointers
    
    PositionsMap     m_positions;
    ofxCsv           m_csv;
    int              m_numPositions;
    WinchMap         m_winches;
    int              m_previousFrame;
    ofVec2f          m_speedRange;
    ofVec2f          m_distanceRange;
    float            m_offset;
    float            m_speed;


};

