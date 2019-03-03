#include "ofApp.h"

#define FREQUENCY (107.7 * 1000000)
#define SAMPLE_RATE 900001
#define BYTES_TO_READ (0xFFFF + 1)

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	float width = ofGetWidth();
	float height = ofGetHeight();
	int deviceCount = rtlsdr_get_device_count();
	cout << "RTL Device Count: " << deviceCount << endl;
	// setup the rtl sdr dongle
	if (deviceCount > 0) {
		std::cout << std::endl << "Opening device 0." << std::endl;

		std::cout << "(RET) rtlsdr_open = " << rtlsdr_open(&device, 0) << std::endl;
		std::cout << "(RET) rtlsdr_set_freq_correction = " << rtlsdr_set_freq_correction(device, 68) << std::endl;
		std::cout << "(RET) rtlsdr_set_center_freq = " << rtlsdr_set_center_freq(device, FREQUENCY) << std::endl;
		std::cout << "(RET) relsdr_set_sample_rate = " << rtlsdr_set_sample_rate(device, SAMPLE_RATE) << std::endl;

		std::cout << "(RET) rtlsdr_reset_buffer = " << rtlsdr_reset_buffer(device) << std::endl;
	}
	int rowsColsVal = LINE_SIZE;
	cout << "HOW BIG IS OUR MESH? " << rowsColsVal << endl;
	// create the mesh
	for (int r = 0; r<rowsColsVal; r++){
	    for (int c = 0; c<rowsColsVal; c++){
	        
	        glm::vec3 pos;      // grid centered at 0,0,0
	        
	        float halfWidth     = width * 0.5;
	        float halfHeight    = height * 0.5;
	        
	        pos.x = ofMap(r, 0, rowsColsVal-1, -halfWidth, halfWidth);
	        pos.y = ofMap(c, 0, rowsColsVal-1, halfHeight, -halfHeight);    // Y+ is up in 3D!
	        pos.z = 0;    // add depth later
	        
	        // add the point to the mesh
	        mesh.addVertex(pos);
	        
	        // add a color for the point
	        mesh.addColor(ofColor());

	        if (r > 0 && c > 0) {
	            int index = r * rowsColsVal + c;
	            // triangle 1
	            mesh.addIndex(index);               // A    - this pt
	            mesh.addIndex(index - 1);           // B    - + col
	            mesh.addIndex(index - rowsColsVal);        // C    - + row
	            
	            // triangle 2
	            mesh.addIndex(index - 1);           // B
	            mesh.addIndex(index - 1 - rowsColsVal);    // D
	            mesh.addIndex(index - rowsColsVal);        // C
	        }
	    }
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	// Read sync
	rtlsdr_read_sync(device, buffer, BYTES_TO_READ, &bytesRead);

	// reset these variables to 0
	iIdx = 0;
	qIdx = 0;
	// Construct the phase data array
	for (int i = 0; i < LINE_SIZE * 2 + 2; i += 2) {
		iData[iIdx++] = buffer[i];
		qData[qIdx++] = buffer[i + 1];
	}
	for (int i = 0; i < LINE_SIZE + 2; ++i) {
		phaseData[i] = atan2(iData[i], qData[i]);
	}
	// Construct the phase data difference double,
	// to use in our ekgLines vector
	for (int i = 0; i < LINE_SIZE; i++) {
		phaseDataDifference[i] = (phaseData[i] - phaseData[i + 1]) * 1000.f;
		ekgLines.push_back(phaseDataDifference[i]);
	}
	// Delete from the front of the vector if we're
	// larger than the mesh
	if (ekgLines.size() > LINE_SIZE * LINE_SIZE) {
		for (int i = 0; i < LINE_SIZE; i++) {
			ekgLines.erase(ekgLines.begin() + i);
		}
	}


	updateZValue();
	updateColors();
}

//--------------------------------------------------------------
void ofApp::draw(){
	cam.begin();
    ofEnableDepthTest();
    mesh.drawWireframe();
    ofDisableDepthTest();
    cam.end();
}

// --------------------------------
void ofApp::updateZValue(){
	for (int i = 0; i < mesh.getVertices().size(); ++i) {
		glm::vec3& vertex = mesh.getVertices()[i];
		vertex.z = ekgLines[i];
	}
}

// --------------------------------
void ofApp::updateColors(){
    
    // map colors based on vertex z / depth
    
    for (int i=0; i<mesh.getVertices().size(); i++){
        
        // 1 color per vertex
        glm::vec3& vertex = mesh.getVertices()[i];
        
        // get depth as percent of noise range
        float depthPercent = ofMap(vertex.z, -noiseAmp, noiseAmp, 0, 1, true);    // map 0-1

        // lerp color
        ofColor color = colorFar.getLerped( colorNear, depthPercent );
        
        mesh.setColor(i, color);        // set mesh color
    }

}

