#include "ofApp.h"
#include "rtl-sdr.h"

#define FREQUENCY (107.7 * 1000000)
#define SAMPLE_RATE 900001
#define BYTES_TO_READ (0xFFFF + 1)

//--------------------------------------------------------------
void ofApp::setup(){
	uint32_t deviceCount = rtlsdr_get_device_count();
	cout << "RTL Device Count: " << deviceCount << endl;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

