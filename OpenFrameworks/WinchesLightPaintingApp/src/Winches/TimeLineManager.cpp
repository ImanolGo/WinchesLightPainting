/*
 *  TimeLineManager.cpp
 *  Winches Light Painting App
 *
 *  Created by Imanol Gomez on 15/03/18.
 *
 */


#include "TimeLineManager.h"
#include "AppManager.h"



TimeLineManager::TimeLineManager(): Manager(), m_currentFrame(0),m_time(0), m_duration(60), m_segmentDuration(0.2), m_resetTime(10), m_timeWidth(100), m_margin(10), m_playMode(Stop)
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
    this->setupText();
    
}

void TimeLineManager::setupImages()
{
    auto numFrames = AppManager::getInstance().getWinchesManager().getNumPositions();
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_margin = width/50;
    m_timeWidth = width - 2*m_margin;
    float w = m_timeWidth/numFrames;
    
    for(int i=0; i<numFrames; i++)
    {
        ImageVisual image; image.setResource("Circle");
        image.setCentred(true);
        image.setWidth(w*0.5, true);
        float x = m_margin + i*w;
        float y = height*0.5;
        image.setPosition(ofPoint(x,y));
        m_images.push_back(image);
    }
   
}

void TimeLineManager::setupRectangles()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    float w = width/120;
    float h = height/8;
    float x = m_margin;
    float y = height*0.5;
    
    m_timeRect = ofPtr<RectangleVisual>(new RectangleVisual(ofPoint(x,y), w,h,true));
    
}

void TimeLineManager::setupFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_fbo.allocate(width,height,GL_RGBA);
    m_fbo.begin(); ofClear(0); m_fbo.end();
}


void TimeLineManager::setupText()
{
    float size = 20;
    float w = m_timeWidth;
    float h = size;
    float x =  m_margin;
    float y =  m_margin;
    ofPoint pos = ofPoint(x, y);
    string name = "Time";
    string text = name + ": 0.0";
    string fontName = LayoutManager::LAYOUT_FONT;
    auto textVisual = ofPtr<TextVisual>(new TextVisual(pos,w,h));
    textVisual->setText(text, fontName, size, ofColor::white);
    m_text[name] = textVisual;
    
    pos.y += (h*1.5);
    name = "Frame";
    text = name + ": 1";
    textVisual = ofPtr<TextVisual>(new TextVisual(pos,w,h));
    textVisual->setText(text, fontName, size, ofColor::white);
    m_text[name] = textVisual;
}

void TimeLineManager::update()
{
    this->updateTime();
    this->checkPlayback();
    this->updateFbo();
}

void TimeLineManager::updateTime()
{
    auto& startPos = m_images.front().getPosition().x;
    auto& endPos = m_images.back().getPosition().x;
    m_time = ofMap(m_timeRect->getPosition().x, startPos, endPos, 0.0, 1.0, true);
    string name = "Time";
    string text = name + ": " + ofToString(m_time);
    m_text[name]->setText(text);
}


void TimeLineManager::checkPlayback()
{
    if(m_playMode == Stop || m_playMode == Panic){
        return;
    }
    
    if(this->isMoving()){
        return;
    }
    
    if(m_playMode == PlayForward){
        this->playNext();
    }
    else if(m_playMode == PlayBackwards){
        this->playPrevious();
    }
    
    
}

void TimeLineManager::updateFbo()
{
    auto color = AppManager::getInstance().getSettingsManager().getColor("BackgroundColor");
    m_fbo.begin();
        ofClear(color);
        this->drawImages();
        m_timeRect->draw();
        this->drawText();
    m_fbo.end();
    
}

bool TimeLineManager::TimeLineManager::isMoving()
{
    int isMoving = false;
    if(m_moveEffect&&!m_moveEffect->isFinished())
    {
        isMoving = true;
    }
    
    return isMoving;
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

void TimeLineManager::drawText()
{
    for(auto text: m_text)
    {
        text.second->draw();
    }
}



void TimeLineManager::onDurationChange(float& value)
{
    auto numFrames = AppManager::getInstance().getWinchesManager().getNumPositions();
    
    m_duration = ofClamp(value, 10, 360);
    m_segmentDuration = m_duration/numFrames;
    
    AppManager::getInstance().getGuiManager().onSegmentDurationChange(m_segmentDuration);
}


void TimeLineManager::onSegmentDurationChange(float& value)
{
//    auto numFrames = AppManager::getInstance().getWinchesManager().getNumPositions();
//    
//    m_segmentDuration= value;
//    m_duration = m_segmentDuration*numFrames;
//    
//    AppManager::getInstance().getGuiManager().onDurationChange(m_duration);
}


void TimeLineManager::playNext()
{
    if(m_playMode == Panic){
        return;
    }
    
    auto numFrames = AppManager::getInstance().getWinchesManager().getNumPositions();
    
    if(m_currentFrame>= numFrames-1){
        m_playMode = Stop;
        return;
    }
    
    if(this->isMoving()){
        ofLogNotice() <<"TimeLineManager::playNext-> still moving!" ;
        return;
    }
    
    m_currentFrame++;
    
    this->moveToFrame(m_currentFrame, m_segmentDuration);
    ofLogNotice() <<"TimeLineManager::playNext-> frame: " << m_currentFrame ;
}

void TimeLineManager::playPrevious()
{
    if(m_playMode == Panic){
        return;
    }
    
    auto numFrames = AppManager::getInstance().getWinchesManager().getNumPositions();
    
    if(m_currentFrame<= 0){
        m_playMode = Stop;
        return;
    }
    
    if(this->isMoving()){
        ofLogNotice() <<"TimeLineManager::playPrevious-> still moving!" ;
        return;
    }
    
    m_currentFrame--;
    
    this->moveToFrame(m_currentFrame, m_segmentDuration);
    ofLogNotice() <<"TimeLineManager::playPrevious-> frame: " << m_currentFrame ;
}



void TimeLineManager::moveToFrame(int frame, float duration)
{
    if(m_playMode == Panic){
        return;
    }
    
    if(frame<0 || frame>=m_images.size()){
        return;
    }
    
    string name = "Frame";
    string text = name + ": " + ofToString(frame+1);
    m_text[name]->setText(text);
    
    AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(m_timeRect);
    
    EffectSettings settings;
    settings.function = LINEAR; settings.type = EASE_OUT;
    settings.startAnimation = 0; settings.animationTime = duration;
    ofPoint pos = m_images[frame].getPosition();
    m_moveEffect = AppManager::getInstance().getVisualEffectsManager().createMoveEffect(m_timeRect, pos, settings);
    
    m_currentFrame = frame;
    
}

void TimeLineManager::reset()
{
    if(m_playMode == Panic){
        return;
    }
    
    m_playMode = Stop;
    this->moveToFrame(0,m_resetTime);
}

void TimeLineManager::setPanic(bool value)
{
    if(value){
         m_playMode = Panic;
         AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(m_timeRect);
         m_currentFrame = 0;
        
        string name = "Frame";
        string text = name + ": " + ofToString(m_currentFrame+1);
        m_text[name]->setText(text);
        m_time = 0.0;
        m_timeRect->setPosition(m_images.front().getPosition());
        
    }
    else{
        m_playMode = Stop;
    }
}


void TimeLineManager::playForward()
{
    if(m_playMode == Panic){
        return;
    }
    
    m_playMode = PlayForward;
    
}

void TimeLineManager::playBackwards()
{
    if(m_playMode == Panic){
        return;
    }
    
    m_playMode = PlayBackwards;
}

void TimeLineManager::stop()
{
    if(m_playMode == Panic){
        return;
    }
    
    m_playMode = Stop;
    
}



