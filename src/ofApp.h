#pragma once

#include "ofMain.h"
#include "rtl-sdr.h"

#define BYTES_TO_READ (0xFFFF + 1)

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void updateZValue();
		void updateColors();

		// called in setup
		unsigned char buffer[BYTES_TO_READ];
		int bytesRead;
		rtlsdr_dev_t *device;
		double phaseData[BYTES_TO_READ / 2];

		// mesh stuff
		ofEasyCam cam;
		ofMesh mesh;
		ofColor colorNear = ofColor::fuchsia;
		ofColor colorFar = ofColor::navy;
		float noiseAmp = 40.f;

		// not called until update
		int iIdx = 0;
		unsigned char iData[BYTES_TO_READ / 2];
		int qIdx = 0;
		unsigned char qData[BYTES_TO_READ / 2];
		
};
