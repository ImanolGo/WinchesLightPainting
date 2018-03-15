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

const string WinchesManager::POSITIONS_DATA_PATH = "positions/winch_data.csv";


WinchesManager::WinchesManager(): Manager(), m_numPositions(0)
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
    
    this->initializePositions();
    
    if(this->readCsv())
    {
        this->loadPositions();
    }
}



bool WinchesManager::readCsv()
{
    // Load a CSV File.
    if(m_csv.load(POSITIONS_DATA_PATH))
    {
        ofLogNotice() <<"WinchesManager::readPositions-> Successfully loaded: "<< POSITIONS_DATA_PATH ;
        ofLogNotice() <<"WinchesManager::readPositions -> csv rows: "<< m_csv.getNumRows() << ", csv cols: " << m_csv.getNumCols();
        m_numPositions = m_csv.getNumRows()/NUM_WINCHES;
        ofLogNotice() <<"WinchesManager::readPositions -> Number of positions"<< m_numPositions;
        return true;
    }
    else
    {
        ofLogNotice() <<"WinchesManager::readPositions -> Unable to load : "<< POSITIONS_DATA_PATH ;
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

void WinchesManager::loadPositions()
{
    for(int i=0; i< m_csv.getNumRows(); i++){
        int index = i % NUM_WINCHES;
        float pos = m_csv.getRow(i).getFloat(0);
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

}



