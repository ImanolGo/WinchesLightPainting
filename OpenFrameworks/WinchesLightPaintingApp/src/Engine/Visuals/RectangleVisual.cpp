/*
 *  RectangleVisual.cpp
 *
 *  Created by Imanol G—mez on 16/02/15.
 *
 */


#include "AppManager.h"
#include "ResourceManager.h"

#include "RectangleVisual.h"


RectangleVisual::RectangleVisual(): BasicVisual(), m_centred(false)
{
    //Intentionally left empty
}


RectangleVisual::RectangleVisual(const ofVec3f& pos, float width, float height, bool centred):
    BasicVisual(pos, width, height),  m_centred(centred)
{
    //Intentionally left empty
}

RectangleVisual::~RectangleVisual()
{
    //Intentionally left empty
}


void RectangleVisual::draw()
{

    ofPushMatrix();
    ofPushStyle();
    
        ofSetColor(m_color);

        ofTranslate(m_position);
    

        if(m_centred){
            ofSetRectMode(OF_RECTMODE_CENTER);
        }
        else{
            ofSetRectMode(OF_RECTMODE_CORNER);
        }

        ofRotateX(m_rotation.x);
        ofRotateY(m_rotation.y);
        ofRotateZ(m_rotation.z);
    
        ofScale(m_scale.x, m_scale.y);
    
        ofDrawRectangle(0, 0, m_width, m_height);

    ofPopStyle();
    ofPopMatrix();
}


