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

public:
    //! Constructor
    WinchesManager();

    //! Destructor
    virtual ~WinchesManager();

    //! setups DMX
    void setup();

    void update();

    int getNumWinches() const {return NUM_WINCHES;}
    


private:
    
   
private:
    
	

};

