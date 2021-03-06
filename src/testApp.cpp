#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
//    ofEnableSmoothing();
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofSetWindowShape(900,500);
    ofSetWindowTitle( "OFPlay" );
//    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetDataPathRoot("../Resources/");
    
    mScreen = NULL;
    logo.loadImage("OFPlay.png");
    font.loadFont("Inconsolata.otf", 15, true,false,false,0.5,90);
    textSeq.set(900*0.5-(logo.getWidth()+75)*0.5,250,logo.getWidth()+75,150);
    textSeq.font = &font;

    //  Get addons list
    //
    ofSaveURLTo("http://www.patriciogonzalezvivo.com/ofplay.xml","ofplay.xml");
    
    //  First time??
    //
    ofFile testFile("config.xml");
    if (testFile.exists()){
        nStep = INSTALL_OF;
        textSeq.loadSequence("03-ofroot.xml");
        textSeq.bFinish = true;
    } else {
        nStep = OFPLAY_INTRO;
        
        textSeq.loadSequence("00-intro.xml");
    }
}


//--------------------------------------------------------------
void testApp::update(){
    
    if (mScreen == NULL){
        
        if (nStep == OFPLAY_INTRO){
            
            if (textSeq.bFinish){
                //  Only for OSX ( help need to port to other platform)
                //
                ofDirectory testDir("/Applications/Xcode.app");
                if (testDir.exists()){
                    nStep = INSTALL_GIT;
                } else {
                    testDir.open("/Developer/Applications/Xcode.app");
                    if (testDir.exists()){
                        nStep = INSTALL_GIT;
                    } else {
                        nStep =INSTALL_XCODE;
                        textSeq.loadSequence("01-xcode.xml");
                    }
                }
            }
            
        } else if (nStep == INSTALL_XCODE){
            
            ofDirectory testDir("/Applications/Xcode.app");
            if (testDir.exists()){
                nStep = INSTALL_GIT;
                textSeq.loadSequence("02-git.xml");
            } else {
                testDir.open("/Developer/Applications/Xcode.app");
                if (testDir.exists()){
                    nStep = INSTALL_GIT;
                    textSeq.loadSequence("02-git.xml");
                } else {
                    if (textSeq.bFinish){
                        nStep =INSTALL_XCODE;
                        textSeq.loadSequence("01-xcode.xml");
                    }
                }
            }
            
        } else if (nStep == INSTALL_GIT){
            
            ofFile testFile("/usr/bin/git");
            if (testFile.exists()){
                nStep = INSTALL_OF;
                textSeq.loadSequence("03-ofroot.xml");
            } else {
                //  NOT REALLY SEARCHING FOR IT
                //
                if (textSeq.bFinish){
                    nStep = INSTALL_OF;
                    textSeq.loadSequence("03-ofroot.xml");
                }
            }
            
        } else if (nStep == INSTALL_OF){
            
            if (textSeq.bFinish){
                searchForOF();
            }
        }
        
        textSeq.update();
    } else {
        mScreen->update();
    }
}

void testApp::searchForOF(){
    //  The XML will store basic information like the OF path
    //
    ofxXmlSettings XML;
    XML.loadFile("config.xml");
    string appToRoot = XML.getValue("appToRoot", "../../../../");
    string ofRoot    = XML.getValue("ofRoot", "../../../");
    string defaultLoc = XML.getValue("defaultNewProjectLocation", "apps/myApps");
    
    string binPath;
    //-------------------------------------
    // calculate the bin path (../../../ on osx) and the sketch path (bin -> root - > defaultLoc)
    //-------------------------------------
    // if appToRoot is wrong, we have alot of issues.  all these paths are used in this project:
    //
#ifdef TARGET_OSX
    binPath = ofFilePath::getAbsolutePath(ofFilePath::join(ofFilePath::getCurrentWorkingDirectory(), "../../../"));
#else
    binPath = ofFilePath::getCurrentExeDir();
#endif
    
    //  Try to search using the appToRoot that by defaul search 4 levels down ( distance from a /bin )
    //
    if ( !isOFFolder( ofFilePath::getAbsolutePath(ofFilePath::join( binPath, appToRoot)) ) ){
        
        //  Keep looping until found a OF path
        //
        while ( !isOFFolder(ofRoot)) {
            
            string command = "";
            ofFileDialogResult res = ofSystemLoadDialog("OF project generator", "choose the folder of your OF install");
            
            string result = res.filePath;
            convertWindowsToUnixPath(result);
            ofRoot = result + "/";
        }
        
    } else {
        ofRoot = ofFilePath::getAbsolutePath( ofFilePath::join( binPath, appToRoot)  );
    }

    XML.setValue("appToRoot", appToRoot);
    XML.setValue("ofRoot", ofRoot );
    XML.setValue("defaultNewProjectLocation", defaultLoc);
    XML.saveFile("config.xml");
    
    // there's some issues internally in OF with non unix paths for OF root
    //
    setOFRoot( ofRoot );
    
    mScreen = new mainScreenOFPlay( ofRoot , defaultLoc, "newProject");
    mScreen->logo = &logo;
    mScreen->resized();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(230);
    
    if (mScreen != NULL){
        mScreen->draw();
    } else {
        ofSetColor(255);
        logo.draw(ofGetWidth()*0.5-logo.getWidth()*0.5, ofGetHeight()*0.5-logo.getHeight());
        
        textSeq.draw();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (mScreen == NULL){
        if ( key == OF_KEY_RIGHT){
            textSeq.setNextLine();
        } else if ( key == OF_KEY_LEFT){
            textSeq.setPrevLine();
        }
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
    ofPoint mouse = ofPoint(x, y);
    
    if (mScreen != NULL){
        mScreen->mousePressed(mouse);
    } else {
        int rta = -1;
        
        for (int i = 0; i < textSeq.size(); i++) {
            if (textSeq.buttons[i].checkMousePressed(mouse)){
                rta = i;
            }
        }
        
        if (rta != -1){
            textSeq.setLine(rta);
        } else if ( x > ofGetWidth()*0.5 ){
            textSeq.setNextLine();
        } else {
            textSeq.setPrevLine();
        }
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    if (mScreen != NULL)
        mScreen->resized();
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    if (dragInfo.files.size() > 1){ 
        
    } else if (dragInfo.files.size() == 1) {
        string open = dragInfo.files[0];
        if (isProjectFolder(open)){
            
            if (mScreen != NULL)
                mScreen->loadProject(open);
        }
    }
}
