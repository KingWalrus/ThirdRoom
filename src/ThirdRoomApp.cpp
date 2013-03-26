//*******************************************************************
//*************************             *****************************
//                         The Third Room
//*************************             *****************************
//*******************************************************************
//By: Colin Honigman (Programmer/Designer)
//    Andrew Walton  (Designer)
//    Ajay Kapur phd (Sponsor)
//*******************************************************************
//*******************************************************************

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"
#include "cinder/MayaCamUI.h"
#include "cinder/Timeline.h"

#include "OscListener.h"
#include "OscSender.h"

#include <vector>
#include <math.h>
#include <stdlib.h>

#include "Bounce.h"

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
        if(objectPosition.distance(mPos) < 7){
            return true;
        }
        else return false;
    };
    
};

class ThirdRoomApp : public AppBasic {
public:
    void prepareSettings( Settings *settings);
	void setup();
	void update();
	void draw();
    void mouseDown( MouseEvent event );
    void drawRoomSkeleton(float width, float length, float height); //draw a room skeleton (for developing) custom width/length/height for installation room
    void setupGrid(float w, float h, float l);
    
    void wallHit(Bounce& balls);
    void animateBox(Box* box);
    
private:
    float roomWidth, roomLength, roomHeight; //variables for room sizes
    float eyePointx, eyePointy, eyePointz;
    float centerPointx, centerPointy, centerPointz;
    Anim<Vec3f>	eyePoint;
    Anim<Vec3f>	centerPoint;
    bool loaded;
    
    Box grid[600];
    
    bool animateGrid;
    int county;
    bool hit;
    bool lookat;
    Vec3f lookie;
    CameraPersp cam;
    
protected:
    MayaCamUI mayaCam; //mayaCam to move through 3d space
    Matrix44f mTransform;
    Vec2i mousePos;
    double mTime;
    InterfaceGl mParams;
    Bounce ball;
    
};

void ThirdRoomApp::prepareSettings( Settings *settings ){
    settings->setWindowSize(1024, 768);
    settings->enableSecondaryDisplayBlanking( false );
}

void ThirdRoomApp::setup(){
    //setup viewing camera
    eyePoint = Vec3f(0, 100, -500);
    centerPoint = Vec3f(0, 0, 50);
    eyePointx = 0;
    eyePointy =  -20;
    eyePointz =  -50;
    centerPointx = 0;
    centerPointy = 0;
    centerPointz = 50;
    
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
    mParams.addParam("LookAt", &lookat);
    mParams.addParam("Look At What?", &lookie, "min=-100 max=100");
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
    ball = Bounce();
    
    lookat = false;
    
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
        timeline().apply( &eyePoint, Vec3f(0, -20, -50), 5.0f, EaseInQuad() );
        loaded = true;
    }
    else{
        
        eyePoint = Vec3f(eyePointx, eyePointy, eyePointz);
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
    if(hit){
        
        ball.setVelocity(rand()%5, rand()%5, rand()%5);
        hit = false;
    }
    ball.update();
    wallHit(ball);
    
}

void ThirdRoomApp::draw(){
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    gl::pushMatrices();
    gl::setMatrices( mayaCam.getCamera() );
    drawRoomSkeleton(roomWidth, roomHeight, roomLength);
    ball.display();
    InterfaceGl::draw();
    // gl::drawSphere(Vec3f(100, -100, 60), 10);
    
}

void ThirdRoomApp::wallHit(Bounce &balls){
    for(int i = 0; i < 520; i++){
        if(grid[i].withinBounds(balls.getPosition())){
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
    timeline().apply( &box->mColor, ColorA((rand()%100)*.01, (rand()%100)*.01, (rand()%100)*.01, 1.0f), .10f, EaseInQuad() );
    timeline().appendTo( &box->mColor, ColorA(1.0f, 1.0f, 1.0f, 1.0f), .10f, EaseInQuad() );
    
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