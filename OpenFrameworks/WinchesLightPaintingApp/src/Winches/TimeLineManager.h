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
#include "VisualEffects.h"
#include "TextVisual.h"


//========================== class TimeLineManager =======================================
//==============================================================================
/** \class TimeLineManager TimeLineManager.h
 *	\brief class for managing the winches position and speed
 *	\details It defines all the winches IDs, calculates positions and speed in DMX values
 *           according to the documentation
 */


class TimeLineManager: public Manager
{
    enum PlaybackMode { Stop, PlayForward, PlayBackwards, Panic};

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
    
    void onDurationChange(float& value);
    
    void onSegmentDurationChange(float& value);
    
    void onResetDurationChange(float& value);
    
    void playNext();
    
    void playForward();
    
    void playBackwards();
    
    void setPanic(bool value);
    
    void playPrevious();
    
    void reset();
    
    void stop();
    
private:
    
    void setupImages();
    
    void setupRectangles();
    
    void setupText();
    
    void setupFbo();
    
    void updateFbo();
    
    void updateTime();
    
    void checkPlayback();
    
    bool isMoving();
    
    void drawFbo();
    
    void drawImages();
    
    void drawText();
    
    void moveToFrame(int frame, float duration);
    
    void stopIfNotMoving();
   
private:
    
    typedef  map<string, ofPtr<TextVisual> >      TextMap;            ///< defines a map of Text attached to an identifier
    typedef  vector<ImageVisual> ImageVector;                  ///< defines a vecotor of images

    ImageVector                 m_images;
    TextMap                     m_text;
    ofPtr<RectangleVisual>      m_timeRect;
    ofFbo                       m_fbo;
    ofPtr<MoveVisual>           m_moveEffect;
    float                       m_duration;
    float                       m_segmentDuration;
    float                       m_resetTime;
    float                       m_timeWidth;
    float                       m_margin;
    
    int                         m_currentFrame;
    float                       m_time;
    PlaybackMode                m_playMode;
    
    
};

