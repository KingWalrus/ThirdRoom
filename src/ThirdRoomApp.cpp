//************************************************************************************************
//*************************             **********************************************************
//
//                         The Third Room
//
//*************************             **********************************************************
//************************************************************************************************
//By: Colin Honigman    (Programmer/Designer)
//    Andrew Walton     (Designer)
//    Ajay Kapur        (Sponsor)
//************************************************************************************************
//************************************************************************************************

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"
#include "cinder/MayaCamUI.h"
#include "cinder/Timeline.h"
#include "cinder/Font.h"
#include "cinder/cairo/Cairo.h"
#include "cinder/Text.h"
#include "cinder/gl/Texture.h"


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

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace params;

struct Box{
    Box() : mColor( Color( 1, 1, 1) ), mPos( Vec3f(0, 0, 0) ), mSize( Vec3f(10, 10, 10) ) {}
    Box( Color color, Vec3f pos, Vec3f size, int ID) : mColor( color ), mPos( pos ), mSize ( size ), id ( ID ){}
    
    int id;
    int width;
    int height;
    int length;
    int x, y, z;
    Anim<ColorA> mColor;
    Anim<Vec3f> mPos;
    Anim<Vec3f> mSize;
    
    bool withinBounds(Vec3f objectPosition){
        if(objectPosition.distance(mPos) < 5){
            return true;
        }
        else return false;
    };
    
    
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
    
    
private:
    float               roomWidth, roomLength, roomHeight; //variables for room sizes
    float               eyePointx, eyePointy, eyePointz;
    float               centerPointx, centerPointy, centerPointz;
    Anim<Vec3f>         eyePoint;
    Anim<Vec3f>         centerPoint;
    bool                loaded;
    bool                doneLoading;
    
    Box                 grid[600];
    
    bool                animateGrid;
    int                 county;
    bool                hit;
    bool                lookat;
    Vec3f               lookie;
    CameraPersp         cam;
    
    osc::Listener       oscListener;
    osc::Sender         oscSender;
    
    string              host;
    int                 port;
    vector<Instrument*> instruments;
    vector<User>       users;
    TextLayout          text;
    gl::Texture         mLabelText;
    
    
    
protected:
    MayaCamUI           mayaCam; //mayaCam to move through 3d space
    Matrix44f           mTransform;
    Vec2i               mousePos;
    double              mTime;
    InterfaceGl         mParams;
    bool                clickSwitch;
    
    enum jointNamesGlobal{
        head = 0,
        neck,
        leftShoulder,
        rightShoulder,
        leftElbow,
        rightElbow,
        leftHand,
        rightHand,
        leftHip,
        rightHip,
        leftKnee,
        rightKnee,
        leftFoot,
        rightFoot
    };
    
};

void ThirdRoomApp::prepareSettings( Settings *settings ){
    settings->setWindowSize(1024, 768);
    settings->enableSecondaryDisplayBlanking( false );
}

void ThirdRoomApp::setup(){
    //setup viewing camera
    eyePoint        = Vec3f(0, 100, 500);
    centerPoint     = Vec3f(0, 0, -50);
    eyePointx       =   0;
    eyePointy       = -20;
    eyePointz       = 50;
    centerPointx    =   0;
    centerPointy    = -20;
    centerPointz    =  -50;
    
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
    mParams.addParam("Hit", &hit);
    mParams.addParam("Switch", & clickSwitch);
    mParams.addParam("LookAt", &lookat);
    mParams.addParam("Look At What?", &lookie);
    mTransform.setToIdentity();
    loaded = false;
    animateGrid = false;
    hit = false;
    county = 0;
    for(int i = 0; i < 600; i++){
        grid[i] = Box();
    }
    roomWidth = roomLength = 100;
    roomHeight = 80;
    
    setupGrid(roomWidth, roomHeight, roomLength);
    glDisable(GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    lookat = false;
    
    host = "127.0.0.1";
    port = 13000;
    oscSender.setup(host, port);
    oscListener.setup(12000);
    
    
    text.clear(Color::white());
    text.setColor(Color(1.0f, 1.0f, 1.0f));
    try {
        text.setFont( Font( "Futura-CondensedMedium", 50 ) );
    }
    catch( ... ) {
        text.setFont( Font( "Arial", 50 ) );
    }
    mLabelText = gl::Texture( text.render( true ) );
    clickSwitch = false;
    doneLoading = false;
    
    
}

void ThirdRoomApp::mouseDown( MouseEvent event ){
    
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
            for(int i = 0; i < 600; i++){
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
    
    
    for(int i = 0; i < users.size(); i++){
        //        if(users[i].isWaving()){
        //            text.addLine("Hello");
        //            cout << "Hello" << endl;
        //            gl::draw( mLabelText, Vec2f(0,0) );
        //
        //        }
        if(users[i].isWavingLeft() && !users[i].isActive(leftHand)){
            instruments.push_back(new Ball(users[i].getJointPosition(leftHand)));
        }
        else if(users[i].isWavingRight() && !users[i].isActive(rightHand)){
            instruments.push_back(new Ball(users[i].getJointPosition(rightHand)));
        }
        
        for(int j = 0; j < instruments.size(); j++){
            
            Instrument* in = *(instruments.begin()+j);
            in->update();
            in->hitTest(&users[i]);
            wallHit((Ball*) in);
            
        }
        for(int b = 0; b < 600; b++){
            if(grid[b].withinBounds(users[i].getJointPosition(users[i].leftFoot)) || grid[b].withinBounds(users[i].getJointPosition(users[i].rightFoot))){
                timeline().apply(&grid[b].mColor, ColorA(1.0, 0, 0, 1.0), 1.50);
            }
            else{
                if((ColorA)grid[b].mColor == ColorA(1.0, 1.0, 1.0, 1.0)){
                    timeline().appendTo(&grid[b].mColor, ColorA(1.0, 1.0, 1.0, 0), 2);
                }
            }
            
        }
      users[i].update();  
    }

    //    for(int i = 0; i < instruments.size(); i++){
    //        Instrument* in = *(instruments.begin()+i);
    //        in->update();
    //        wallHit((Ball*) in);
    //
    //    }
    
    oscUpdate();
    
}


void ThirdRoomApp::oscUpdate(){
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
                    users[i].setJointPosition(users[i].leftHip,         Vec3f( message.getArgAsFloat(25),message.getArgAsFloat(26),message.getArgAsFloat(27) ));
                    users[i].setJointPosition(users[i].rightHip,        Vec3f( message.getArgAsFloat(28),message.getArgAsFloat(29),message.getArgAsFloat(30) ));
                    users[i].setJointPosition(users[i].leftKnee,        Vec3f( message.getArgAsFloat(31),message.getArgAsFloat(32),message.getArgAsFloat(33) ));
                    users[i].setJointPosition(users[i].rightKnee,       Vec3f( message.getArgAsFloat(34),message.getArgAsFloat(35),message.getArgAsFloat(36) ));
                    users[i].setJointPosition(users[i].leftFoot,        Vec3f( message.getArgAsFloat(37),message.getArgAsFloat(38),message.getArgAsFloat(39) ));
                    users[i].setJointPosition(users[i].rightFoot,       Vec3f( message.getArgAsFloat(40),message.getArgAsFloat(41),message.getArgAsFloat(42) ));
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
    }
}
void ThirdRoomApp::draw(){
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    gl::pushMatrices();
    
    
    gl::setMatrices( mayaCam.getCamera() );
    for(int i = 0; i < users.size(); i++){
        users[i].display();
    }
    for(int i = 0; i < instruments.size(); i++){
        Instrument* in = *(instruments.begin()+i);
        in->display();
    }
    
    drawRoomSkeleton(roomWidth, roomHeight, roomLength);
    InterfaceGl::draw();
    
}

void ThirdRoomApp::wallHit(Ball *ball){
    for(int i = 0; i < 520; i++){
        if(grid[i].withinBounds(ball->getPosition())){
            animateBox(&grid[i]);
        }
    }
}


void ThirdRoomApp::animateBox(Box* box){
    timeline().apply( &box->mSize, Vec3f(10, 10, 10), .05f, EaseInQuad() );
    
    
    if(box->id < 160){
        timeline().appendTo( &box->mSize, Vec3f(10, 10, 0), .10f, EaseOutQuad() );
    }
    else if(box->id >= 160 && box->id < 320){
        timeline().appendTo( &box->mSize, Vec3f(0, 10, 10), .10f, EaseOutQuad() );
    }
    else if (box->id >= 320 && box->id < 420){
        timeline().appendTo( &box->mSize, Vec3f(10, 0, 10), .10f, EaseOutQuad() );
    }
    else if (box->id >= 420 && box->id < 520){
        timeline().appendTo( &box->mSize, Vec3f(10, 0, 10), .10f, EaseOutQuad() );
    }
    timeline().apply( &box->mColor, ColorA(1.0, 1.0, 1.0, 1.0f), .10f, EaseInQuad() );
    timeline().appendTo( &box->mColor, ColorA(0, 0, 1.0f, 0.0f), .50f, EaseInQuad() );
    
}

void ThirdRoomApp::drawRoomSkeleton(float width,float height, float length){
    
    gl::color(1.0f, 1.0f, 1.0f, .3f);
    // gl::drawCube(Vec3f(0, 0, 0), Vec3f(width, height, length));
    for(int i = 0; i < 600; i++){
        gl::color(grid[i].mColor);
        gl::drawStrokedCube(grid[i].mPos, grid[i].mSize);
    }
    
}

void ThirdRoomApp::setupGrid(float w, float h, float l){
    int count = 0;
    //front wall
    for(int y = -(h/2); y < (h/2); y+=10){
        for(int x = -(w/2); x < (w/2); x+=10){
            grid[count].mPos = Vec3f(x+5, y, (l/2)-5);
            grid[count].mSize = Vec3f(10, 10, 0);
            grid[count].id = count;
            count++;
        }
    }
    //back wall
    for(int y = -(h/2); y < (h/2); y+=10){
        for(int x = -(w/2); x < (w/2); x+=10){
            grid[count].mPos = Vec3f(x+5, y, (-l/2)-5);
            grid[count].mSize = Vec3f(10, 10, 0);
            grid[count].id = count;
            count++;
        }
    }
    
    for(int y = -(h/2); y < (h/2); y+=10){
        for(int z = -(l/2); z < (l/2); z+=10){
            grid[count].mPos = Vec3f(-w/2, y, z);
            grid[count].mSize = Vec3f(0, 10, 10);
            grid[count].id = count;
            count++;
        }
    }
    for(int y = -(h/2); y < (h/2); y+=10){
        for(int z = -(l/2); z < (l/2); z+=10){
            grid[count].mPos = Vec3f(w/2, y, z);
            grid[count].mSize = Vec3f(0, 10, 10);
            grid[count].id = count;
            count++;
        }
    }
    //ceiling and floor
    for(int z= -(l/2); z < (l/2); z+=10){
        for(int x = -(w/2); x < (w/2); x+=10){
            grid[count].mPos = Vec3f(x+5, (h/2)-5, z);
            grid[count].mSize = Vec3f(10, 0, 10);
            grid[count].id = count;
            count++;
        }
    }
    
    for(int z= -(l/2); z < (l/2); z+=10){
        for(int x = -(w/2); x < (w/2); x+=10){
            grid[count].mPos = Vec3f(x+5, (-h/2)-5, z);
            grid[count].mSize = Vec3f(10, 0, 10);
            grid[count].id = count;
            count++;
        }
    }
    
    while(count < 600){
        grid[count].mSize = Vec3f(0, 0, 0);
        grid[count].mPos = Vec3f(1000, 1000, 1000);
        grid[count].id = count;
        count++;
        
    }
}


CINDER_APP_BASIC( ThirdRoomApp, RendererGl )
