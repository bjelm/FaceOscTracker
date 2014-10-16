/*
 FaceOscTracker
 
 TO DO:
 - Check faces swapping labels (IDs)
 - Load images in array // setup()
 */

#include "ofApp.h"
#include <iostream>
#include <vector>

using namespace ofxCv;
using namespace cv;
vector<string> imageMemory;


//--------------------------------------------------------------

void ofApp::setup(){
    ofSetFrameRate(60);
    
    // Init camera
    drawPictures    = false;
    showVideo       = true;

    camWidth 		= 1024;	// try to grab at this size.
    camHeight 		= 768;
    camFrameRate    = 25;
    
    //we can now get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    
    
    for(int i = 0; i < devices.size(); i++){
        cout << devices[i].id << ": " << devices[i].deviceName;
        if( devices[i].bAvailable ){
            cout << endl;
        }else{
            cout << " - unavailable " << endl;
        }
    }
    
    
    
    //Normal camera
    
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(camFrameRate);
    vidGrabber.initGrabber(camWidth,camHeight);
    
    
    //ONLY PS3
    /*
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(camFrameRate);
    vidGrabber.initGrabber(camWidth, camHeight);
    vidGrabber.setAutogain(true);
    vidGrabber.setAutoWhiteBalance(true);
    vidGrabber.setBrightness(30);
    vidGrabber.setContrast(30);
    */
    //ONLY PS3
    
    
    videoInverted 	= new unsigned char[camWidth*camHeight*3];
    videoTexture.allocate(camWidth,camHeight, GL_RGB);
    
    ofSetVerticalSync(false);

    imageMemory.assign(300,"NULL");

    
    //finder.setup("haarcascade_frontalface_alt2.xml");
    finder.setup("haarcascade_frontalface_default.xml");
    
    // Define face tracking presets
    finder.setPreset(ObjectFinder::Fast);
    
    /*
    finder.setRescale(.3);
    finder.setMinNeighbors(2);
    finder.setMultiScaleFactor(1.2);
    finder.setMinSizeScale(.03);
    finder.setMaxSizeScale(.8);
    finder.setCannyPruning(false);
    finder.setFindBiggestObject(false);
    
    
    finder.setRescale(.2);
    finder.setMinNeighbors(2);
    finder.setMultiScaleFactor(1.1);
    finder.setMinSizeScale(.01);
    finder.setMaxSizeScale(.5);
    finder.setCannyPruning(true);
    finder.setFindBiggestObject(false);
    */
      finder.setRescale(.7);
      finder.setMinNeighbors(2);
      finder.setMultiScaleFactor(1.1);
      finder.setMinSizeScale(.01);
      finder.setMaxSizeScale(0.4);
      finder.setCannyPruning(true);
      finder.setFindBiggestObject(false);
     
    
    finder.setUseHistogramEqualization(true);
    finder.getTracker().setSmoothingRate(.3);
    
    // Change tracker persistence
    // "persistence" determines how many frames an object can last without being
    // seen until the tracker forgets about it. "maximumDistance" determines how
    // far an object can move until the tracker considers it a new object.

    finder.getTracker().setPersistence(300); // Default: 15
    finder.getTracker().setMaximumDistance(200); // Default: 64
    
     ofEnableAlphaBlending();
    
    // Init OSC sender
    sender.setup(HOST, PORT);
}

//--------------------------------------------------------------

void ofApp::update(){
    
    // Update camera and finder
    vidGrabber.update();
    
    if (vidGrabber.isFrameNew())
    {
        finder.update(vidGrabber);
        videoTexture.loadData(vidGrabber.getPixelsRef());
    }
    
    // Resseting osc messages
    ofxOscMessage area1;
    ofxOscMessage area2;
    ofxOscMessage area3;
    ofxOscMessage area4;
    ofxOscMessage area5;
    ofxOscMessage area6;
    ofxOscMessage area7;
    
    area1.setAddress("/face/area0/");
    area1.addIntArg(0);
    sender.sendMessage(area1);
    
    area2.setAddress("/face/area1/");
    area2.addIntArg(0);
    sender.sendMessage(area2);
    
    area3.setAddress("/face/area2/");
    area3.addIntArg(0);
    sender.sendMessage(area3);
    
    area4.setAddress("/face/area3/");
    area4.addIntArg(0);
    sender.sendMessage(area4);
    
    area5.setAddress("/face/area4/");
    area5.addIntArg(0);
    sender.sendMessage(area5);
    
    area6.setAddress("/face/area5/");
    area6.addIntArg(0);
    sender.sendMessage(area6);
    
    area7.setAddress("/face/area6/");
    area7.addIntArg(0);
    sender.sendMessage(area7);
    
    
 
    
    int labels=finder.getTracker().getCurrentLabels().size();
 
    
    for (int i = 0; i < labels; i++){
        
         /*
         if (finder.getLabel(i) > 10 ){
         
          Maybe reset the number of objects so i starts counting from 0 again.
 
         }
         */
        
        
        int numlabel=finder.getLabel(i);
        int areaSize=finder.getObject(i).getArea();
        
        char str[50];
        sprintf(str, "/face/area%d/", i);
        
        ofxOscMessage area;
        
        area.setAddress(str);
        area.addIntArg(areaSize/100);
        
        sender.sendMessage(area);

        
         /*
         ofxOscMessage id;
         id.setAddress("/face/id");
         id.addIntArg(i);
         sender.sendMessage(id);
     
         ofxOscMessage fx;
         fx.setAddress("/face/positionx");
         fx.addIntArg(object.x);
         sender.sendMessage(fx);
         
         
         ofxOscMessage fy;
         fy.setAddress("/face/positiony");
         fy.addIntArg(object.y);
         sender.sendMessage(fy);
         */

    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //Draw video
    if (showVideo) {
     videoTexture.draw(0, 0);
    }
   
    
   //finder.draw();
   //ofDrawBitmapStringHighlight(ofToString(finder.size()), 10,
    
    
    std::vector<unsigned int> trackerLables;
    
    trackerLables = finder.getTracker().getCurrentLabels();
    
    
    for(int i = 0; i < finder.size(); i++) {

        ofRectangle object = finder.getObjectSmoothed(i);

        //Image memory
        
        //Check if images is in memory, and if it is load it
        if ((imageMemory.at(finder.getTracker().getLabelFromIndex(i)) != "NULL")){
        
            //draw images
            if (drawPictures){
            imageOverlay.loadImage(imageMemory.at(finder.getTracker().getLabelFromIndex(i)));
            
                
            cout << "If not empty";
            cout << endl;
            cout << finder.getTracker().getLabelFromIndex(i);
            cout << endl;
            cout << imageMemory.at(finder.getTracker().getLabelFromIndex(i));
            cout << endl;
            }
            
           //Else add it to memory
        }else{

            imageMemory[finder.getTracker().getLabelFromIndex(i)]=animals[(int)ofRandom(0,4)];
            
            // draw images
            if (drawPictures) {
                
            cout << "Else if empty";
            cout << endl;
            cout << finder.getTracker().getLabelFromIndex(i);
            cout << endl;
            cout << imageMemory.at(finder.getTracker().getLabelFromIndex(i));
            cout << endl;
            
            
            imageOverlay.loadImage(imageMemory.at(finder.getTracker().getLabelFromIndex(i)));
            }
            
        
        }
        
        
        imageOverlay.setAnchorPercent(.5, .5);
        
        float scaleAmount = 1.4 * object.width / imageOverlay.getWidth();
        

        ofPushMatrix();
        ofTranslate(object.x + object.width / 2., object.y + object.height * .42);
        

        ofScale(scaleAmount, scaleAmount);
        
        if (drawPictures){
        imageOverlay.draw(0, 0);
        }
        
        ofPopMatrix();
        ofPushMatrix();
        ofTranslate(object.getPosition());
        ofDrawBitmapStringHighlight(ofToString(finder.getLabel(i)), 0, 0);
        //ofLine(ofVec2f(), toOf(finder.getVelocity(i)) * 10);
        ofPopMatrix();
    }
}