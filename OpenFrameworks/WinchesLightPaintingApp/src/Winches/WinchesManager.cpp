/*
 *  WinchesManager.cpp
 *  Winches Light Painting App
 *
 *  Created by Imanol Gomez on 15/03/18.
 *
 */


#include "WinchesManager.h"
#include "AppManager.h"


const int WinchesManager::NUM_WINCHES = 6;

const string WinchesManager::POSITIONS_DATA_PATH = "positions/winch_data5.csv";


WinchesManager::WinchesManager(): Manager(), m_numPositions(0), m_previousFrame(-1), m_offset(0.0), m_speed(100)
{
    //Intentionally left empty
}

WinchesManager::~WinchesManager()
{
   ofLogNotice() << "WinchesManager::destructor";
}


//--------------------------------------------------------------

void WinchesManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    ofLogNotice() <<"WinchesManager::initialized" ;
    
    this->setupWinches();
    this->initializePositions();
    this->initializeRanges();
    this->loadFiles();
    
    if(this->readCsv())
    {
        this->loadPositions();
    }
}

void WinchesManager::setupWinches()
{
    float w = AppManager::getInstance().getSettingsManager().getAppWidth()/NUM_WINCHES;
    float h = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    for (int i=0; i<NUM_WINCHES; i++)
    {
        float x = i*w;
        float y = 0;
        ofPtr<Winch> winch = ofPtr<Winch>(new Winch(ofPoint(x,y), w, h, i));
        m_winches[i] = winch;
    }
}


bool WinchesManager::readCsv()
{
    string path = POSITIONS_DATA_PATH;
    
    if(m_files.find(m_currentFile)!=m_files.end()){
        path = m_files[m_currentFile];
    }
    
    // Load a CSV File.
    if(m_csv.load(path))
    {
        ofLogNotice() <<"WinchesManager::readPositions-> Successfully loaded: "<< path ;
        ofLogNotice() <<"WinchesManager::readPositions -> csv rows: "<< m_csv.getNumRows() << ", csv cols: " << m_csv.getNumCols();
        m_numPositions = m_csv.getNumRows()/NUM_WINCHES;
        ofLogNotice() <<"WinchesManager::readPositions -> Number of positions: "<< m_numPositions;
        return true;
    }
    else
    {
        ofLogNotice() <<"WinchesManager::readPositions -> Unable to load : "<< path ;
        return false;
    
    }
}

void WinchesManager::initializePositions()
{
    for (int i=0; i<NUM_WINCHES; i++)
    {
        PositionsVector positions;
        m_positions[i] = positions;
    }
}

void WinchesManager::initializeRanges()
{
    m_distanceRange.x = 0; m_distanceRange.y = 10.0; //Winch 10 Lifting Range [0m - 10m]
    m_speedRange.x = 0.05; m_speedRange.y = 0.3;    //Winch 10 Speed Range [5cm/s - 30cm/s]
}

bool WinchesManager::loadFiles()
{
    //some path, may be absolute or relative to bin/data
    string path = "positions/";
    
    ofDirectory dir(path);
    
    ofLogNotice() <<"WinchesManager::loadFiles ..." ;
    //only show csv files
    dir.allowExt("csv");
 
    //populate the directory object
    if(dir.listDir()==0){
        ofLogNotice()<< "WinchesManager::loadFiles-> No image files found in: " << dir.getAbsolutePath();
        return false;
    }
    
    ofLogNotice()<< "WinchesManager::loadFiles-> Path: " << dir.getAbsolutePath();
    ofLogNotice()<< "WinchesManager::loadFiles-> Size: " << dir.size();
    
    //go through and print out all the paths
    for(int i = 0; i < dir.size(); i++){
        string name = this->getFileName(dir.getPath(i));
        ofLogNotice()<< "WinchesManager::loadFiles-> Loaded: " << name;
        m_files[name] = dir.getPath(i);
    }
    
    m_currentFile = m_files.begin()->first;
    
    return true;
    
}

void WinchesManager::loadPositions()
{
    for(int i=0; i< m_positions.size(); i++){
        m_positions[i].clear();
    }
    
    for(int i=0; i< m_csv.getNumRows(); i++){
        int index = i % NUM_WINCHES;
        float pos = m_csv.getRow(i).getFloat(0)/1000; // Turn positions from mm to m
        m_positions[index].push_back(pos);
    }
    
    for(auto position: m_positions)
    {
        string posString;
        for(auto pos: position.second){
            posString = posString + ofToString(pos) + ", ";
        }
        
        ofLogNotice() <<"WinchesManager::loadPositions-> " << position.first << ": " + posString;
    }
}

void WinchesManager::update()
{
    this->updateWinches();
}


void WinchesManager::updateWinches()
{
    float offset = ofMap(m_offset, m_distanceRange.x,  m_distanceRange.y, 0.0, 1.0, true);
    for(auto winch: m_winches)
    {
        winch.second->update();
        float value = ofMap(winch.second->getValue() - offset, 0.0, 1.0, 100.0, 0.0);
        AppManager::getInstance().getDmxManager().onSetPosition(winch.first, value);
        AppManager::getInstance().getDmxManager().onSetSpeed(winch.first, m_speed);
        //ofLogNotice() <<"WinchesManager::updateWinches-> " <<value;
    }
}



void WinchesManager::draw()
{
    this->drawWinches();
}

void WinchesManager::drawWinches()
{
    for(auto winch: m_winches)
    {
        winch.second->draw();
    }
}


void WinchesManager::stop()
{
    this->setFrame(m_previousFrame, 1.0); // Set to current frame, that is stop
}

void WinchesManager::setFrame(int index, float time)
{
    if(index<0 || index >=m_numPositions){
        return;
    }
    
    for (int i=0; i<NUM_WINCHES; i++)
    {
        auto& positions = m_positions[i];
        this->startAnimation(i, positions[index], time);
        AppManager::getInstance().getDmxManager().onSetSpeed(i, m_speed);
    }
    
    m_previousFrame = index;
    
}

void WinchesManager::reset()
{
    for (int i=0; i<NUM_WINCHES; i++)
    {
        auto& positions = m_positions[i];
        AppManager::getInstance().getDmxManager().onSetPosition(i, positions[0]);
        AppManager::getInstance().getDmxManager().onSetSpeed(i, 30);
    }
    
   
}

float WinchesManager::getSpeed(float currentPos, float prevPos, float time)
{
    return 0.0;
}

void WinchesManager::startAnimation(int _id, float currentPos, float time )
{
    float distanceNorm = ofMap(currentPos, m_distanceRange.x, m_distanceRange.y, 0.0, 1.0, true);
    if(m_winches.find(_id)!=m_winches.end()){
        
        EffectSettings settings;
        settings.function = LINEAR; settings.type = EASE_OUT;
        settings.startAnimation = 0; settings.animationTime = time;
        
        AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(m_winches[_id]);
        AppManager::getInstance().getVisualEffectsManager().createValueEffect(m_winches[_id], distanceNorm, settings);
        
        //m_winches[_id]->setDistance(currentPos, distanceNorm);
    }
}

void WinchesManager::setWinch(int _id, float currentPos, float prevPos, float time)
{
    float speed = (currentPos-prevPos)/time;
    float speedNorm = ofMap(speed, m_speedRange.x, m_speedRange.y, 0.0, 1.0);
    
    float distanceNorm = ofMap(currentPos, m_distanceRange.x, m_distanceRange.y, 0.0, 1.0, true);
    
    if(m_winches.find(_id)!=m_winches.end()){
        m_winches[_id]->setSpeed(speed, speedNorm);
        m_winches[_id]->setDistance(currentPos, distanceNorm);
    }
}

void WinchesManager::sendDmx(int _id, float currentPos, float prevPos, float time)
{
    float distancePercentage = ofMap(currentPos, m_distanceRange.x, m_distanceRange.y, 100.0, 0.0, true);
    AppManager::getInstance().getDmxManager().onSetPosition(_id, distancePercentage);
    
    float speed = abs(currentPos-prevPos)/time;
    
    float speedPercentage  = 0.0; //If stop zero value
    if(speed>0){ //If not zero range = [0.05m/s - 30m/s]
        speedPercentage = ofMap(speed, m_speedRange.x, m_speedRange.y, 100.0/255.0 , 100.0, true);
    }
    
    AppManager::getInstance().getDmxManager().onSetSpeed(_id, speedPercentage);
}

string WinchesManager::getFileName(const string& path)
{
    std::vector<std::string> strs = ofSplitString(path, "/");
    strs = ofSplitString(strs.back(), ".");
    return strs.front();
}


void WinchesManager::setFile(const string& name)
{
    if(m_files.find(name)==m_files.end()){
        return;
    }
    
    m_currentFile = name;
    if(this->readCsv())
    {
        this->loadPositions();
    }
    
    AppManager::getInstance().getTimeLineManager().reset();
}

