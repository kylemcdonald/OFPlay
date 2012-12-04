#pragma once

#include "ofMain.h"

#include "ofAddon.h"
#include "CBLinuxProject.h"
#include "CBWinProject.h"
#include "visualStudioProject.h"
#include "xcodeProject.h"
#include <Poco/Path.h>

#include "ofxXmlSettings.h"

#include "froebelEditBox.h"
#include "froebelTextBox.h"
#include "froebelListBox.h"

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
 
    void    loadProject(string _path);
    bool    isAddonCore(string _addonName);
    string  setTarget(int targ);
    void    setStatus(string newStatus);
    void    generateProject();
    
    ofImage         logo;
    
    ofTrueTypeFont  font;
    ofTrueTypeFont  secondFont;

    froebelEditBox  projectName;
    froebelEditBox  projectPath;
    froebelListBox  platformsList;
    froebelListBox  addonsList;
    froebelTextBox  generateButton;
    
    string      appToRoot;
    string      defaultLoc;
    string      addonsPath;
    
    string      status;
    float       statusSetTime;
    float       statusEnergy;
    
    float       defaultHeight;
    
    baseProject *project;
};
