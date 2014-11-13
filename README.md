#FaceOscTracker documentation

Based on examples by:<br/>
https://github.com/arturoc - oscSenderExample<br/>
https://github.com/kylemcdonald - example-face-follow

This work was done during a short course called DIY multitouch less touch held during the the introductory weeks at FHP-potsdam.

Keywords: openFrameworks, face tracking, Ocs, OpenCV

#The concept
During a course in _DIY multiTouch less touch_ at Potsdam Fachhochschule we tried different types of DIY multiTouch examples. One of the examples were faceTracking. As we explored faceTracking we came up with an idea of connecting faces to music/beats. What would happen if several tracked faces generated a beat, and together music. Also, if a face can be remembered we could represent this by either playing a specific beat for that face and also have a visual representation. The original idea here was to change the color ambience of a room. But we only used colored blobs for the visual representation. We also wanted the visual to react to the music that was playing. This was done by changing the sizes and of the blobs with the beat.

##Description
The FaceOscTracker tracks faces and sends the size of the Blob/Square of face as an OSC message.
The OSC massage is sent together with an id corresponding to that face to Osculator (http://www.osculator.net/). Osculator is used to convert the OSC messages and data to midi signals. By doing this we can send the midi signals to a program like Ableton Live. Abelton can then recive the signals and start or stop different tracks with beats. With the midi mapping in Ableton we could also map signals to chage effects on tracs. This woudl be done by mapping the values from Osculator. This is hard to tweak. it depends on the lighting in the room, the quality of the camera and the processing power of the computer. A larger distance from the object to track, the higher sensitivity of the image processing, the more processing power it consumes. The tracker can track several faces. It can also, "remember" the face and represent this with a color on specific blob for that face. A camera is used for the FaceTracking and a microphone or by mapping the built in sound source the size of the blobs can dynamically change to the music. Is also possible to map an image to a face. This is more for fun, put maybe there could be usefull for someone eslse.


##Requirements</b><br/>

<b>Framework:</b><br/>
openFrameworks - http://openframeworks.cc/

<b>openFramworks addons:</b><br/>
ofxOpenCv, ofxOsc, ofxCv

<b>Sofware</b><br/>
Xcode<br/>
Osculator - http://www.osculator.net/

###Folder structure (only the interesting files)

This is the folder strutureof the interesing files that hold varibles or code for tweaking.
In the file ofApp.cpp there are settings.¨


```
├── FaceOscTracker
    ├── src
        ├── main.cpp       (Setting for application windowsize)
        ├── ofApp.cpp      (Setup for video capture, tracking, and sound device, and image array)
        ├── ofApp.h        (Variable declarations)

    ├── bin
        ├── data           (Folder for images and for tracking xml data)
              ├── haarcascade_frontalface_alt2.xml
              ├── haarcascade_frontalface_default.xml
              ├── ape1.png
              ├── cat1.png
              ├── dog1.png
              ├── dog2.png
              ├── dog3.png
              ├── sunglasses.png


```
#Setup

```c

  string animals[6]={"dog1.png","dog2.png","dog3.png","ape1.png","cat1.png","sunglasses.png"};


  // Init camera, see the available video devices in the consolse
  drawPictures    = false; //Show images from the data folder, change image array for more images
  showVideo       = false;
  cameraDevice    = 0;

  //Camera settings
  camWidth 		    = 640;
  camHeight 		  = 480;
  camFrameRate    = 120;

  //Audio device, see the available audio devices in the consolse
  inputAdioDevice = 0;
```

<b>Tracking</b><br/>
Predefined settings:<br/>
In the file _ofApp.cpp_ at _Face tracking presets_ Tracking can be set with three predefined levels of see sensitivity. _Fast_, _Accurate_, _Sensitive_.<br/>

Manual settings:<br/>
If there is a need for more advanced settings uncomment the part that says
 _Manual tracking_ and play with the settings.

<b>Tracking examples for manual tweaking</b>

```c
//Fast
setRescale(.25);
setMinNeighbors(2);
setMultiScaleFactor(1.2);
setMinSizeScale(.25);
setMaxSizeScale(.75);
setCannyPruning(true);
setFindBiggestObject(false);

//Accurate
setRescale(.5);
setMinNeighbors(6);
setMultiScaleFactor(1.02);
setMinSizeScale(.1);
setMaxSizeScale(1);
setCannyPruning(true);
setFindBiggestObject(false);

//Sensitive
setRescale(.5);
setMinNeighbors(1);
setMultiScaleFactor(1.02);
setMinSizeScale(.1);
setMaxSizeScale(1);
setCannyPruning(false);
setFindBiggestObject(false);
```
