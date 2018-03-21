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
    this->setupImagesDropDown();
    this->setupImageGui();
    this->setupWinchesGui();
    this->setupTimeLineGui();
    this->setupDmxGui();
    this->setupGuiEvents();
    this->loadGuiValues();

    
    ofLogNotice() <<"GuiManager::initialized";
    
}

void GuiManager::setupGuiParameters()
{
    ofxDatGuiLog::quiet();
    
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


void GuiManager::setupImagesDropDown()
{
    string label = "IMAGES:";
    vector<string> opts;
    auto images = AppManager::getInstance().getImageManager().getImages();
    for(auto image: images){
        opts.push_back(image.first);
    }
    
    m_gui.addDropdown(label, opts);
    auto menu = m_gui.getDropdown(label);
    menu->expand(); //let's have it open by default
    menu->setStripeColor(ofColor::orange);
    for (int i=0; i<menu->size(); i++) menu->getChildAt(i)->setStripeColor(ofColor::yellow);
    m_gui.addBreak();
  
}

void GuiManager::setupImageGui()
{
    auto imageManager = &AppManager::getInstance().getImageManager();
    m_brightness.set("Brightness", 1.0 , 0.0, 1.0);
    m_brightness.addListener(imageManager, &ImageManager::onBrightnessChange);
    m_parameters.add(m_brightness);
    
    m_imageRate.set("ScaleTime", 0.0 , 0.0, 1.0);
    m_imageRate.addListener(imageManager, &ImageManager::onRateChange);
    m_parameters.add(m_imageRate);
    
    
    // add a folder to group a few components together //
    ofxDatGuiFolder* folder = m_gui.addFolder("IMAGE", ofColor::purple);
    folder->addSlider(m_brightness);
    folder->addSlider(m_imageRate);
    folder->expand();
    
    m_gui.addBreak();
}

void GuiManager::setupWinchesGui()
{
    auto winchesManager = &AppManager::getInstance().getWinchesManager();
    m_offset.set("Offset (m)", 0.0 , 0.0, 10.0);
    m_offset.addListener(winchesManager, &WinchesManager::onOffsetChange);
    m_parameters.add(m_offset);
    
    m_speed.set("Speed (100%)", 100.0 , 0.0, 100.0);
    m_speed.addListener(winchesManager, &WinchesManager::onSpeedChange);
    m_parameters.add(m_speed);
    
    
    
    // add a folder to group a few components together //
    ofxDatGuiFolder* folder = m_gui.addFolder("WINCHES", ofColor::green);
    folder->addSlider(m_offset);
    folder->addSlider(m_speed);
    folder->expand();
    
    m_gui.addBreak();
}


void GuiManager::setupTimeLineGui()
{
    auto timeLineManager = &AppManager::getInstance().getTimeLineManager();
    
    m_timeLineDuration.set("Duration", 10, 0, 60);
    m_timeLineDuration.addListener(timeLineManager, &TimeLineManager::onDurationChange);
    m_parameters.add(m_timeLineDuration);
    
    m_timeLineSegmentDuration.set("Segment Duration", 2, 0, 5);
    m_timeLineDuration.addListener(timeLineManager, &TimeLineManager::onSegmentDurationChange);
    m_parameters.add(m_timeLineDuration);
    
    m_timeLineResetDuration.set("Reset Duration", 2, 10, 30);
    m_timeLineResetDuration.addListener(timeLineManager, &TimeLineManager::onResetDurationChange);
    m_parameters.add(m_timeLineResetDuration);
    
    // add a folder to group a few components together //
    ofxDatGuiFolder* folder = m_gui.addFolder("TIMELINE", ofColor::greenYellow);
    
    folder->addSlider(m_timeLineDuration);
    folder->addSlider(m_timeLineSegmentDuration);
    folder->addSlider(m_timeLineResetDuration);
    folder->addButton("* PlayF");
    folder->addButton("* PlayB");
    folder->addButton("* Stop");
    folder->addButton("* Next");
    folder->addButton("* Previous");
    folder->addButton("* Reset");
    folder->expand();
    
    m_gui.addBreak();
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
    m_dmxWinch.addListener(this, &GuiManager::onDmxWinchChange);
    folder->addSlider(m_dmxWinch);
    
    m_dmxInitChannel.set("InitChannel", 1, 1, 64);
    folder->addSlider(m_dmxInitChannel);
    
    m_channel1.set("Channel1", 0, 0, 255);
    m_channel1.addListener(this, &GuiManager::onDmxChannel1Change);
    folder->addSlider(m_channel1);
    
    m_channel2.set("Channel2", 0, 0, 255);
    m_channel2.addListener(this, &GuiManager::onDmxChannel2Change);
    folder->addSlider(m_channel2);
    
    m_channel3.set("Channel3", 0, 0, 255);
    m_channel3.addListener(this, &GuiManager::onDmxChannel3Change);
    folder->addSlider(m_channel3);
    
    m_channel4.set("Channel4", 0, 0, 255);
    m_channel4.addListener(this, &GuiManager::onDmxChannel4Change);
    folder->addSlider(m_channel4);
    
    m_channel5.set("Channel5", 0, 0, 255);
    m_channel5.addListener(this, &GuiManager::onDmxChannel5Change);
    folder->addSlider(m_channel5);
    
    m_channel6.set("Channel6", 0, 0, 255);
    m_channel6.addListener(this, &GuiManager::onDmxChannel6Change);
    folder->addSlider(m_channel6);
    
    m_channel7.set("Channel7", 0, 0, 255);
    m_channel7.addListener(this, &GuiManager::onDmxChannel7Change);
    folder->addSlider(m_channel7);
  
    
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
    
    if(e.target->getName() == "IMAGES:")
    {
        AppManager::getInstance().getImageManager().setImage(e.target->getLabel());
        m_gui.getDropdown(e.target->getName())->setLabel("FONT: " + e.target->getLabel());
    }
    
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
    
    else if(e.target->getName() == "* Save GUI")
    {
        this->saveGuiValues();
    }
}


void GuiManager::onToggleEvent(ofxDatGuiToggleEvent e)
{
    cout << "onToggleEvent: " << e.target->getName() << " selected" << endl;
    
    if(e.target->getName() == "Toggle Panic")
    {
        this->onPanicToggleChange(e.target->getChecked());
    }
}


void  GuiManager::onPanicToggleChange(bool value)
{
    ofxDatGuiToggle* toggle = m_gui.getToggle("Toggle Panic");
    if(toggle->getChecked()!=value){
        toggle->setChecked(value);
    }
    
    AppManager::getInstance().getTimeLineManager().setPanic(value);
    AppManager::getInstance().getDmxManager().onPanicChange(value);
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

void GuiManager::onDmxWinchChange(int& value)
{
    auto num = AppManager::getInstance().getDmxManager().getNumChannelsPerWinch();
    m_dmxInitChannel = 1 + value*num;
}

void GuiManager::onDmxChannel1Change(int& value)
{
    AppManager::getInstance().getDmxManager().onSendDmx(m_dmxInitChannel, value);
}

void GuiManager::onDmxChannel2Change(int& value)
{
    AppManager::getInstance().getDmxManager().onSendDmx(m_dmxInitChannel+1, value);
}

void GuiManager::onDmxChannel3Change(int& value)
{
    AppManager::getInstance().getDmxManager().onSendDmx(m_dmxInitChannel+2, value);
}


void GuiManager::onDmxChannel4Change(int& value)
{
    AppManager::getInstance().getDmxManager().onSendDmx(m_dmxInitChannel+3, value);
}


void GuiManager::onDmxChannel5Change(int& value)
{
    AppManager::getInstance().getDmxManager().onSendDmx(m_dmxInitChannel+4, value);
}

void GuiManager::onDmxChannel6Change(int& value)
{
    AppManager::getInstance().getDmxManager().onSendDmx(m_dmxInitChannel+5, value);
}

void GuiManager::onDmxChannel7Change(int& value)
{
    AppManager::getInstance().getDmxManager().onSendDmx(m_dmxInitChannel+6, value);
}



