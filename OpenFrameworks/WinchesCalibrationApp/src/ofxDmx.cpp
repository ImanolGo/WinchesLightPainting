#include "ofxDmx.h"
#include "ofMain.h"

#define DMX_PRO_HEADER_SIZE 4
#define DMX_PRO_START_MSG 0x7E
#define DMX_START_CODE 0
#define DMX_START_CODE_SIZE 1
#define DMX_PRO_SEND_PACKET 6 // "periodically send a DMX packet" mode
#define DMX_PRO_SERIAL_NUMBER 10 // Get Widget Serial Number Request
#define DMX_PRO_END_SIZE 1
#define DMX_PRO_END_MSG 0xE7

ofxDmx::ofxDmx()
:connected(false)
,needsUpdate(false) {
}

ofxDmx::~ofxDmx() {
	serial.close();
	connected = false;
}

bool ofxDmx::connect(int device, unsigned int channels) {
	serial.listDevices();
	connected = serial.setup(device, 57600); 
	setChannels(channels);
	return connected;
}

bool ofxDmx::connect(string device, unsigned int channels) {
	serial.listDevices();
	connected = serial.setup(device.c_str(), 57600);
	setChannels(channels);
	return connected;
}

bool ofxDmx::autoconnect(unsigned int channels)
{
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    
    for(auto device: deviceList)
    {
        if(this->checkConnection(device.getDeviceID())) //open a device number
        {
            this->connect(device.getDeviceID(), channels);
            ofLogNotice() <<"ENTTEC USB PRO connected to port " << device.getDeviceName();
			return true;
        }
    }

    connected = false;
	return false;
}

bool ofxDmx::checkConnection(int portNum)
{
    if(serial.setup(portNum, 57600)) //open a device number
    {
        this->sendPin();
        ofSleepMillis(50);
        if(this->receivedOk()){
            ofLogNotice() <<"Received OK from ENTECC USB PRO ";
			serial.close();
            return true;
        }
		else {
			serial.close();
		}
    }
    
    ofLogError() <<" ENTTEC USB PRO is not connected ";
    //m_serial.setup(0, BAUD_RATE);
    return false;
    
}

void ofxDmx::sendPin()
{
    unsigned int dataSize = 0;
    unsigned int packetSize = DMX_PRO_HEADER_SIZE + dataSize + DMX_PRO_END_SIZE;
    vector<unsigned char> packet(packetSize);
    
    // header
    packet[0] = DMX_PRO_START_MSG;
    packet[1] = DMX_PRO_SERIAL_NUMBER;
    packet[2] = dataSize & 0xff; // data length lsb
    packet[3] = (dataSize >> 8) & 0xff; // data length msb
    
    // data
    //packet[4] = DMX_START_CODE; // first data byte
    //copy(levels.begin(), levels.end(), packet.begin() + 5);
    
    // end
    packet[4] = DMX_PRO_END_MSG;
    
    serial.writeBytes(&packet[0], packetSize);
}

bool ofxDmx::receivedOk()
{
    if ( serial.available() == 0 )
    {
        return false;
    }
    
    /// // we want to read 4 bytes
    int bytesRequired = 4;
    unsigned char bytes[4];
    int bytesRemaining = bytesRequired;
    // loop until we've read everything
    while ( bytesRemaining > 0 ){
        // check for data
        if ( serial.available() > 0 ){
            // try to read - note offset into the bytes[] array, this is so
            // that we don't overwrite the bytes we already have
            int bytesArrayOffset = bytesRequired - bytesRemaining;
            int result = serial.readBytes( &bytes[bytesArrayOffset], bytesRemaining );
            
            // check for error code
            if ( result == OF_SERIAL_ERROR ){
                // something bad happened
                ofLog( OF_LOG_ERROR, "unrecoverable error reading from serial" );
                break;
            }
            else if ( result == OF_SERIAL_NO_DATA ){
                // nothing was read, try again
            }
            else {
                // we read some data!
                bytesRemaining -= result;
            }
        }
    }
    
    return true;
}

bool ofxDmx::isConnected() {
	return connected;
}

void ofxDmx::disconnect() {
	serial.close();
    connected = false;
}

void ofxDmx::setChannels(unsigned int channels) {
	levels.resize(ofClamp(channels, 24, 512));
}

void ofxDmx::update(bool force) {
    if(!connected){
        return;
    }
    
	if(needsUpdate || force) {
		needsUpdate = false;
		unsigned int dataSize = levels.size() + DMX_START_CODE_SIZE;
		unsigned int packetSize = DMX_PRO_HEADER_SIZE + dataSize + DMX_PRO_END_SIZE;
		vector<unsigned char> packet(packetSize);
		
		// header
		packet[0] = DMX_PRO_START_MSG;
		packet[1] = DMX_PRO_SEND_PACKET;
		packet[2] = dataSize & 0xff; // data length lsb
		packet[3] = (dataSize >> 8) & 0xff; // data length msb
		
		// data
		packet[4] = DMX_START_CODE; // first data byte
		copy(levels.begin(), levels.end(), packet.begin() + 5);
		
		// end
		packet[packetSize - 1] = DMX_PRO_END_MSG;
		
		serial.writeBytes(&packet[0], packetSize);
		
#ifdef OFXDMX_SPEW
		cout << "@" << ofGetSystemTime() << endl;
		for(int i = 0; i < packetSize; i++) {
			cout << setw(2) << hex << (int) packet[i];
			if((i + 1) % 8 == 0) {
				cout << endl;
			}
		}
#endif
	}
}

bool ofxDmx::badChannel(unsigned int channel) {
	if(channel > levels.size()) {
		//ofLogError() << "Channel " + ofToString(channel) + " is out of bounds. Only " + ofToString(levels.size()) + " channels are available.";
		return true;
	}
	if(channel == 0) {
		//ofLogError() << "Channel 0 does not exist. DMX channels start at 1.";
		return true;
	}
	return false;
}

void ofxDmx::setLevel(unsigned int channel, unsigned char level) {
	if(badChannel(channel)) {
		return;
	}
	channel--; // convert from 1-initial to 0-initial
	if(level != levels[channel]) {
		levels[channel] = level;
		needsUpdate = true;
	}
}

void ofxDmx::clear() {
	for (int i = 0; i < levels.size(); i++) {
		levels[i] = 0;
	}
}

unsigned char ofxDmx::getLevel(unsigned int channel) {
	if(badChannel(channel)) {
		return 0;
	}
	channel--; // convert from 1-initial to 0-initial
	return levels[channel];
}
