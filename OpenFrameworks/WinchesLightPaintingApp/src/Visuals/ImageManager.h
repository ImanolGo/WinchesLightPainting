/*
 *  ImageManager.h
 *  Winches Light Painting App
 *
 *  Created by Imanol Gomez on 14/03/18.
 *
 */



#pragma once

#include "Manager.h"
#include "ImageVisual.h"
#include "RectangleVisual.h"


//========================== class ImageManager ==============================
//============================================================================
/** \class ImageManager ImageManager.h
 *	\brief Class managing the video player visuals
 *	\details it creates and updates the video player
 */

class ImageManager: public Manager
{

public:
    
    //! Constructor
    ImageManager();
    
    //! Destructor
    ~ImageManager();
    
    //! Setup the Video Manager
    void setup();
    
    //! Update the Video Manager
    void update();
    
    //! Draw the Video Manager
    void draw();
    
    int getHeight() const {return m_image.getOriginalHeight();}
    
    void onBrightnessChange(int& value);
    
private:
    
    void setupFbo();
    
    void setupImage();
    
    void setupRectangle();
    
    void updateFbo();
    
    void updateRectangle();
    
    void updateColors();
    
    void drawFbo();
    
private:
    
    ofFbo           m_fbo;
    ImageVisual     m_image;
    RectangleVisual m_cursor;
    ofPixels        m_pixels;
    int             m_brightness;
    
};



