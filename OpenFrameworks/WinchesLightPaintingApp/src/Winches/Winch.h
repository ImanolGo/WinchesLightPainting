/*
 *  Winch.h
 *  Winches Light Painting App
 *
 *  Created by Imanol Gomez on 16/03/18.
 *
 */

#pragma once

#include "ofMain.h"
#include "BasicVisual.h"
#include "TextVisual.h"
#include "ImageVisual.h"
#include "RectangleVisual.h"


class Winch: public BasicVisual {
    
public:
    
    Winch(const ofPoint& position, float width, float height, int _id);
    
    virtual ~Winch();
    
    void update();
    
    void draw();
    
private:
    
    void setup();
    
    void setupImages();
    
    void setupText();
    
    void setupRectangles();
    
    void setParameters(float distance, float speed);
    
    void updateText();
    
    void drawRectangles();
    
    void drawImages();
    
    void drawText();
    
    void setId(int value) {m_id = value;}
    
    int getId() const {return m_id;}
    
    
private:
    
    
    typedef  map< string, RectangleVisual> RectanglesMap;
    typedef  map< string,  TextVisual>    TextsMap;
    
    RectanglesMap       m_rectangles;
    TextsMap            m_text;
    ImageVisual         m_image;
    
    float               m_distance;
    float               m_speed;
    int                 m_id;
};




