#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxCv.h"

// OSC connection
#define HOST "localhost"
#define PORT 3333

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
    ofVideoGrabber cam;
    ofxCv::ObjectFinder finder;
    ofxOscSender sender;
    ofImage image0;
//    ofImage image1;
//    ofImage image2;
//    ofImage image3;
//    ofImage image4;
    string animals[5]={"dog1.png","dog2.png","dog3.png","ape1.png","cat1.png"};
};
