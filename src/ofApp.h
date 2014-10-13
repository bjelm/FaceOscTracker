#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxCv.h"
#define HOST "localhost"
#define PORT 3333

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    ofVideoGrabber cam;
    ofxCv::ObjectFinder finder;
    ofxOscSender sender;
    ofImage image0;
    ofImage image1;
    ofImage image2;
    ofImage image3;
    ofImage image4;
    string animals[5]={"dog1.png","dog2.png","dog3.png","ape1.png","cat1.png"};
};
