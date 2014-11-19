/*
 FaceOscTracker
 
 TO DO:
 - Check faces swapping labels (IDs)
 - Load images in array // setup()
 */

#include "ofApp.h"
#include <iostream>
#include <vector>

int cameraDevice;
int inputAdioDevice;
using namespace ofxCv;
using namespace cv;

vector<string> imageMemory;
vector<int> colorMemory;
vector<int> colorMemory2;

string animals[6]={"dog1.png","dog2.png","dog3.png","ape1.png","cat1.png","sunglasses.png"};

void ofApp::setup(){

    // Init camera, see the available video devices in the consolse
    drawPictures    = false;
    showVideo       = false;
    cameraDevice    = 0;
    
    //Camera settings
    camWidth 		= 640;
    camHeight 		= 480;
    camFrameRate    = 120;
    
    //Audio device, see the available audio devices in the consolse
    inputAdioDevice = 3;
    
    
    //finder.setup("haarcascade_frontalface_alt2.xml");
    finder.setup("haarcascade_frontalface_default.xml");
    
    //Face tracking presets, the more sensitive, the more processing power it takes choose between: Fast, Accurate , Sensitive
    finder.setPreset(ObjectFinder::Fast);

    //Manual tracking parapmeters Uncomment to activate
    /*
     finder.setRescale(.3);
     finder.setMinNeighbors(2);
     finder.setMultiScaleFactor(1.2);
     finder.setMinSizeScale(.03);
     finder.setMaxSizeScale(.8);
     finder.setCannyPruning(false);
     finder.setFindBiggestObject(false);
     */
    
    reactionDistance = 10; //Tweak the osc output to fit trigger reaction for playing
    
    ofSetVerticalSync(true);
    ofSetCircleResolution(50);
    
    if (showVideo) {
        ofSetBackgroundAuto(false);
        //ofEnableAlphaBlending();
        ofSetFrameRate(90);
    }else{
        ofBackground(255, 255, 255);
        ofEnableAlphaBlending();
        ofSetFrameRate(90);
        ofSetBackgroundAuto(true);
    }
    
    //List sounddevices in the
    soundStream.listDevices();
    
    //if you want to set a different device id
    soundStream.setDeviceID(inputAdioDevice); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
    int bufferSize = 512;
    
    
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(400, 0.0);
    
    bufferCounter	= 0;
    drawCounter		= 0;
    smoothedVol     = 0.0;
    scaledVol		= 0.0;
    
    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    
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

    vidGrabber.setDeviceID(cameraDevice);
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

    imageMemory.assign(300,"NULL");
    colorMemory.assign(300,0);
    colorMemory2.assign(300,0);
    
    finder.setUseHistogramEqualization(true);
    finder.getTracker().setSmoothingRate(.5);
    
    // Change tracker persistence
    // "persistence" determines how many frames an object can last without being
    // seen until the tracker forgets about it. "maximumDistance" determines how
    // far an object can move until the tracker considers it a new object.

    finder.getTracker().setPersistence(150); // Default: 15
    finder.getTracker().setMaximumDistance(camWidth/2); // Default: 64

    
    // Init OSC sender
    sender.setup(HOST, PORT);
    
}

//--------------------------------------------------------------

void ofApp::update(){
    
    //Particle
    
    //lets scale the vol up to a 0-1 range
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    
    //lets record the volume into an array
    volHistory.push_back( scaledVol );
    
    //if we are bigger the the size we want to record - lets drop the oldest value
    if( volHistory.size() >= 400 ){
        volHistory.erase(volHistory.begin(), volHistory.begin()+1);
    }
    
    //particle end
    
    // Update camera and finder
    vidGrabber.update();
    
    if (vidGrabber.isFrameNew())
    {
        videoTexture.setFromPixels(vidGrabber.getPixelsRef());
        //videoTexture.loadData(vidGrabber.getPixelsRef());
        
        videoTexture.mirror(false, true);
        videoTexture.update();
        
        finder.update(videoTexture);
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
        area.addIntArg(areaSize/reactionDistance);
        
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
        
        float videoW=videoTexture.getWidth();
        float ratio=ofGetWidth()/videoW;
        ofScale(ratio, ratio);
        videoTexture.draw(0,0);
        
    }else{
   
        float videoW=videoTexture.getWidth();
        float ratio=ofGetWidth()/videoW;
        ofScale(ratio, ratio);
            
        ofSetColor(255, 255, 255,15);
        ofFill();
        ofRect(0, 0, ofGetScreenWidth(), ofGetScreenHeight());
        
    }
   
    
   //finder.draw();
   //ofDrawBitmapStringHighlight(ofToString(finder.size()), 10,
    
    
    std::vector<unsigned int> trackerLables;
    
    trackerLables = finder.getTracker().getCurrentLabels();
    
    
    for(int i = 0; i < finder.size(); i++) {

        ofRectangle object = finder.getObjectSmoothed(i);
        
        //reMappedX = ofMap(object.x, 0, camWidth, 0, ofGetScreenWidth());   // circle X
        //reMappedY = ofMap(object.y, 0, camHeight, 0, ofGetScreenHeight());  // circle Y
        
        reMappedX = object.x;
        reMappedY = object.y;
        
        
        
        //Shader
        float targetX = reMappedX;
        float dx = targetX - x;
        if(abs(dx) > 1) {
            x += dx * easing;
        }
        
        float targetY = reMappedY;
        float dy = targetY - y;
        if(abs(dy) > 1) {
            y += dy * easing;
        }
        
        /*
        float targetR = scaledVol*100.0f;
        float dr = targetR - r;
        if(abs(dr) > 1) {
            r += dr * easing;
        }
         */
        //
        

        //DRAW IMAGES
        if (drawPictures) {
            //Check if images is in memory, and if it is, load it
            if ((imageMemory.at(finder.getTracker().getLabelFromIndex(i)) != "NULL")){
            
                //draw images
               
                imageOverlay.loadImage(imageMemory.at(finder.getTracker().getLabelFromIndex(i)));
                
                    
                cout << "If not empty";
                cout << endl;
                cout << finder.getTracker().getLabelFromIndex(i);
                cout << endl;
                cout << imageMemory.at(finder.getTracker().getLabelFromIndex(i));
                cout << endl;
               
                
               //Else add it to memory
            }else{

                imageMemory[finder.getTracker().getLabelFromIndex(i)]=animals[(int)ofRandom(0,5)];
                
                // draw images
               
                    
                cout << "Else if empty";
                cout << endl;
                cout << finder.getTracker().getLabelFromIndex(i);
                cout << endl;
                cout << imageMemory.at(finder.getTracker().getLabelFromIndex(i));
                cout << endl;
                
                
                imageOverlay.loadImage(imageMemory.at(finder.getTracker().getLabelFromIndex(i)));
               
                
            
            }
        }
        
        if(!drawPictures){
        //DRAW PARTICLES
        //Check if images is in memory, and if it is load it
        if ((colorMemory.at(finder.getTracker().getLabelFromIndex(i)) != 0)){
            
            //Particle start
            if(!showVideo){
                ofSetColor(colorMemory[finder.getTracker().getLabelFromIndex(i)], scaledVol *100, colorMemory2[finder.getTracker().getLabelFromIndex(i)]);
                ofFill();
                
                ofCircle(reMappedX+object.width / 2.,reMappedY+ object.height * .42, 50+scaledVol * 200.0f);

                ofSetColor(35, 100, scaledVol * 135,50);
                ofFill();
                ofCircle(reMappedX+object.width / 2., reMappedY+ object.height * .42, 25+scaledVol * 200.0f);
            }else{
                ofSetColor(35, 100, scaledVol * 135,50);
                ofFill();
            ofCircle(reMappedX+object.width / 2., reMappedY+ object.height * .42, 25+scaledVol * 200.0f);
            }
            
            //Particle end

            
            //Else add it to memory
        }else{
            
            colorMemory[finder.getTracker().getLabelFromIndex(i)]=(int)ofRandom(1,255);
            colorMemory2[finder.getTracker().getLabelFromIndex(i)]=(int)ofRandom(1,255);
            
            //Particle start
            if(!showVideo){
                ofSetColor(colorMemory[finder.getTracker().getLabelFromIndex(i)], scaledVol *100, colorMemory2[finder.getTracker().getLabelFromIndex(i)]);
                ofFill();
                
                ofCircle(reMappedX+object.width / 2., reMappedY+ object.height * .42, 50+scaledVol * 200.0f);
                
                ofSetColor(35, 100, scaledVol *135,50);
                ofFill();
                ofCircle(reMappedX+object.width / 2., reMappedY+ object.height * .42, 25+scaledVol * 200.0f);
            }else{
                ofSetColor(35, 100, scaledVol *135,50);
                ofFill();
            ofCircle(reMappedX+object.width / 2., reMappedY+ object.height * .42, 25+scaledVol * 200.0f);
            }
  
            
            //Particle end
            
        }
        }
        //NEW
        
        if (drawPictures){
        
        imageOverlay.setAnchorPercent(.5, .5);
        
        float scaleAmount = 1.4 * object.width / imageOverlay.getWidth();
        

        ofPushMatrix();
        ofTranslate(reMappedX + object.width / 2., reMappedY + object.height * .42);
        
        ofScale(scaleAmount, scaleAmount);
        
        
        imageOverlay.draw(0, 0);
        }
        
        ofPopMatrix();
        ofPushMatrix();
        ofTranslate(object.getPosition());
        //ofDrawBitmapStringHighlight(ofToString(finder.getLabel(i)), 0, 0);
        //ofLine(ofVec2f(), toOf(finder.getVelocity(i)) * 10);
        ofPopMatrix();
    }
}


//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
    float curVol = 0.0;
    
    // samples are "interleaved"
    int numCounted = 0;
    
    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    for (int i = 0; i < bufferSize; i++){
        left[i]		= input[i*2]*0.5;
        right[i]	= input[i*2+1]*0.5;
        
        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        numCounted+=2;
    }
    
    //this is how we get the mean of rms
    curVol /= (float)numCounted;
    
    // this is how we get the root of rms
    curVol = sqrt( curVol );
    
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
    
    bufferCounter++;
    
}