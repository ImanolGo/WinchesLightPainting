/*
 *  Winch.cpp
 *  Winches Light Painting App
 *
 *  Created by Imanol Gomez on 16/03/18.
 *
 */


#include "Winch.h"
#include "AppManager.h"


Winch::Winch(const ofPoint& position, float width, float height, int _id): BasicVisual(position, width, height), m_id(_id)
{
    this->setup();
}

Winch::~Winch()
{
    //Intentionaly left empty
}

void Winch::setup()
{
    this->setupImages();
    this->setupRectangles();
    this->setupText();
}


void Winch::setupImages()
{
    ofVec3f position(0,0);
    m_image.setResource("Circle");
    m_image.setCentred(true);
    m_image.setWidth(m_width*0.15, true);
    float x = m_width*0.5;
    float y = m_height*0.5;
    m_image.setPosition(ofPoint(x,y));
}

void Winch::setupText()
{
    float margin = m_width/10;
    float fontSize = margin;
    string text = ofToString(m_id);
    string fontName ="fonts/open-sans/OpenSans-Bold.ttf";
    ofColor textColor = ofColor::white;
    float w = m_width - 2*margin;
    float h = fontSize;
    
    m_text["id"] = TextVisual(m_image.getPosition(), w, h, true);
    m_text["id"].setText(text, fontName, fontSize, textColor);
    m_text["id"].setColor(ofColor(0));
    
    
    fontSize = m_rectangles["bottom"].getHeight()/4;
    h = fontSize*2;
    w = m_width - 2*margin;
    text = "Distance: 0.0m\nSpeed: 0.0m/s";
    auto pos = m_rectangles["bottom"].getPosition();
    pos.x +=  m_rectangles["bottom"].getWidth()*0.5;
    pos.y += m_rectangles["bottom"].getHeight()*0.5;
    m_text["bottom"] = TextVisual(pos, w, h, true);
    m_text["bottom"].setLineHeight(2.0);
    m_text["bottom"].setText(text, fontName, fontSize, textColor);
}

void Winch::setupRectangles()
{
    RectangleVisual rect;
    rect.setWidth(m_width*0.25); rect.setHeight(m_width*0.05);
    float x = m_width*0.5 - rect.getWidth()*0.5; float y = 0;
    rect.setPosition(ofPoint(x,y));
    m_rectangles["top"] = rect;

    rect.setWidth(m_width*0.01); rect.setHeight(m_image.getPosition().y);
    x =m_width*0.5  - rect.getWidth()*0.5; y = 0;
    rect.setPosition(ofPoint(x,y));
    m_rectangles["string"] = rect;
    
    auto color = AppManager::getInstance().getSettingsManager().getColor("EdgeBackgroundColor");
    rect.setWidth(m_width); rect.setHeight(m_height*0.15);
    x = 0; y = m_height-rect.getHeight();
    rect.setPosition(ofPoint(x,y));
    rect.setColor(color);
    m_rectangles["bottom"] = rect;
    
}

void Winch::draw()
{
    ofEnableAlphaBlending();
    ofPushMatrix();
    
    ofTranslate(m_position);
    ofScale(m_scale.x, m_scale.y);
    //ofTranslate(-m_width*0.5,-m_width*0.5);
    
    ofRotateX(m_rotation.x);
    ofRotateY(m_rotation.y);
    
        this->drawRectangles();
        this->drawImages();
        this->drawText();
    
    ofPopMatrix();
    ofDisableAlphaBlending();
}



void Winch::update()
{
    this->updateText();
}


void Winch::setParameters(float distance, float speed)
{
    
}

void Winch::drawRectangles()
{
    for(auto& rect: m_rectangles){
        rect.second.draw();
    }
    
}

void Winch::drawImages()
{
    m_image.draw();
}

void Winch::drawText()
{
    for(auto& text: m_text){
        text.second.draw();
    }
    
}


void Winch::updateText()
{
    
}





