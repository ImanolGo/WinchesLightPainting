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
    this->setupImage();
    
    ofLogNotice() <<"ImageManager::initialized" ;
    
}


void ImageManager::setupImage()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_image.setResource("Abstract");
    m_image.setWidth(width);
    m_image.setHeight(height);
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
    this->updateFbo();
}

void ImageManager::updateFbo()
{
    m_fbo.begin();
        ofClear(0);
        m_image.draw();
    m_fbo.end();
}



void ImageManager::draw()
{
    this->drawFbo();
}



void ImageManager::drawFbo()
{
    m_fbo.draw(0,0);
}


