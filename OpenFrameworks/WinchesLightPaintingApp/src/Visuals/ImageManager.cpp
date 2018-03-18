/*
 *  ImageManager.cpp
 *  Winches Light Painting App
 *
 *  Created by Imanol Gomez on 14/03/18.
 *
 */


#include "ofMain.h"

#include "ImageManager.h"
#include "AppManager.h"
#include "ofQTKitPlayer.h"


ImageManager::ImageManager(): Manager()
{
    //Intentionally left empty
}


ImageManager::~ImageManager()
{
    ofLogNotice() <<"ImageManager::Destructor" ;
}


void ImageManager::setup()
{
    if(m_initialized)
        return;
    
    
    Manager::setup();
    
    this->setupFbo();
    this->setupRectangle();
    this->setupImage();
    
    ofLogNotice() <<"ImageManager::initialized" ;
    
}

void ImageManager::setupRectangle()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    float h = height;
    float w = width/170;
    float x = 0;
    float y = h*0.5;
    
    m_cursor.setPosition(ofPoint(x,y));m_cursor.setCentred(true);
    m_cursor.setHeight(h); m_cursor.setWidth(w);
    //m_cursor.setColor(ofColor(255));
}

void ImageManager::setupImage()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_image.setResource("SurfaceImage");
    m_image.setWidth(width);
    m_image.setHeight(height);
    auto& texture = m_image.getTexture();
    texture.readToPixels(m_pixels);
    
    ofLogNotice() <<"ImageManager::setupImage -> w = " <<  m_pixels.getWidth() << ", h = " <<  m_pixels.getHeight();
}

void ImageManager::setupFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_fbo.allocate(width,height,GL_RGBA);
    m_fbo.begin(); ofClear(0); m_fbo.end();
}


void ImageManager::update()
{
    this->updateRectangle();
    this->updateFbo();
    this->updateColors();
}

void ImageManager::updateRectangle()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    auto pos = m_cursor.getPosition();
    pos.x = width*AppManager::getInstance().getTimeLineManager().getCurrentTime();
    m_cursor.setPosition(pos);
}

void ImageManager::updateFbo()
{
    m_fbo.begin();
        ofClear(0);
        m_image.draw();
        m_cursor.draw();
    m_fbo.end();
}

void ImageManager::updateColors()
{
    float time = AppManager::getInstance().getTimeLineManager().getCurrentTime();
    int x = (m_pixels.getWidth()-1) * time;
    
    for(int y=0; y<m_pixels.getHeight(); y++)
    {
        ofColor color = m_pixels.getColor(x,y);
        //ofColor color = ofColor::white;
        AppManager::getInstance().getLedsManager().setColor(y, color);
    }
    
}


void ImageManager::draw()
{
    this->drawFbo();
}



void ImageManager::drawFbo()
{
    m_fbo.draw(0,0);
}

