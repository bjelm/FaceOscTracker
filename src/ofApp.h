#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxCv.h"
#include "ofxPS3EyeGrabber.h"

// OSC connection
#define HOST "localhost"
#define PORT 3333

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
    bool                showVideo;
    //Ps3 camera
    //ofxPS3EyeGrabber    vidGrabber;
    
    //Normal camera
    ofVideoGrabber 	    vidGrabber;
    unsigned char * 	videoInverted;
    ofImage             videoTexture;
    //ofTexture			videoTexture;
    int 				camWidth;
    int 				camHeight;
    int                 camFrameRate;
    bool                drawPictures;
    
    ofxCv::ObjectFinder finder;
    ofxOscSender sender;
    ofImage imageOverlay;

    string animals[5]={"dog1.png","dog2.png","dog3.png","ape1.png","cat1.png"};
    
    void audioIn(float * input, int bufferSize, int nChannels);
    
    vector <float> left;
    vector <float> right;
    vector <float> volHistory;
    
    int 	bufferCounter;
    int 	drawCounter;
    
    float smoothedVol;
    float scaledVol;
    float x;
    float y;
    float r;
    float easing = 0.05;
    
    
    ofSoundStream soundStream;
};
