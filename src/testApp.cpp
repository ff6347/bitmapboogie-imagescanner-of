#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);

	ofSetVerticalSync(true);
	ofEnableSmoothing();
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 320-xInit;

    drawPadding = false;
    gui = new ofxUICanvas();
    gui->addLabel("Select an Image", OFX_UI_FONT_LARGE);
    gui->addSpacer();
    gui->addButton("B1", false, 44, 44);
    gui->addSpacer();
	gui->addWidgetDown(new ofxUILabel("Scan Resolution", OFX_UI_FONT_MEDIUM));
    gui->addWidgetDown(new ofxUIMinimalSlider(length-xInit, 1, 10,scan_resolution, "RES", OFX_UI_FONT_MEDIUM));
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
    red = 0; blue = 0; green = 0;
	ofBackground(red, green, blue);
    
    gui->setColorBack(ofColor(255,100));
    gui->setWidgetColor(OFX_UI_WIDGET_COLOR_BACK, ofColor(255,100));
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

    ofBackground(red, green, blue, 255);
//	
//	ofPushStyle();
//	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
//	ofPopStyle();
    
    for (int i=0; i<loadedImages.size(); i++){
		loadedImages[i].draw(0, 0);
	}
}

void testApp::guiEvent(ofxUIEventArgs &e){
	string name = e.widget->getName();
	int kind = e.widget->getKind();
    
    if(name == "RES"){
        ofxUIMinimalSlider *slider = (ofxUIMinimalSlider *) e.widget;
		scan_resolution = (int)slider->getScaledValue();
        cout << "value: " << scan_resolution << endl;
    }else if(name == "B1"){

        ofxUIButton *button = (ofxUIButton *) e.widget;

        cout << "value: " << button->getValue() << endl;
      if(button->getValue()==1){
      //Open the Open File Dialog
      ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a jpg or png");
      
      //Check if the user opened a file
      if (openFileResult.bSuccess){
          
          ofLogVerbose("User selected a file");
          
          //We have a file, check it and process it
          processOpenFileSelection(openFileResult);
          
      }else {
          ofLogVerbose("User hit cancel");
      }
      }
	}
   }

void testApp::writeColorData(vector<ofColor> imagecols,vector <int> x_s, vector <int> y_s, string path, string extension,int width, int height){
    
    int lastindex = path.find_last_of(".");
    string rawname = path.substr(0, lastindex);
    ofLogVerbose(ofToString(rawname));
    
    ofFile file(rawname +".txt",ofFile::WriteOnly);
    
    file << "{'width':"<< width <<",'height':"<< height << ",'scanresolution': " << ofToString(scan_resolution) << ", 'colors':[";
    for(int i = 0; i < imagecols.size();i++){
        float r = imagecols[i].r;
        float g = imagecols[i].g;
        float b = imagecols[i].b;
        float a = imagecols[i].a;
    file << "{'x':" << ofToString(x_s[i]) << ",'y':" <<  ofToString(y_s[i]) << ",";
    file << "'rgba':[" <<
        ofToString(r) << "," <<
        ofToString(g) <<  "," <<
        ofToString(b) << "," <<
        ofToString(a) << "]}";
    if(i != imagecols.size() -1){
        file << ",";
    }
    }
    file << "]}";
    file.close();

}

void testApp::processOpenFileSelection(ofFileDialogResult openFileResult){
    
    ofLogVerbose("getName(): "  + openFileResult.getName());
	ofLogVerbose("getPath(): "  + openFileResult.getPath());
	
	ofFile file (openFileResult.getPath());
    ofLogVerbose("Path only: " + file.path());
    //Limiting this example to one image so we delete previous ones
    processedImages.clear();
    loadedImages.clear();
    
    ofLogVerbose("The file exists - now checking the type via file extension");
    string fileExtension = ofToUpper(file.getExtension());
    //We only want images
    if (fileExtension == "JPG" || fileExtension == "PNG") {
        //Save the file extension to use when we save out
        originalFileExtension = fileExtension;
        //Load the selected image
        ofImage image;
        image.loadImage(openFileResult.getPath());
//        if (image.getWidth()>ofGetWidth() || image.getHeight() > ofGetHeight()) {
//            image.resize(image.getWidth()/2, image.getHeight()/2);
//        }
        loadedImages.push_back(image);
        //Make some short variables
        int w = image.getWidth();
        int h = image.getHeight();
        //Walk through the pixels
        vector<ofColor> imagecolors;
        vector<int> xs;
        vector<int> ys;
        for (int y = 0; y < h; y+= scan_resolution){
            //Create a vector to store and sort the colors
            for (int x = 0; x < w; x+=scan_resolution){
                
                //Capture the colors for the row
                ofColor color = image.getColor(x, y);
                xs.push_back(x);
                ys.push_back(y);
                imagecolors.push_back(color);
//                ofLogVerbose(ofToString(color));
                
                
            }
        }
        writeColorData(imagecolors,xs,ys,  file.path(),  originalFileExtension,w,h);
//        ofLogVerbose(imagecolors);
    }
    
}
void testApp::exit(){
    delete gui;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key)
    {
        case 'p':
        {
            drawPadding = !drawPadding;
            gui->setDrawWidgetPadding(drawPadding);
        }
            break;
            
        case 'g':
        {
            gui->toggleVisible();
        }
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}