#FaceOscTracker documentation

Based on examples by:<br/>
https://github.com/arturoc - oscSenderExample<br/>
https://github.com/kylemcdonald - example-face-follow

This work was done during a short course called DIY multitouch less touch held during the the introductory weeks at FHP-potsdam.

Keywords: openFrameworks, face tracking, Ocs, OpenCV

##The concept
During a course in _DIY multiTouch less touch_ at Potsdam Fachhochschule we tried different types of DIY multiTouch examples. One of the examples were faceTracking. As we explored faceTracking we came up with an idea of connecting faces to music/beats. What would happen if several tracked faces generated a beat, and together music. Also, if a face can be remembered we could represent this by either playing a specific beat for that face and also have a visual representation. The original idea here was to change the color ambience of a room. But we only used colored blobs for the visual representation. We also wanted the visual representation to react to the music that was playing. This was done by changing the size of the blobs in sync with the beat.

##Description
The FaceOscTracker tracks faces and sends the size of the Blob/Square of face as an OSC message.
The OSC massage is sent together with an id corresponding to that face to Osculator (http://www.osculator.net/). Osculator is used to convert the OSC messages and data to midi signals. By doing this we can send the midi signals to a program like Ableton Live. Abelton can then recive the signals and start or stop different tracks with beats. With the midi mapping in Ableton we could also map signals to chage effects on tracs. This woudl be done by mapping the values from Osculator. This is hard to tweak. it depends on the lighting in the room, the quality of the camera and the processing power of the computer. A larger distance from the object to track, the higher sensitivity of the image processing, the more processing power it consumes. The tracker can track several faces. It can also, "remember" the face and represent this with a color on specific blob for that face. A camera is used for the FaceTracking and a microphone or by mapping the built in sound source the size of the blobs can dynamically change to the music. Is also possible to map an image to a face. This is more for fun, put maybe there could be usefull for someone eslse.


##Requirements</b><br/>

Some knowledge of Ableton live, midi and some programming, at least programming terminology.


<b>Framework:</b><br/>
openFrameworks - http://openframeworks.cc/

<b>openFramworks addons:</b><br/>
ofxOpenCv, ofxOsc, ofxCv

<b>Sofware</b><br/>
Xcode<br/>
Osculator<br/>
Ableton Live<br/>



###Folder structure (only the interesting files)

This is the folder strutureof the interesing files that hold varibles or code for tweaking.
In the file ofApp.cpp there are settings.¨


```
├── FaceOscTracker
    ├── src
        ├── main.cpp       (Setting for application window size)
        ├── ofApp.cpp      (Setup for video capture, tracking, and sound device)
        ├── ofApp.h        (Variable declarations)

    ├── bin
        ├── data           (Folder for images and for tracking xml data)
              ├── haarcascade_frontalface_alt2.xml   (Tracking data, not to be changed)
              ├── haarcascade_frontalface_default.xml     (Tracking data, not to be changed)



```
##Setup
The folowing values in _ofApp.cpp_ are to be changed to fit your needs. Enable the parameter _showVideo_ when setting up the camera to make it easier. When you start the compile the program, you can see what camera device and audio device you have.


```c


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
###Face overlay Images
If you want to play with overlaying images on faces this is where you change,
In the beginning of _ofApp.cpp_ there is an array where you can change image file names. Images are not included. You need to create your own images.

```c
//Array for adding pictures from the data folder
  string animals[6]={"image1.png","image2.png","image3.png","image4.png","image5.png","sunglasses.png"};
```

##Setting up with Osculator and Ableton live

###Setting up Osculator
Launch the FaceOscTracker app from Xcode. Then start Osculator and change the port to the same as it is set in FaceOscTracker code _ofApp.h_. If it is already set to 3333 you are good to go. Osculator should map upp 7 areas after this. Save this osculator setup. Then change all the event types to Midi CC. Set value to 0 and assign a different channel to every OSC message(Signal)<br/>

###code in _ofApp.h_
```c
//OSC connection
#define HOST "localhost"
#define PORT 3333
```

###Osculator setup
![Alt text](http://www.bjelm.com/gitStuff/oscSetup.jpg "Optional title")

###Setting trigger sensitivity
With trigger sensitivity I mean what osc input signal level with trigger a MIDI CC signal. These settings are depending on want you want do do, but for our setup a value of max in 1000 and max out 1000 was good. Press the "secret" weird looking arrow button in the red circle. And yu can chancge the input and output max values.<br/><br/>
![Alt text](http://www.bjelm.com/gitStuff/oscSetup2.jpg "Optional title") <br/>


###Setting Ableton live to receive midi from Osculator
Then you need to start Osculator so that Ableton can see it. Then start Ableton live and go to the settings pane. And these are the settings in Ableton live for communicating with Osculator and Ableton live.<br/><br/>
![Alt text](http://www.bjelm.com/gitStuff/abeltonMidiSetup.jpg "Optional title") <br/><br/>

After this you have go into Midi mapping mode on Ableton. Map the midi signals coming from Osculator to what you want to happen in Ableton live. The easiest way to do this is to press the OSC signal button on each signal coming from FaceOscTracker. See red circle in the image below. Choose track and then a midi option on the track, a you have created in Ableton live, then press the button in the red circle on the osc signal you want to map to that midi track.

![Alt text](http://www.bjelm.com/gitStuff/osculatorbutton.jpg "Optional title") <br/>


<b>Good luck and happy tracking</b><br/>
/Petter B
