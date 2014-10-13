#include "ofApp.h"

//#include <vector>

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
  
    
    finder.setup("haarcascade_frontalface_alt2.xml");
    //finder.setup("haarcascade_frontalface_default.xml");
    
    //finder.setPreset(ObjectFinder::Fast);
    
    finder.setRescale(.3);
    finder.setMinNeighbors(2);
    finder.setMultiScaleFactor(1.2);
    finder.setMinSizeScale(.03);
    finder.setMaxSizeScale(.8);
    finder.setCannyPruning(true);
    finder.setFindBiggestObject(false);
    finder.setUseHistogramEqualization(true);
    
    
    
    cam.initGrabber(1024, 768);
    
   
    
    /*
    image0.loadImage("dog1.png");
    image1.loadImage("dog2.png");
    image2.loadImage("cat1.png");
    image3.loadImage("ape1.png");
    image4.loadImage("dog3.png");
    */
    
    
    ofEnableAlphaBlending();
    sender.setup(HOST, PORT);
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    cam.update();
    if(cam.isFrameNew()) {
        finder.update(cam);
    }
    
    //Resseting osc messages
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
    
    
    std::vector<unsigned int> trackerLables;
    trackerLables = finder.getTracker().getCurrentLabels();
    
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
        
        
        cout << str;
        cout << areaSize/100;
        cout << endl;
        cout << finder.getTracker().getCurrentLabels().size();
        cout << endl;
        cout << numlabel;
        

        
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
   //ofDrawBitmapStringHighlight(ofToString(finder.size()), 10, 20);
   
        for(int i = 0; i < finder.size(); i++) {
        
        //ofRectangle object = finder.getObjectSmoothed(i);
        ofRectangle object = finder.getObject(i);
        image0.loadImage(animals[i]);
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


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
