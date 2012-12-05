//
//  froebelContainer.h
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/5/12.
//
//

#ifndef FROEBELCONTAINER
#define FROEBELCONTAINER

#include "ofMain.h"
#include "froebelTextBox.h"

class froebelContainer : public ofRectangle {
public:
    
    froebelContainer();
    
    void            addElement( froebelTextBox *_newElement);
    bool            select(string _value);
    vector<string>  getSelected();
    void            clear();
    void            reset();
    
    virtual bool    checkMousePressed(ofPoint _mouse);
    
    vector<froebelTextBox*> elements;
    
    void        update();
    void        draw();
    
    froebelColor    bgColor;
    ofRectangle slider;
    
    bool        bEnable;
    int         maxHeight;
private:
    float       offsetY;
    float       offsetPct;
    float       damp;
    float       totalBoxHeight;
    float       totalLenght;
};

#endif
