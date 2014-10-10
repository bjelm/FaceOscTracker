#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(120);
  
    
    finder.setup("haarcascade_frontalface_alt2.xml");
    //finder.setPreset(ObjectFinder::Fast);
    
    finder.setRescale(.3);
    finder.setMinNeighbors(2);
    finder.setMultiScaleFactor(1.2);
    finder.setMinSizeScale(.03);
    finder.setMaxSizeScale(.8);
    finder.setCannyPruning(true);
    finder.setFindBiggestObject(false);
    
    cam.initGrabber(1280, 720);
    sunglasses.loadImage("sunglasses.png");
    lionface.loadImage("lionface.png");
    ofEnableAlphaBlending();
    sender.setup(HOST, PORT);
}

//--------------------------------------------------------------
void ofApp::update(){
    cam.update();
    if(cam.isFrameNew()) {
        finder.update(cam);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.draw(0, 0);
    //finder.draw();
   // ofDrawBitmapStringHighlight(ofToString(finder.size()), 10, 20);
    
    for(int i = 0; i < finder.size(); i++) {
        ofRectangle object = finder.getObjectSmoothed(i);
        lionface.setAnchorPercent(.5, .5);
        float scaleAmount = 1.4 * object.width / lionface.getWidth();
        
        ofPushMatrix();
        ofTranslate(object.x + object.width / 2., object.y + object.height * .42);
        
        char str[50];
        sprintf(str, "/face/area%d", i);
        
        
        ofxOscMessage area;
        area.setAddress(str);
        area.addIntArg(object.getArea());
        sender.sendMessage(area);
        
       /*
        ofxOscMessage id;
        id.setAddress("/face/id");
        id.addIntArg(i);
        sender.sendMessage(id);
       */
        ofScale(scaleAmount, scaleAmount);
        lionface.draw(0, 0);
       /*
        ofxOscMessage fx;
        fx.setAddress("/face/positionx");
        fx.addIntArg(object.x);
        sender.sendMessage(fx);
        
        
        ofxOscMessage fy;
        fy.setAddress("/face/positiony");
        fy.addIntArg(object.y);
        sender.sendMessage(fy);
        */
        
        ofPopMatrix();
        ofPushMatrix();
        ofTranslate(object.getPosition());
        //ofDrawBitmapStringHighlight(ofToString(finder.getLabel(i)), 0, 0);
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
    ofxOscMessage m;
    m.setAddress("/mouse/position");
    m.addIntArg(x);
    m.addIntArg(y);
    sender.sendMessage(m);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    ofxOscMessage m;
    m.setAddress("/mouse/button");
    m.addStringArg("down");
    sender.sendMessage(m);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    ofxOscMessage m;
    m.setAddress("/mouse/button");
    m.addStringArg("up");
    sender.sendMessage(m);
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
