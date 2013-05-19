/**************************************************************************************************************************
 *     ______  __  __   ______       ______  __  __   __   ______   _____       ______   ______   ______   __    __       *
 *    /\__  _\/\ \_\ \ /\  ___\     /\__  _\/\ \_\ \ /\ \ /\  == \ /\  __-.    /\  == \ /\  __ \ /\  __ \ /\ "-./  \      *
 *    \/_/\ \/\ \  __ \\ \  __\     \/_/\ \/\ \  __ \\ \ \\ \  __< \ \ \/\ \   \ \  __< \ \ \/\ \\ \ \/\ \\ \ \-./\ \     *
 *       \ \_\ \ \_\ \_\\ \_____\      \ \_\ \ \_\ \_\\ \_\\ \_\ \_\\ \____-    \ \_\ \_\\ \_____\\ \_____\\ \_\ \ \_\    *
 *        \/_/  \/_/\/_/ \/_____/       \/_/  \/_/\/_/ \/_/ \/_/ /_/ \/____/     \/_/ /_/ \/_____/ \/_____/ \/_/  \/_/    *
 *                                                                                                                        *
 *                                                                                                                        *
 *                                                                                                                        *
 *  By: Colin Honigman    (Programmer/Designer)                                                                           *
 *      Andrew Walton     (Designer)                                                                                      *
 *      Ajay Kapur        (Sponsor)                                                                                       *
 **************************************************************************************************************************
 **************************************************************************************************************************/

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"
#include "cinder/MayaCamUI.h"
#include "cinder/Timeline.h"
#include "cinder/Font.h"
#include "cinder/cairo/Cairo.h"
#include "cinder/Text.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/MovieWriter.h"
#include "cinder/CinderResources.h"
#include "cinder/TriMesh.h"
#include "cinder/Serial.h"
#include "cinder/Thread.h"

#include "OscListener.h"
#include "OscSender.h"

#include <vector>
#include <math.h>
#include <stdlib.h>

#include "MoveBehavior.h"
#include "BounceAll.h"
#include "BounceUpDown.h"
#include "Instrument.h"
#include "Ball.h"
#include "Screen.h"
#include "User.h"

#define RES_PREVIEW_IMAGE			CINDER_RESOURCE( ../resources/, lava.jpg, 128, IMAGE )
#define BUFSIZE 80
#define READ_INTERVAL 0.01


using namespace ci;
using namespace ci::app;
using namespace std;
using namespace params;

GLfloat mat_diffuse[]		= { 0.5, 0.5, 0.8, 1.0 };
GLfloat mat_emission[]		= { 1.0, 0.0, 1.0, 0.0 };
GLfloat mat_shininess[]     = { 50.0 };



struct Box{
    Box() : mColor( ColorA( 1, 1, 1, .5) ), mPos( Vec3f(0, 0, 0) ), mSize( Vec3f(10, 10, 10) ), on(false) {}
    Box( Color color, Vec3f pos, Vec3f size, int ID) : mColor( color ), mPos( pos ), mSize ( size ), id ( ID ), on (false){}
    
    int     id;
    int     width;
    int     height;
    int     length;
    int     x, y, z;
    int     wallID;
    bool    on;
    bool    states[2][1];
    bool    lastState;
    int     note;
    double  lastTouch;
    Anim<ColorA> mColor;
    Anim<Vec3f> mPos;
    Anim<Vec3f> mSize;
    
    
    bool withinBounds(Vec3f objectPosition){
        //        if(objectPosition.distance(mPos) < 5){
        //
        //            return true;
        //        }
        Vec3f pos = mPos.value();
        if(objectPosition.distance(pos) < 4){
            return true;
            if(wallID == 0){
                if(objectPosition.x < pos.x+5 && objectPosition.x > pos.x-5 && objectPosition.y > pos.y && objectPosition.z < pos.z+5 && objectPosition.z > pos.z-5){
                    return true;
                }
            }
            else if(wallID == 1){
                if(objectPosition.x < pos.x+5 && objectPosition.x > pos.x-5 && objectPosition.y < pos.y && objectPosition.z < pos.z+5 && objectPosition.z > pos.z-5){
                    //  cout << "Ceiling panel " << id << endl;
                    return true;
                }
            }
            else if(wallID == 2){
                if(objectPosition.x < pos.x+5 && objectPosition.x > pos.x-5 && objectPosition.z > pos.z && objectPosition.y < pos.y+5 && objectPosition.y > pos.y-5){
                    return true;
                }
            }
            else if(wallID == 3){
                if(objectPosition.x < pos.x+5 && objectPosition.x > pos.x-5 && objectPosition.z < pos.z && objectPosition.y < pos.y+5 && objectPosition.y > pos.y-5){
                    return true;
                }
            }
            else if(wallID == 4){
                if(objectPosition.z < pos.z+5 && objectPosition.z > pos.z-5 && objectPosition.x < pos.x && objectPosition.y < pos.y+5 && objectPosition.y > pos.y-5){
                    return true;
                }
            }
            else if(wallID == 5){
                if(objectPosition.z < pos.z+5 && objectPosition.z > pos.z-5 && objectPosition.x > pos.x && objectPosition.y < pos.y+5 && objectPosition.y > pos.y-5){
                    return true;
                }
            }
            else return false;
        }
        else return false;
        return false;
    }
    
    
    bool touched(Vec3f userHand){
        Vec3f pos = mPos.value();
        if(pos.distance(userHand) < 4 && (getElapsedSeconds() - lastTouch) > 3){
            
            lastState = on;
            on = !on;
            lastTouch = getElapsedSeconds();
            //cout << "touched " << id << " " << on << endl;
            //mColor = ColorA(1.0, 1.0, 0.0, 1.0);
            return true;

        }
        else return false;
        
        return false;
    }
    
    bool touched(Vec3f userHand, Vec3f rotation){
        Vec3f pos = mPos.value();
        if(rotation == Vec3f(0.0, 0.0, 360.0)){
            if(pos.distance(userHand) < 4 && (getElapsedSeconds() - lastTouch) > 3){
                lastState = on;
                on = !on;
                lastTouch = getElapsedSeconds();
                cout << "touched " << id << " in second orientation" << endl;
                //mColor = ColorA(0.0, 1.0, 0.0, 1.0);
                return true;
            }
            else return false;
        }
        else if(rotation == Vec3f(0.0, 0.0, 0.0)){
            if(pos.distance(userHand) < 4 && (getElapsedSeconds() - lastTouch) > 3){
                lastState = on;
                on = !on;
                lastTouch = getElapsedSeconds();
                cout << "touched " << id << " in first orientation" << endl;
                //mColor = ColorA(1.0, 1.0, 0.0, 1.0);
                return true;
            }
            else return false;
        }
        else{
            return false;
        }
        return false;
    }
    
    
    bool isOn(){
//        if(on){
//           // mColor = ColorA(1.0, 1.0, 0, .7);
//        }
//        else{
        if(!on){
           // mColor = ColorA(1.0, 1.0, 1.0, 0.0);
        }
        return on;
    }
    
};


class ThirdRoomApp : public AppBasic {
public:
    
    void                prepareSettings( Settings *settings);
	void                setup();
	void                update();
	void                draw();
    void                mouseDown( MouseEvent event );
    /*! \brief Draw Room Skeleton
     *draw a room skeleton custom width/length/height for installation room */
    void                drawRoomSkeleton(float width, float length, float height);
    void                setupGrid(float w, float h, float l);
    void                wallHit(Ball* ball);
    void                animateBox(Box* box);
    void                oscUpdate();
    void                drawCharacterVerbose( cairo::Context &ctx, Vec2f where );
    void                introduction();
    void                updateSerial();
    void                sendOscWall(int _wall, int _note);
    void                sendOscCeiling(int _wall, int _note, float velocity);
    void                sendOscSequence(int step, int state);
    void                sendOscSequence2(int step, int state);
    void                sendGroupNote(float note);
    void                sendGroupNoteOff();
    void                sendGroupParametersOsc(float, float, float);
    void                sendOscScreen(float, float, float);
    void                threadedUpdate();
    void                shutdown();
    double              nowMinus(double time);
    
    std::thread         mThread, mOscThread;
    bool                mShouldQuit;
private:
    float               roomWidth, roomLength, roomHeight, totalBoxes; //variables for room sizes
    float               eyePointx, eyePointy, eyePointz;
    float               centerPointx, centerPointy, centerPointz;
    Anim<Vec3f>         eyePoint;
    Anim<Vec3f>         centerPoint;
    Anim<Vec3f>         roomRotation;
    bool                loaded;
    bool                doneLoading;
    
    Box                 grid[1400];
    
    bool                animateGrid;
    int                 county;
    bool                hit;
    bool                lookat;
    bool                showRoom;
    Vec3f               lookie;
    CameraPersp         cam;
    
    osc::Listener       oscListener;
    osc::Sender         oscSender;
    
    string              host;
    int                 port;
    vector<Instrument*> instruments;
    vector<User>        users;
    vector<TriMesh>     meshes;
    vector<Vec2i>       groups;
    
    int                 sequences[2][1400];
    
    //Text variables
    TextLayout          text;
    gl::Texture         mLabelText;
    Anim<ColorA>        textColor;
    Anim<ColorA>        helloColor;
    Anim<string>        hello;
    string              welcome;
    string              would;
    bool                intro;
    double              introTime;
    float                 bass[6];
    int                 bassCount;
    int                 screenCount;
    float               storeScreens[3];
    float               storeScreensZ[3];
    float               rotateX, rotateY, rotateZ;
    
protected:
    MayaCamUI           mayaCam; //mayaCam to move through 3d space
    Matrix44f           mTransform;
    Vec2i               mousePos;
    double              mTime;
    InterfaceGl         mParams;
    bool                clickSwitch;
    bool                DIFFUSE;
    Font                mFont;
    Shape2d             mShape;
    
    enum jointNamesGlobal{
        head = 0,
        neck,
        leftShoulder,
        rightShoulder,
        leftElbow,
        rightElbow,
        leftHand,
        rightHand,
        torso,
        leftHip,
        rightHip,
        leftKnee,
        rightKnee,
        leftFoot,
        rightFoot
    };
    
    enum side{
        ceiling = 0,
        floor,
        frontWall,
        backWall,
        leftWall,
        rightWall
    };
    
    
    qtime::MovieWriter	mMovieWriter;
    TriMesh             mesh;
    Vec3f               textVec;
    //Serial Variables
    bool                serialFound, sendSerialMessage, textureComplete;
    Serial              serial;
    uint8_t             ctr;
    std::string         lastString;
    double              sinceLastRead, lastUpdate;
    gl::Texture         mTexture;
    
    int                 howManyGrouped;
    
    int                 mouseCount;
    float               sequencerPosition;
};

void ThirdRoomApp::prepareSettings( Settings *settings ){
    settings->setFrameRate(40.0f);
    settings->setWindowSize(800, 600);
    //settings->setDisplay( ci::Display::getDisplays()[1] );
    settings->enableSecondaryDisplayBlanking( false );
}

void ThirdRoomApp::setup(){
    glShadeModel(GL_SMOOTH);
    //setup viewing camera
    eyePoint        = Vec3f(0, 100, 500);
    centerPoint     = Vec3f(0, 0, -50);
    eyePointx       =   0;
    eyePointy       = -20;
    eyePointz       =  50;
    centerPointx    =   0;
    centerPointy    = -20;
    centerPointz    = -50;
    rotateX = rotateY = rotateZ = 0;
    roomRotation = Vec3f(0.0, 0.0, 0.0);
    mThread = thread( bind( &ThirdRoomApp::threadedUpdate, this));
    mThread.detach();
    //mOscThread = thread( bind( &ThirdRoomApp::oscUpdate, this));
    //mOscThread.detach();
    mShouldQuit = false;
    
    mParams = InterfaceGl("Menu", Vec2i(200, 200));
    mParams.addParam("Eye Point X", &eyePointx, "min=-1000 max=1000");
    mParams.addParam("Eye Point Y", &eyePointy, "min=-1000 max=1000");
    mParams.addParam("Eye Point Z", &eyePointz, "min=-1000 max=1000");
    mParams.addParam("Center Point X", &centerPointx, "min=-1000 max=1000");
    mParams.addParam("Center Point Y", &centerPointy, "min=-1000 max=1000");
    mParams.addParam("Center Point Z", &centerPointz, "min=-1000 max=1000");
    mParams.addParam("Room Width", &roomWidth, "min=50 max=500");
    mParams.addParam("Room Height", &roomHeight, "min=50 max=500");
    mParams.addParam("Room Length", &roomLength, "min=50 max=500");
    mParams.addParam("Animate", &animateGrid);
    mParams.addParam("Show Room", &showRoom);
    mParams.addParam("Rotate X", &rotateX);
    mParams.addParam("Rotate Y", &rotateY);
    mParams.addParam("Rotate Z", &rotateZ);
    mTransform.setToIdentity();
    loaded = false;
    animateGrid = false;
    hit = false;
    county = 0;

    roomWidth = 120;
    roomLength = 120;
    roomHeight = 70;
    totalBoxes = (roomWidth/10)*(roomLength/10)*(roomHeight/10);
    cout << "Total Boxes: " << totalBoxes << endl;
    for(int i = 0; i < totalBoxes; i++){
        grid[i] = Box();
        grid[i].states[0][0] = grid[i].states[1][0] = false;
    }
    
    setupGrid(roomWidth, roomHeight, roomLength);
    glDisable(GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    lookat = false;
    
    
    host = "127.0.0.1";
    port = 13000;
    oscSender.setup(host, port);
    oscListener.setup(12000);

    mFont = Font("Futura-CondensedMedium", 172);
    vector<Font::Glyph> glyphs = mFont.getGlyphs("abc123");
    clickSwitch = false;
    doneLoading = false;
    
    DIFFUSE = true;
    
    
    hello       = "Hello";
    welcome     = "Welcome to the Third Room";
    intro       = true;
    textColor   = ColorA(1.0f, 1.0f, 1.0f, 0.0f);
    textVec     = Vec3f(-400, 500, -900);
    showRoom    = false;
   // sendOscScreen(20, 20, 20);
    
    //    fs::path path = getSaveFilePath();
    //	if( path.empty() )
    //		return; // user cancelled save
    //
    //Serial Setup
    ctr = 0;
    lastString = "";
    sinceLastRead = 0.0;
    lastUpdate  = 0.0;
    sendSerialMessage = false;
    textureComplete = false;
    sequencerPosition = -500;
    
    bass[0] = 48;
    bass[1] = 48;
    bass[2] =  46;
    bass[3] = 43;
    bass[4] = 50;
    bass[5] = 46;
    bassCount = 0;
    
    screenCount = 0;
    storeScreens[0] = 0;
    storeScreens[1] = 0;
    storeScreens[2] = 0;
    
//    const vector<Serial::Device> &devices(Serial::getDevices());
//    for( vector<Serial::Device>::const_iterator deviceIt = devices.begin(); deviceIt != devices.end(); ++deviceIt ) {
//		console() << "Device: " << deviceIt->getName() << endl;
//	}
	serialFound = false;
//	try {
//		//Serial::Device dev = Serial::findDeviceByNameContains("tty.usbserial");
//		//serial = Serial( dev, 57600);
//        serialFound = false;
//        
//	}
//	catch( ... ) {
//		console() << "There was an error initializing the serial device!" << std::endl;
//        serialFound = false;
//		//exit( -1 );
//	}
    if(serialFound){
        // wait for * as a sign for first contact
        char contact = 0;
        while(contact != '*')
        {
            contact = (char) serial.readByte();
        }
        // request actual data
        serial.writeByte(ctr);
        
        // clear accumulated contact messages in buffer
        char b = '*';
        while(serial.getNumBytesAvailable() > -1)
        {
            b = serial.readByte();
            console() << b << "_";
        }
        
        serial.flush();
        
    }
    
    
    //Quicktime save filepath -- intialize all variables before this point
    //    qtime::MovieWriter::Format format;
    //    if( qtime::MovieWriter::getUserCompressionSettings( &format, loadImage( loadResource( RES_PREVIEW_IMAGE ) ) ) ) {
    //        mMovieWriter = qtime::MovieWriter( path, getWindowWidth(), getWindowHeight(), format );
    //    }
    
    
}

void ThirdRoomApp::mouseDown( MouseEvent event ){
    mouseCount++;
}

void ThirdRoomApp::threadedUpdate(){
    ci::ThreadSetup threadSetup;
    while(!mShouldQuit){
        if(!intro && nowMinus(introTime) > 5){
            for(int i = 0; i < users.size(); i++){
                
                //************************************************
                //Gestures and Consequences
                //************************************************
                Instrument* theInstrument = users[i].isGesturing();
                if(theInstrument != NULL){
                    theInstrument->setBoundaries(Vec3f(roomWidth, roomHeight, roomLength));
                    instruments.push_back(theInstrument);
                }
                
                
                // Hit Test Instruments
                for(int j = 0; j < instruments.size(); j++){
                    Instrument* in = *(instruments.begin()+j);
                    if(in->getName() == "Screen"){
                        
                        storeScreens[screenCount%3] = in->getSize().x;
                        if(in->getUserID() == users[i].getUserID() && !in->hitTest(&users[i])){
                            instruments.erase(instruments.begin()+j);
                        }
                        
                        screenCount++;
                    }
                    else{
                        if(!in->isHit()){
                            in->hitTest(&users[i]);
                        }
                        else if(in->isHit() && in->getUserID() == users[i].getUserID()){
                            
                            in->setPosition(users[i].getJointPosition(in->getUserHand()));
                            if(in->getUserHand() == leftHand && in->getTime() > 1 && users[i].isThrowingLeft() ){
                                
                                in->setHit(false);
                                in->setMoving(true);
                                in->setVelocity(users[i].getPositionDifference(leftHand));
                                in->getVelocityPtr()->limit(5);
                                in->setLastThrown(getElapsedSeconds());
                                in->setColor(ColorA(1.0, 1.0, 0.0, 1.0));
                                users[i].setUnactive(leftHand);
                                //cout << "throwing left " << in->getVelocity() <<  endl;
                            }
                            else if(in->getUserHand() == rightHand && in->getTime() > 1 && users[i].isThrowingRight()){
                                
                                in->setHit(false);
                                in->setMoving(true);
                                in->setVelocity(users[i].getPositionDifference(rightHand));
                                in->getVelocityPtr()->limit(5);
                                in->setLastThrown(getElapsedSeconds());
                                in->setColor(ColorA(1.0, 1.0, 0.0, 1.0));
                                users[i].setUnactive(rightHand);
                                //cout << "throwing right " <<  in->getVelocity() << endl;
                            }
                        }
                    }
                }
                
                
                //*******************************************************************************************************************
                // Grouping Detection
                //*******************************************************************************************************************
                for(int u = i+1; u < users.size(); u++){
                    ci::Vec3f tempJoint = users[i].getJointPosition(torso);
                    if(!users[i].isGrouped() && !users[u].isGrouped() && tempJoint.distance(users[u].getJointPosition(torso)) < 20){
                        users[i].setGroup(true);
                        users[u].setGroup(true);
                        sendGroupNote(bass[bassCount%6]);
                        bassCount++;
                        
                    }
                    else if(users[i].isGrouped() && users[i].isGrouped() && tempJoint.distance(users[u].getJointPosition(torso)) > 20){
                        users[i].setGroup(false);
                        users[u].setGroup(false);
                     //   cout << "setting false" << endl;
                        sendGroupNoteOff();
                    
                    }

                }
                if(users.size() == 1 && users[0].isGrouped()){
                    users[0].setGroup(false);
                    sendGroupNoteOff();
                    
                }
                
            }
        }
        
    }
}

void ThirdRoomApp::update(){
    
    cam.setEyePoint( eyePoint );
    cam.setCenterOfInterestPoint( Vec3f(centerPointx, centerPointy, centerPointz) );
    if(lookat){
        cam.lookAt(lookie);
    }
    cam.setPerspective(80.0f, getWindowAspectRatio(), 1.0f, 1000.0f);
    mayaCam.setCurrentCam( cam );
    if(!loaded){
        timeline().apply( &eyePoint, Vec3f(0, -30, 50), 5.0f, EaseInQuad() );
        loaded = true;
        doneLoading = true;
    }
    else{
        
        eyePoint = Vec3f(eyePointx, eyePointy, eyePointz);
    }
    if(doneLoading){
        for(int i = 0; i < totalBoxes; i++){
            timeline().appendTo( &grid[i].mColor, ColorA(1.0f, 1.0f, 1.0f, 0.0f), 6.60f, EaseInQuad() );
        }
        doneLoading = false;
    }
    
    if(animateGrid){
        
        timeline().apply( &grid[county%600].mSize, Vec3f(10, 10, 10), .10f, EaseInQuad() );
        if(county < 160){
            timeline().appendTo( &grid[county%600].mSize, Vec3f(10, 10, 0), .20f, EaseOutQuad() );
        }
        else if(county >= 160 && county < 320){
            timeline().appendTo( &grid[county%600].mSize, Vec3f(0, 10, 10), .20f, EaseOutQuad() );
        }
        else if (county >= 320 && county < 520){
            timeline().appendTo( &grid[county%600].mSize, Vec3f(10, 0, 10), .20f, EaseOutQuad() );
        }
        timeline().apply( &grid[county%600].mColor, ColorA((rand()%100)*.01, (rand()%100)*.01, (rand()%100)*.01, 1.0f), .10f, EaseInQuad() );
        timeline().appendTo( &grid[county%600].mColor, ColorA(1.0f, 1.0f, 1.0f, 1.0f), .10f, EaseInQuad() );
        
        county++;
        if(county > 599){
            animateGrid = false;
            county = 0;
        }
    }
    if(showRoom){
        for(int i = 0; i < 1008; i++){
            if(grid[i].mColor.value() != ColorA(1.0, 1.0, 1.0, 1.0)){
                //grid[i].mColor = ColorA(1.0, 1.0, 1.0, 1.0);
                timeline().appendTo(&grid[i].mColor, ColorA(1.0f, 1.0f, 1.0f, 1.0f), .5f, EaseInQuad() );
            }
        }
    }
    mesh.clear();
    for(int i = 0; i < users.size(); i++){
        if(intro){
            if(users[i].isWaving()){
                //  cout << "waving hello" << endl;
                introduction();
                introTime = getElapsedSeconds();
            }
        }
        else{
            if(users[i].isFullClearing()){
                instruments.erase(instruments.begin(), instruments.end());
                //cout << "clearing" << endl;
                for(int u = 0; u < users.size(); u++){
                    users[u].setUnactive(leftHand);
                    users[u].setUnactive(rightHand);
                    users[u].setScreen(false);
                    users[u].setGroup(false);
                }
                for(int i = 0; i < totalBoxes; i++){
                    grid[i].on = false;
                    grid[i].states[0][0] = false;
                    grid[i].states[1][0] = false;
                }
                for(int i = 0; i < 48; i++){
                    sendOscSequence(i, 0);
                }
                
            }
            else if(users[i].isClearing()){
                instruments.erase(instruments.begin(), instruments.end());
                //cout << "clearing" << endl;
                for(int u = 0; u < users.size(); u++){
                    users[u].setUnactive(leftHand);
                    users[u].setUnactive(rightHand);
                    users[u].setScreen(false);
                }
                sendGroupNoteOff();
            }
            if(users[i].isStomping()){
                
            }
            
            if(users[i].isGrouped()){
                mesh.appendColorRGB(Color(1.0, 0, 1.0));
                mesh.appendVertex(users[i].getJointPosition(head));
                mesh.appendColorRGB(Color(1.0, 0, .8));
                mesh.appendVertex(users[i].getJointPosition(leftShoulder));
                mesh.appendColorRGB(Color(1.0, 0, 1.0));
                mesh.appendVertex(users[i].getJointPosition(rightShoulder));
                mesh.appendColorRGB(Color(.8, 0, 1.0));
                mesh.appendVertex(users[i].getJointPosition(leftElbow));
                mesh.appendColorRGB(Color(.7, 0, .8));
                mesh.appendVertex(users[i].getJointPosition(rightElbow));
                mesh.appendColorRGB(Color(.6, 0, .6));
                mesh.appendVertex(users[i].getJointPosition(leftHand));
                mesh.appendColorRGB(Color(.5, 0, 1.0));
                mesh.appendVertex(users[i].getJointPosition(rightHand));
                mesh.appendColorRGB(Color(.6, 0, 1.0));
                mesh.appendVertex(users[i].getJointPosition(leftFoot));
                mesh.appendColorRGB(Color(.5, 0, .5));
                mesh.appendVertex(users[i].getJointPosition(rightFoot));
                mesh.appendColorRGB(Color(.3, 0, .3));
     
            
            }
        }
        
        
        for(int b = 0; b < totalBoxes; b++){
            //Billie Jean Disco Floor
            if( grid[b].wallID == floor){
                
                if(grid[b].withinBounds(users[i].getJointPosition(leftFoot)) || grid[b].withinBounds(users[i].getJointPosition(rightFoot))){
                    timeline().apply(&grid[b].mColor, ColorA(1.0, 0, 0, 1.0), .5);
                }
                else{
                    if(roomRotation.value() == Vec3f(0.0, 0.0, 0.0)){
                        if(!grid[b].states[0][0] && grid[b].mColor.value() != ColorA(1.0, 1.0, 1.0, 0)  ){
                            timeline().apply(&grid[b].mColor, ColorA(1.0, 1.0, 1.0, 0), .5);
                        }
//                        else if(!grid[b].states[0][0] && (ColorA)grid[b].mColor == ColorA(1.0, 1.0, 0.0, .7) ){
//                            timeline().appendTo(&grid[b].mColor, ColorA(1.0, 1.0, 1.0, 0), 1.0);
//                        }
                        else if(grid[b].states[0][0] && (ColorA)grid[b].mColor != ColorA(1.0, 1.0, 0, .5)){
                            timeline().apply(&grid[b].mColor, ColorA(1.0, 1.0, 0.0, .5), 1.0);
                        }
                    }
                    else if(roomRotation.value() == Vec3f(0.0, 0.0, 360.0)){
                        if(!grid[b].states[1][0] && grid[b].mColor.value() != ColorA(1.0, 1.0, 1.0, 0.0)){
                            timeline().apply(&grid[b].mColor, ColorA(1.0, 1.0, 1.0, 0), .5);
                        }
//                        else if(!grid[b].states[1][0] && (ColorA)grid[b].mColor == ColorA(0, 1.0, 0, .7) ){
//                            timeline().appendTo(&grid[b].mColor, ColorA(1.0, 1.0, 1.0, 0), 1.0);
//                        }
                        else if(grid[b].states[1][0] && grid[b].mColor.value() != ColorA(0.0, 1.0, 0, .5)){
                            timeline().apply(&grid[b].mColor, ColorA(0.0, 1.0, 0.0, .5), 1.0);
                        }
                    }
                       
                }
                //Touch Floor Sequencer
                if(roomRotation.value() == Vec3f(0.0, 0.0, 0.0)){
                    if(grid[b].touched(users[i].getJointPosition(leftHand)) ){
                        sequences[0][b] = !sequences[0][b];
                        grid[b].states[0][0] = !grid[b].states[0][0];
                        sendOscSequence(grid[b].id%48, grid[b].states[0][0]);
                    
                    }
                }
                else if(roomRotation.value() == Vec3f(0.0, 0.0, 360.0)){
                    if(grid[b].touched(users[i].getJointPosition(leftHand)) ){
                        sequences[1][b] = !sequences[1][b];
                        grid[b].states[1][0] = !grid[b].states[1][0];
                        sendOscSequence2(grid[b].id%48, grid[b].states[1][0]);
                        
                    }
                }
            }
        }
        
        
        users[i].update();
        
    }
    
    for(int j = 0; j < instruments.size(); j++){
        Instrument* in = *(instruments.begin()+j);
        in->update();
        if(in->getName() == "Ball"){
            wallHit((Ball*) in);
        }
        else if(in->getName() == "Screen"){
            if(storeScreens != storeScreensZ){
                sendOscScreen(storeScreens[0], storeScreens[1], storeScreens[2]);
                storeScreensZ[0] = storeScreens[0];
                storeScreensZ[1] = storeScreens[1];
                storeScreensZ[2] = storeScreens[2];
            }
        }
        for(int i = j+1; i < instruments.size(); i++){
            Instrument* in2 = *(instruments.begin()+i);
            if(in->getName() == "Ball" && in2->getName() == "Ball"){
                in->collisionTest(in2);
            }
        }
    }
        
    if(serialFound){
        updateSerial();
    }
    oscUpdate();
    
}

void ThirdRoomApp::updateSerial(){
    double now = getElapsedSeconds();
    double deltaTime = now - lastUpdate;
    lastUpdate = now;
    sinceLastRead += deltaTime;
    
    if(sinceLastRead > READ_INTERVAL)
    {
        sendSerialMessage = true;
        sinceLastRead = 0.0;
    }
    
    
    if (sendSerialMessage)
    {
        // request next chunk
        serial.writeByte(ctr);
        
        try{
            // read until newline, to a maximum of BUFSIZE bytes
            lastString = serial.readStringUntil('\n', BUFSIZE );
            
        } catch(SerialTimeoutExc e) {
            console() << "timeout" << endl;
        }
        
        
        sendSerialMessage = false;
        
        ctr+=8;
        console() << lastString << endl;
        
        
//        TextLayout simple;
//        simple.setFont( Font( "Arial Black", 24 ) );
//        simple.setColor( Color( .7, .7, .2 ) );
//        simple.addLine( lastString );
//        simple.setLeadingOffset( 0 );
//        mTexture = gl::Texture( simple.render( true, false ) );
//        textureComplete = true;
        
        serial.flush();
    }
}


void ThirdRoomApp::oscUpdate(){
   // ci::ThreadSetup threadSetup2;
   // while(!mShouldQuit){
        while(oscListener.hasWaitingMessages()){
            osc::Message message;
            oscListener.getNextMessage(&message);
            
            if(message.getAddress() == "/skeleton"){
                int tempID = message.getArgAsInt32(0);
                for(int i = 0; i < users.size(); i++){
                    if(users[i].getUserID() == tempID){
                        users[i].setJointPosition(users[i].head,            Vec3f( message.getArgAsFloat(1),message.getArgAsFloat(2),message.getArgAsFloat(3) ));
                        users[i].setJointPosition(users[i].neck,            Vec3f( message.getArgAsFloat(4),message.getArgAsFloat(5),message.getArgAsFloat(6) ));
                        users[i].setJointPosition(users[i].leftShoulder,    Vec3f( message.getArgAsFloat(7),message.getArgAsFloat(8),message.getArgAsFloat(9) ));
                        users[i].setJointPosition(users[i].rightShoulder,   Vec3f( message.getArgAsFloat(10),message.getArgAsFloat(11),message.getArgAsFloat(12) ));
                        users[i].setJointPosition(users[i].leftElbow,       Vec3f( message.getArgAsFloat(13),message.getArgAsFloat(14),message.getArgAsFloat(15) ));
                        users[i].setJointPosition(users[i].rightElbow,      Vec3f( message.getArgAsFloat(16),message.getArgAsFloat(17),message.getArgAsFloat(18) ));
                        users[i].setJointPosition(users[i].leftHand,        Vec3f( message.getArgAsFloat(19),message.getArgAsFloat(20),message.getArgAsFloat(21) ));
                        users[i].setJointPosition(users[i].rightHand,       Vec3f( message.getArgAsFloat(22),message.getArgAsFloat(23),message.getArgAsFloat(24) ));
                        users[i].setJointPosition(users[i].torso,           Vec3f( message.getArgAsFloat(25),message.getArgAsFloat(26),message.getArgAsFloat(27) ));
                        users[i].setJointPosition(users[i].leftHip,         Vec3f( message.getArgAsFloat(28),message.getArgAsFloat(29),message.getArgAsFloat(30) ));
                        users[i].setJointPosition(users[i].rightHip,        Vec3f( message.getArgAsFloat(31),message.getArgAsFloat(32),message.getArgAsFloat(33) ));
                        users[i].setJointPosition(users[i].leftKnee,        Vec3f( message.getArgAsFloat(34),message.getArgAsFloat(35),message.getArgAsFloat(36) ));
                        users[i].setJointPosition(users[i].rightKnee,       Vec3f( message.getArgAsFloat(37),message.getArgAsFloat(38),message.getArgAsFloat(39) ));
                        users[i].setJointPosition(users[i].leftFoot,        Vec3f( message.getArgAsFloat(40),message.getArgAsFloat(41),message.getArgAsFloat(42) ));
                        users[i].setJointPosition(users[i].rightFoot,       Vec3f( message.getArgAsFloat(43),message.getArgAsFloat(44),message.getArgAsFloat(45) ));
                    }
                }
            }
            
            else if(message.getAddress() == "/newUser"){
                int tempID = message.getArgAsInt32(0);
                users.push_back(User(tempID));
                cout << "NEW USER!  " << tempID <<  endl;
            }
            else if(message.getAddress() == "/lostUser"){
                for(int i = 0; i < users.size(); i++){
                    int tempID = message.getArgAsInt32(0);
                    if(users[i].getUserID() == tempID){
                        users.erase(users.begin() + i);
                        cout << "LOST USER!" << endl;
                    }
                }
            }
            else if(message.getAddress() == "/shakeX"){
                cout << "Shake X: " <<  message.getArgAsFloat(0) << " " << message.getArgAsInt32(1) << endl;
                if(roomRotation.value() == Vec3f(0, 0, 0)){
                    timeline().apply(&roomRotation, Vec3f(0, 0, 360), 4.0);
                    for(int i = 0; i < totalBoxes; i++){
                        if(grid[i].states[1][0] && grid[i].mColor.value() != ColorA(0.0, 1.0, 0.0, .7)){
                            timeline().appendTo(&grid[i].mColor, ColorA(0.0, 1.0, 0.0, .7), 4.0);
                        }
                        else if(grid[i].mColor.value() != ColorA(1.0, 1.0, 1.0, 0.0)){
                            timeline().appendTo(&grid[i].mColor, ColorA(1.0, 1.0, 1.0, 0.0), 2.0);
                            
                        }
                    }
                }
                else{
                    timeline().apply(&roomRotation, Vec3f(0, 0, 0), 4.0);
                    for(int i = 0; i < totalBoxes; i++){
                        if(grid[i].states[0][0] && grid[i].mColor.value() != ColorA(1.0, 1.0, 0.0, .7)){
                            timeline().apply(&grid[i].mColor, ColorA(1.0, 1.0, 0.0, .7), 4.0);
                        }
                        else if(grid[i].mColor.value() != ColorA(1.0, 1.0, 1.0, 0.0)) {
                            timeline().apply(&grid[i].mColor, ColorA(1.0, 1.0, 1.0, 0.0), 2.0);
                            
                        }
                    }
                }
            }
            else if(message.getAddress() == "/shakeY"){
                cout << "Shake Y: " <<  message.getArgAsFloat(0) << " " << message.getArgAsInt32(1) << endl;
            }
        }
//    }
}

void ThirdRoomApp::draw(){
    gl::clear( Color( 0, 0, 0 ) );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glDisable(GL_LIGHTING);
    gl::disableDepthWrite();
    gl::disableDepthRead();
   // gl::popMatrices();
    gl::enableAlphaBlending();

    gl::rotate(roomRotation);
    drawRoomSkeleton(roomWidth, roomHeight, roomLength);
   // gl::pushMatrices();
    gl::translate(textVec);
    gl::rotate(Vec3f(0, 180, 180));
    gl::drawStringCentered(hello, Vec2f(getWindowWidth()/2.0f, getWindowHeight()/2.0f), textColor, mFont);
    gl::drawStringCentered(welcome, Vec2f(getWindowWidth()/2.0f, getWindowHeight()/2.0f), helloColor, mFont);
    
    gl::enableDepthWrite();
    gl::enableDepthRead();
    gl::setMatrices( mayaCam.getCamera() );

    
   // gl::color(ColorA(0.0, 1.0, 0.0, 1.0));
   // gl::drawLine(Vec3f(sequencerPosition, -40, -55), Vec3f(sequencerPosition, -40, 55));
  //  gl::pushModelView();
    for(int i = 0; i < users.size(); i++){
        users[i].display();
        
    }
   // gl::popModelView();
    //gl::pushMatrices();
    //gl::rotate(roomRotation);
    GLfloat light_position[] = { -30.0f, 0.0f, 40.0f, 1.0 };
    GLfloat light1_position[] = {30.0f, 0.0f, 40.0f, 1.0 };
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_COLOR_MATERIAL);
    
    glLightfv( GL_LIGHT0, GL_POSITION, light_position );
    glLightfv( GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv( GL_LIGHT1, GL_SHININESS, mat_shininess);
    glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    //  glMaterialfv( GL_FRONT, GL_EMISSION, mat_emission );
    
    for(int i = 0; i < instruments.size(); i++){
        Instrument* in = *(instruments.begin()+i);
        glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, in->getColor() );
        in->display();
    }
    if(mesh.getNumVertices() > 0){
        for(int i = 0; i < mesh.getNumVertices(); i++){
            mesh.appendTriangle(i, (i+2)%mesh.getNumVertices(), (i+4)%mesh.getNumVertices());
            mesh.appendTriangle(i, (i+1)%mesh.getNumVertices(), (i+3)%mesh.getNumVertices());
            for(int j = mesh.getNumVertices(); j > 0; j--){

                mesh.appendTriangle(i, j, ((j+i)%mesh.getNumVertices()));

            }
            
        }
        AxisAlignedBox3f b = mesh.calcBoundingBox();
        Vec3f v = b.getSize();
        sendGroupParametersOsc(v.x, v.y, v.z);
        gl::draw(mesh);
        
    }
    //gl::popMatrices();

    InterfaceGl::draw();
    if( mMovieWriter )
        mMovieWriter.addFrame( copyWindowSurface() );
    glFlush();
    
}

void ThirdRoomApp::wallHit(Ball *ball){
    for(int i = 0; i < totalBoxes; i++){
        if(grid[i].withinBounds(ball->getPosition())){
            animateBox(&grid[i]);
            //ball->wallHit(grid[i].wallID);
            if(grid[i].wallID == ceiling){
                sendOscCeiling(grid[i].wallID, grid[i].note, abs(ball->getVelocity().y));
               // cout << abs(ball->getVelocity().y) << endl;
            }
            else{
                sendOscWall(grid[i].wallID, ball->getNote());
            }
        }
    }
}


void ThirdRoomApp::animateBox(Box* box){
    
    //timeline().apply( &box->mSize, Vec3f(10, 10, 10), .05f, EaseInQuad() );
    timeline().apply( &box->mColor, ColorA(0, 0, 1.0f, 1.0f), .50f, EaseInQuad() );
    
//    if(box->wallID == frontWall || box->wallID == backWall){
//        timeline().appendTo( &box->mSize, Vec3f(10, 10, 0), .10f, EaseOutQuad() );
//    }
//    else if(box->wallID == leftWall || box->wallID == rightWall){
//        timeline().appendTo( &box->mSize, Vec3f(0, 10, 10), .10f, EaseOutQuad() );
//    }
//    else if (box->wallID == floor || box->wallID == ceiling){
//        timeline().appendTo( &box->mSize, Vec3f(10, 0, 10), .10f, EaseOutQuad() );
    //}
    
    if(box->isOn()){
   //     timeline().appendTo( &box->mColor, ColorA(1.0, 1.0, 0.0, .6), .10f, EaseOutQuad() );
    }
    else{
     //   timeline().appendTo( &box->mColor, ColorA(1.0, 1.0, 1.0, 0.0f), .10f, EaseInQuad() );
    }
}

//***********************************************************************
//Sending OSC functions
//***********************************************************************

void ThirdRoomApp::sendOscWall(int _wall, int _note){
    osc::Message msg;
    msg.setAddress("/wall");
    msg.setRemoteEndpoint(host, port);
    msg.addIntArg(_wall);
    msg.addIntArg(_note);
    oscSender.sendMessage(msg);
}

void ThirdRoomApp::sendOscCeiling(int _wall, int _note, float velocity){
    osc::Message msg;
    msg.setAddress("/ceiling");
    msg.setRemoteEndpoint(host, port);
    msg.addIntArg(_wall);
    msg.addIntArg(_note);
    msg.addFloatArg(velocity);
    oscSender.sendMessage(msg);
}

void ThirdRoomApp::sendOscSequence(int step, int state){
    osc::Message msg;
    msg.setAddress("/setSequence");
    msg.setRemoteEndpoint(host, port);
    msg.addIntArg(step);
    msg.addIntArg(state);
    oscSender.sendMessage(msg);
}

void ThirdRoomApp::sendOscSequence2(int step, int state){
    osc::Message msg;
    msg.setAddress("/setSequence2");
    msg.setRemoteEndpoint(host, port);
    msg.addIntArg(step);
    msg.addIntArg(state);
    oscSender.sendMessage(msg);
}

void ThirdRoomApp::sendGroupNote(float note){
    cout << note << endl;
    osc::Message msg;
    msg.setAddress("/groupNote");
    msg.setRemoteEndpoint(host, port);
    msg.addFloatArg(note);
    oscSender.sendMessage(msg);
}

void ThirdRoomApp::sendGroupNoteOff(){
    osc::Message msg;
    msg.setAddress("/groupNoteOff");
    msg.setRemoteEndpoint(host, port);
    msg.addIntArg(1);
    oscSender.sendMessage(msg);
}

void ThirdRoomApp::sendGroupParametersOsc(float x, float y, float z){
    osc::Message msg;
    msg.setAddress("/groupParams");
    msg.setRemoteEndpoint(host, port);
    msg.addFloatArg(x);
    msg.addFloatArg(y);
    msg.addFloatArg(z);
    oscSender.sendMessage(msg);
}

void ThirdRoomApp::sendOscScreen(float one, float two, float three){
    osc::Message msg;
    msg.setAddress("/screen");
    msg.setRemoteEndpoint(host, port);
    msg.addFloatArg(one);
    msg.addFloatArg(two);
    msg.addFloatArg(three);
    oscSender.sendMessage(msg);
}
/************************************************************************/
/************************************************************************/

void ThirdRoomApp::drawRoomSkeleton(float width,float height, float length){
    
    //gl::color(1.0f, 1.0f, 1.0f, .3f);
    // gl::drawCube(Vec3f(0, 0, 0), Vec3f(width, height, length));
    for(int i = 0; i < totalBoxes; i++){
        
        if(grid[i].mColor.value() != ColorA(1.0, 1.0, 1.0, 0.0)){
//            if(roomRotation.value() == Vec3f(0.0, 0.0, 0.0) && grid[i].states[0][0]){
//                //grid[i].mColor = ColorA(1.0, 1.0, 0.0, .7);
//                gl::color(grid[i].mColor);
//            }
//            else if(roomRotation.value() == Vec3f(0.0, 0.0, 360.0) && grid[i].states[1][0]){
//                //grid[i].mColor = ColorA(0.0, 1.0, 0.0, .7);
//                gl::color(grid[i].mColor);
//            }
            gl::color(grid[i].mColor);
            gl::drawStrokedCube(grid[i].mPos, grid[i].mSize);
           // gl::drawColorCube(grid[i].mPos, grid[i].mSize);
            //gl::drawCube(grid[i].mPos, grid[i].mSize);
        }
        //gl::drawSphere(grid[i].mPos, .5);
    }
    
}
/****************************************************************************************/
/****************************************************************************************/
/* Setting up the Room Grid, made out of Box objects */


void ThirdRoomApp::setupGrid(float w, float h, float l){
    int subGrid = 0;
    int subCount = 0;
    int count = 0;
    //ceiling and floor
    for(int z= -(l/2); z < (l/2); z+=10){
        for(int x = -(w/2); x < (w/2); x+=10){
            grid[count].mPos = Vec3f(x+5, (h/2)-5, z);
            grid[count].mSize = Vec3f(10, 0, 10);
            grid[count].id = count;
            grid[count].wallID = ceiling;
            grid[count].note = subGrid;
          //  cout << grid[count].id << " " << grid[count].note << endl;
            count++;
            subCount++;
            if(subCount%3 == 0){
                subGrid++;
            }
            if(subCount < 36){
                subGrid = subGrid%4;
            }
            else if(subCount >= 36 && subCount <71){
                subGrid = 4+ subGrid%4;
            }
            else if(subCount >= 72 && subCount <107){
                subGrid = 8 + subGrid%4;
            }
            else if(subCount >= 108 && subCount < 144){
                subGrid = 12 + subGrid%4;
            }
            
            
        }
    }
    
    for(int z= -(l/2); z < (l/2); z+=10){
        for(int x = (w/2); x > -(w/2); x-=10){
            grid[count].mPos = Vec3f(x-5, (-h/2)-5, z);
            grid[count].mSize = Vec3f(10, 0, 10);
            grid[count].id = count;
            grid[count].wallID = floor;
            count++;
        }
    }
    //front wall
    for(int y = -(h/2); y < (h/2); y+=10){
        for(int x = -(w/2); x < (w/2); x+=10){
            grid[count].mPos = Vec3f(x+5, y, (l/2)-5);
            grid[count].mSize = Vec3f(10, 10, 0);
            grid[count].id = count;
            grid[count].wallID = frontWall;
            count++;
        }
    }
    //back wall
    for(int y = -(h/2); y < (h/2); y+=10){
        for(int x = -(w/2); x < (w/2); x+=10){
            grid[count].mPos = Vec3f(x+5, y, (-l/2)-5);
            grid[count].mSize = Vec3f(10, 10, 0);
            grid[count].id = count;
            grid[count].wallID = backWall;
            count++;
        }
    }
    
    for(int y = -(h/2); y < (h/2); y+=10){
        for(int z = -(l/2); z < (l/2); z+=10){
            grid[count].mPos = Vec3f(-w/2, y, z);
            grid[count].mSize = Vec3f(0, 10, 10);
            grid[count].id = count;
            grid[count].wallID = leftWall;
            count++;
        }
    }
    for(int y = -(h/2); y < (h/2); y+=10){
        for(int z = -(l/2); z < (l/2); z+=10){
            grid[count].mPos = Vec3f(w/2, y, z);
            grid[count].mSize = Vec3f(0, 10, 10);
            grid[count].id = count;
            grid[count].wallID = rightWall;
            count++;
        }
    }

    
    while(count < 1400){
        grid[count].mSize = Vec3f(0, 0, 0);
        grid[count].mPos = Vec3f(1000, 1000, 1000);
        grid[count].id = count;
        count++;
        
    }
}


void ThirdRoomApp::introduction(){
    timeline().appendTo(&textColor, ColorA(1.0f, 1.0f, 1.0f, 1.0f), 1.5, EaseInQuad());
    timeline().appendTo(&textColor, ColorA(1.0f, 1.0f, 1.0f, 0.0f), 1.5, EaseInQuad());
    
        intro = false;
    
}

double ThirdRoomApp::nowMinus(double time){
    return getElapsedSeconds() - time;
}

void ThirdRoomApp::shutdown(){
    mShouldQuit = true;
    mThread.join();
   // mOscThread.join();
}

CINDER_APP_BASIC( ThirdRoomApp, RendererGl )
