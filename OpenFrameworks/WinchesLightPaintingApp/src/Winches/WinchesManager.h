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

    int getNumWinches() const {return NUM_WINCHES;}
    
    int getNumPositions() const {return m_numPositions;}


private:
    
    bool readCsv();
    
    void initializePositions();
    
    void loadPositions();
    
   
private:
    
    typedef  vector<float> PositionsVector;                ///< defines a vecotor of winch positions
    typedef  map<int, PositionsVector  >    PositionsMap;     ///< defines a map of PositionsVector attached to an ID
    
    PositionsMap     m_positions;
    ofxCsv           m_csv;
    int              m_numPositions;

};

