/*
 *  GuiManager.h
 *  Winches Light Painting App
 *
 *  Created by Imanol Gomez on 14/03/18.
 *
 */


#pragma once

#include "Manager.h"
#include "ofxDatGui.h"

//========================== class GuiManager ==============================
//============================================================================
/** \class GuiManager GuiManager.h
 *	\brief Class managing the application«s grapical user interface (GUI)
 *	\details It creates the gui and the callback functions that will be affected
 *    by the gui
 */

class GuiManager: public Manager
{
    static const string GUI_SETTINGS_FILE_NAME;
    static const string GUI_SETTINGS_NAME;
    static const int GUI_WIDTH;
    
public:
    
    //! Constructor
    GuiManager();
    
    //! Destructor
    ~GuiManager();
    
    //! Set-up the gui
    void setup();
    
    //! Draw the gui
    void draw();
    
    void update();
    
    void saveGuiValues();
    
    void loadGuiValues();
    
    void toggleGui();
    
    void showGui(bool show){m_showGui=show;}
    
    int getWidth() {return GUI_WIDTH;}
    
    int getHeight() {return m_gui.getHeight();}
    
    ofPoint  getPosition() {return m_gui.getPosition();}
    
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    
    void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
    
    void onButtonEvent(ofxDatGuiButtonEvent e);
    
    void onToggleEvent(ofxDatGuiToggleEvent e);
    
    void onMatrixEvent(ofxDatGuiMatrixEvent e);
    
    void onPanicToggleChange(bool value);
    
    void onDurationChange(float& value);
    
    void onSegmentDurationChange(float& value);
    
    void onDmxWinchChange(int& value);
    
    void onDmxChannel1Change(int& value);
    
    void onDmxChannel2Change(int& value);
    
    void onDmxChannel3Change(int& value);
    
    void onDmxChannel4Change(int& value);
    
    void onDmxChannel5Change(int& value);
    
    void onDmxChannel6Change(int& value);
    
    void onDmxChannel7Change(int& value);

private:
    
    void setupGuiParameters();
    
    void setupGuiEvents();
    
    void setupImageGui();
    
    void setupImagesDropDown();
    
    void setupWinchesGui();
    
    void setupTimeLineGui();
    
    void setupDmxGui();
    
    void drawRectangle();
    
    
private:
    
    ofxDatGui			m_gui;
    
    ofParameter<float>	m_guiFPS;
    
    
    ofParameterGroup        m_parameters;
    
    ofParameter<float>        m_brightness;
    ofParameter<float>        m_imageRate;
    
    ofParameter<float>      m_offset;
    ofParameter<float>      m_speed;
    
    ofParameter<float> m_timeLineDuration;
    ofParameter<float> m_timeLineSegmentDuration;
    ofParameter<float> m_timeLineResetDuration;
    
    ofParameter<bool>   m_panicToggle;
    ofParameter<int>    m_dmxWinch;
    ofParameter<int>    m_dmxInitChannel;
    ofParameter<int>    m_channel1;
    ofParameter<int>    m_channel2;
    ofParameter<int>    m_channel3;
    ofParameter<int>    m_channel4;
    ofParameter<int>    m_channel5;
    ofParameter<int>    m_channel6;
    ofParameter<int>    m_channel7;
    //vector<ofParameter<int>>    m_dmxChannels;
    
    
    bool        m_showGui;  //It defines the whether the gui should be shown or not
};

//==========================================================================

