/*
 *  TimeLineManager.h
 *  Winches Light Painting App
 *
 *  Created by Imanol Gomez on 15/03/18.
 *
 */


#pragma once

#include "ofMain.h"
#include "Manager.h"
#include "ImageVisual.h"
#include "RectangleVisual.h"
#include "ofxSimpleTimer.h"


//========================== class TimeLineManager =======================================
//==============================================================================
/** \class TimeLineManager TimeLineManager.h
 *	\brief class for managing the winches position and speed
 *	\details It defines all the winches IDs, calculates positions and speed in DMX values
 *           according to the documentation
 */


class TimeLineManager: public Manager
{

public:
    //! Constructor
    TimeLineManager();

    //! Destructor
    virtual ~TimeLineManager();

    void setup();

    void update();
    
    void draw();
    
    int getNumPositions() const {return m_currentFrame;}
    
    float getCurrentTime() const {return m_time;}


private:
    
    void setupImages();
    
    void setupRectangles();
    
    void setupFbo();
    
    void updateFbo();
    
    void drawFbo();
    
    void drawImages();
    
   
private:
    
    typedef  vector<ImageVisual> ImageVector;                  ///< defines a vecotor of images

    ImageVector       m_images;
    RectangleVisual   m_cursorRectangle;
    RectangleVisual   m_timeRect;
    ofFbo             m_fbo;
    
    int              m_currentFrame;
    float            m_time;
    
};

