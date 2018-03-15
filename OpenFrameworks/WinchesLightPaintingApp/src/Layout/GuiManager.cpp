/*
 *  GuiManager.cpp
 *  Winches Light Painting App
 *
 *  Created by Imanol Gomez on 14/03/18.
 *
 */


#include "ofMain.h"

#include "AppManager.h"

#include "GuiManager.h"
#include "GuiTheme.h"


const string GuiManager::GUI_SETTINGS_FILE_NAME = "xmls/GuiSettings.xml";
const string GuiManager::GUI_SETTINGS_NAME = "GUI";
const int GuiManager::GUI_WIDTH = 350;


GuiManager::GuiManager(): Manager(), m_showGui(true)
{
    //Intentionally left empty
}


GuiManager::~GuiManager()
{
    this->saveGuiValues();
    ofLogNotice() <<"GuiManager::Destructor";
}


void GuiManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    
    this->setupGuiParameters();
    this->setupTimeLineParameters();
    this->setupDmxGui();
    this->setupGuiEvents();
    this->loadGuiValues();

    
    ofLogNotice() <<"GuiManager::initialized";
    
}

void GuiManager::setupGuiParameters()
{
    m_gui.setPosition(ofxDatGuiAnchor::TOP_LEFT);
    m_gui.setTheme(new GuiTheme());
    
    
    int margin =  LayoutManager::MARGIN;
    m_gui.setAutoDraw(false);
    auto pos = m_gui.getPosition();
    m_gui.setPosition(pos.x + margin, pos.y + margin);
    m_gui.addHeader(GUI_SETTINGS_NAME, true);
    
    // add some components //
    //m_gui.addLabel("PrimaveraSound GUI");
    
    m_gui.addFRM();
    m_gui.addButton("* Save GUI");
    
    m_gui.addBreak();
    
}


void GuiManager::setupTimeLineParameters()
{
    auto timeLineManager = &AppManager::getInstance().getTimeLineManager();
    
    m_timeLineDuration.set("Duration", 30, 30, 360);
    m_timeLineDuration.addListener(timeLineManager, &TimeLineManager::onDurationChange);
    m_parameters.add(m_timeLineDuration);
    
    m_timeLineSegmentDuration.set("Segment Duration", 2, 0, 30);
    m_timeLineDuration.addListener(timeLineManager, &TimeLineManager::onSegmentDurationChange);
    m_parameters.add(m_timeLineDuration);
    
    // add a folder to group a few components together //
    ofxDatGuiFolder* folder = m_gui.addFolder("TIMELINE", ofColor::greenYellow);
    
    folder->addSlider(m_timeLineDuration);
    folder->addSlider(m_timeLineSegmentDuration);
    folder->addButton("* PlayF");
    folder->addButton("* PlayB");
    folder->addButton("* Stop");
    folder->addButton("* Next");
    folder->addButton("* Previous");
    folder->addButton("* Reset");
    folder->expand();
}

void GuiManager::setupDmxGui()
{
    auto dmxManager = &AppManager::getInstance().getDmxManager();
    auto numWinches = AppManager::getInstance().getWinchesManager().getNumWinches();
    auto numChannelsPerWinch = dmxManager->getNumChannelsPerWinch();
    
    // add a folder to group a few components together //
    ofxDatGuiFolder* folder = m_gui.addFolder("DMX", ofColor::cyan);
    
    folder->addButton("* PANIC!!!!");
    
    //m_panicToggle.set("Toggle Panic", false);
    //m_panicToggle.addListener(dmxManager, &DmxManager::onPanicChange);
    folder->addToggle("Toggle Panic");

    m_dmxWinch.set("Winch", 0, 0, numWinches-1);
    folder->addSlider(m_dmxWinch);
    
    m_dmxInitChannel.set("InitChannel", 1, 1, 64);
    folder->addSlider(m_dmxInitChannel);
    
    for(int i=0; i<numChannelsPerWinch; i++){
        ofParameter<int> parameter;
        string name = "Channel " + ofToString(i+1);
        parameter.set(name, 0, 0, 255);
        folder->addSlider(parameter);
    }
    
    folder->expand();
    m_gui.addBreak();
}


void GuiManager::draw()
{
    if(!m_showGui)
        return;
    
    this->drawRectangle();
    
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    m_gui.draw();
    ofDisableAlphaBlending();
    ofDisableSmoothing();
}

void GuiManager::update()
{
    m_gui.update();
}


void GuiManager::saveGuiValues()
{
    ofXml xml;
    xml.serialize(m_parameters);
    xml.save(GUI_SETTINGS_FILE_NAME);
}

void GuiManager::loadGuiValues()
{
    ofXml xml(GUI_SETTINGS_FILE_NAME);
    xml.deserialize(m_parameters);
}


void GuiManager::toggleGui()
{
    m_showGui = !m_showGui;
}

void GuiManager::drawRectangle()
{
    int margin =  LayoutManager::MARGIN;
    ofPushStyle();
    ofSetColor(15);
    ofDrawRectangle( m_gui.getPosition().x - margin, 0, m_gui.getWidth() + 2*margin, ofGetHeight());
    ofPopStyle();
}


void GuiManager::setupGuiEvents()
{
    m_gui.onDropdownEvent(this, &GuiManager::onDropdownEvent);
    m_gui.onColorPickerEvent(this, &GuiManager::onColorPickerEvent);
    m_gui.onButtonEvent(this, &GuiManager::onButtonEvent);
    m_gui.onToggleEvent(this, &GuiManager::onToggleEvent);
    m_gui.onMatrixEvent(this, &GuiManager::onMatrixEvent);
}

void GuiManager::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    cout << "onDropdownEvent: " << e.target->getName() << " Selected" << endl;
    
}

void GuiManager::onColorPickerEvent(ofxDatGuiColorPickerEvent e)
{
    cout << "onColorPickerEvent: " << e.target->getName() << " Selected" << endl;
}

void GuiManager::onButtonEvent(ofxDatGuiButtonEvent e)
{
    cout << "onButtonEvent: " << e.target->getName() << " Selected" << endl;
    
    if(e.target->getName() == "* PANIC!!!!")
    {
        this->onPanicToggleChange(true);
    }
    else if(e.target->getName() == "* Next")
    {
        AppManager::getInstance().getTimeLineManager().playNext();
    }
    else if(e.target->getName() == "* Previous")
    {
        AppManager::getInstance().getTimeLineManager().playPrevious();
    }
    else if(e.target->getName() == "* Reset")
    {
        AppManager::getInstance().getTimeLineManager().reset();
    }
    
    else if(e.target->getName() == "* PlayF")
    {
        AppManager::getInstance().getTimeLineManager().playForward();
    }
    
    else if(e.target->getName() == "* PlayB")
    {
        AppManager::getInstance().getTimeLineManager().playBackwards();
    }
    else if(e.target->getName() == "* Stop")
    {
        AppManager::getInstance().getTimeLineManager().stop();
    }
}


void GuiManager::onToggleEvent(ofxDatGuiToggleEvent e)
{
    cout << "onToggleEvent: " << e.target->getName() << " Selected" << endl;
    
    if(e.target->getName() == "Toggle Panic")
    {
        this->onPanicToggleChange(e.target->getChecked());
    }
}

void GuiManager::onMatrixEvent(ofxDatGuiMatrixEvent e)
{
    cout << "onMatrixEvent " << e.child << " : " << e.enabled << endl;
}


void GuiManager::onDurationChange(float& value)
{
    m_timeLineDuration = ofClamp(value, m_timeLineDuration.getMin(),  m_timeLineDuration.getMax());
}

void GuiManager::onSegmentDurationChange(float& value)
{
    m_timeLineSegmentDuration = ofClamp(value, m_timeLineSegmentDuration.getMin(),  m_timeLineSegmentDuration.getMax());
}


