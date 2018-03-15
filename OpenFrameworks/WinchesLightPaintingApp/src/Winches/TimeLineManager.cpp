/*
 *  TimeLineManager.cpp
 *  Winches Light Painting App
 *
 *  Created by Imanol Gomez on 15/03/18.
 *
 */


#include "TimeLineManager.h"
#include "AppManager.h"


TimeLineManager::TimeLineManager(): Manager(), m_currentFrame(0),m_time(0)
{
    //Intentionally left empty
}

TimeLineManager::~TimeLineManager()
{
   ofLogNotice() << "TimeLineManager::destructor";
}


//--------------------------------------------------------------

void TimeLineManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    ofLogNotice() <<"TimeLineManager::initialized" ;
    
    this->setupFbo();
    this->setupImages();
    this->setupRectangles();
    
  
}

void TimeLineManager::setupImages()
{
    auto numFrames = AppManager::getInstance().getWinchesManager().getNumPositions();
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    float margin = width/50;
    float totalWidth = width - 2*margin;
    float w = totalWidth/numFrames;
    
    for(int i=0; i<numFrames; i++)
    {
        ImageVisual image; image.setResource("Circle");
        image.setCentred(true);
        image.setWidth(w*0.5, true);
        float x = margin + i*w;
        float y = height*0.5;
        image.setPosition(ofPoint(x,y));
        m_images.push_back(image);
    }
   
}

void TimeLineManager::setupRectangles()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    float margin = width/50;
    float w = width - 2*margin;
    float h = height/100;
    float x = width*0.5;
    float y = height*0.5;
    
    m_timeRect.setCentred(true);
    m_timeRect.setWidth(w); m_timeRect.setHeight(h);
    m_timeRect.setPosition(ofPoint(x,y));
    
    
}

void TimeLineManager::setupFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_fbo.allocate(width,height,GL_RGBA);
    m_fbo.begin(); ofClear(0); m_fbo.end();
}



void TimeLineManager::update()
{
    this->updateFbo();
}

void TimeLineManager::updateFbo()
{
    auto color = AppManager::getInstance().getSettingsManager().getColor("BackgroundColor");
    m_fbo.begin();
        ofClear(color);
        //m_timeRect.draw();
        this->drawImages();
    m_fbo.end();
    
}


void TimeLineManager::draw()
{
    this->drawFbo();
}

void TimeLineManager::drawFbo()
{
    m_fbo.draw(0,0);
}

void TimeLineManager::drawImages()
{
    for(auto image: m_images)
    {
        image.draw();
    }
}


