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
    ofSetVerticalSync(true);
    ofSetFrameRate(15);
    
    imageMemory.assign(300,"NULL");

    
    //finder.setup("haarcascade_frontalface_alt2.xml");
    finder.setup("haarcascade_frontalface_default.xml");
    
    // Define face tracking presets
    //finder.setPreset(ObjectFinder::Fast);
    
    /*
    finder.setRescale(.3);
    finder.setMinNeighbors(2);
    finder.setMultiScaleFactor(1.2);
    finder.setMinSizeScale(.03);
    finder.setMaxSizeScale(.8);
    finder.setCannyPruning(false);
    finder.setFindBiggestObject(false);
    */
    
    finder.setRescale(.27);
    finder.setMinNeighbors(2);
    finder.setMultiScaleFactor(1.2);
    finder.setMinSizeScale(.05);
    finder.setMaxSizeScale(.2);
    finder.setCannyPruning(false);
    finder.setFindBiggestObject(false);
    
    finder.setUseHistogramEqualization(false);
    finder.getTracker().setSmoothingRate(.3);
    
    // Change tracker persistence
    // "persistence" determines how many frames an object can last without being
    // seen until the tracker forgets about it. "maximumDistance" determines how
    // far an object can move until the tracker considers it a new object.

    finder.getTracker().setPersistence(200); // Default: 15
    finder.getTracker().setMaximumDistance(700); // Default: 64
    
    // Init camera
    cam.initGrabber(1280, 720);
    
    /*
    image0.loadImage("dog1.png");
    image1.loadImage("dog2.png");
    image2.loadImage("cat1.png");
    image3.loadImage("ape1.png");
    image4.loadImage("dog3.png");
    */
    
    ofEnableAlphaBlending();
    
    // Init OSC sender
    sender.setup(HOST, PORT);
}

//--------------------------------------------------------------

void ofApp::update(){
    
    // Update camera and finder
    cam.update();
    if(cam.isFrameNew()) {
        finder.update(cam);
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
        cout << str;
        cout << areaSize/100;
        cout << endl;
        cout << finder.getTracker().getCurrentLabels().size();
        cout << endl;
        cout << numlabel;
        cout << endl;
      
      */
       // cout << finder.getTracker().getSmoothingRate();
        

        
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
    cam.draw(0, 0);
    
   //finder.draw();
   //ofDrawBitmapStringHighlight(ofToString(finder.size()), 10,
    
    
    std::vector<unsigned int> trackerLables;
    
    //std::vector<unsigned int> imageMemory;
    
    trackerLables = finder.getTracker().getCurrentLabels();
    
    
    for(int i = 0; i < finder.size(); i++) {

        ofRectangle object = finder.getObjectSmoothed(i);

        //Building image memory
        /*
         if(std::find(vector.begin(), vector.end(), item)!=vector.end()){
         if ((i > 0 && i < imageMemory(i).size()) && (imageMemory()[i] != NULL)){
         
        
         
         // image0.loadImage(animals[i]);
         
         }
        }
         if (finder.getTracker().getLabelFromIndex(i) > 0 && finder.getTracker().getLabelFromIndex(i) < imageMemory.size()){
         
         */
        
        if ((imageMemory.at(finder.getTracker().getLabelFromIndex(i)) != "NULL")){
        
     
            image0.loadImage(imageMemory.at(finder.getTracker().getLabelFromIndex(i)));
            
            cout << "If not empty";
            cout << endl;
            cout << finder.getTracker().getLabelFromIndex(i);
            cout << endl;
            cout << imageMemory.at(finder.getTracker().getLabelFromIndex(i));
            cout << endl;
            
        }else{

            imageMemory[finder.getTracker().getLabelFromIndex(i)]=animals[(int)ofRandom(0,4)];
            
            cout << "Else if empty";
            cout << endl;
            cout << finder.getTracker().getLabelFromIndex(i);
            cout << endl;
            cout << imageMemory.at(finder.getTracker().getLabelFromIndex(i));
            cout << endl;
            
            image0.loadImage(imageMemory.at(finder.getTracker().getLabelFromIndex(i)));
        }
        
        
        image0.setAnchorPercent(.5, .5);
        
        float scaleAmount = 1.4 * object.width / image0.getWidth();
        

        ofPushMatrix();
        ofTranslate(object.x + object.width / 2., object.y + object.height * .42);
        

        ofScale(scaleAmount, scaleAmount);
        image0.draw(0, 0);
        
        ofPopMatrix();
        ofPushMatrix();
        ofTranslate(object.getPosition());
        ofDrawBitmapStringHighlight(ofToString(finder.getLabel(i)), 0, 0);
        //ofLine(ofVec2f(), toOf(finder.getVelocity(i)) * 10);
        ofPopMatrix();
    }
}