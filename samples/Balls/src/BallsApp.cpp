#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "chipmunk.h"
#include "drawSpace.h"

#define GRABABLE_MASK_BIT (1<<31)
#define NOT_GRABABLE_MASK (~GRABABLE_MASK_BIT)

using namespace ci;
using namespace ci::app;
using namespace std;

class chipmunkApp : public AppBasic {
public:
    void prepareSettings( Settings *settings );
	void                setup();
	void                mouseDown( MouseEvent event );	
	void                update();
	void                draw();
    cpSpace             *space; // Holds our Space object
};


drawSpaceOptions options = {
	0,
	0,
	1,
	4.0f,
	1.0f,
	1.5f,
};

void chipmunkApp::prepareSettings( Settings *settings ) {
    settings->setWindowSize( 1080, 720 );
    settings->setFrameRate( 60.0f );
}

void chipmunkApp::setup()
{
    cpInitChipmunk();
    space = cpSpaceNew();
    cpSpaceResizeActiveHash(space, 30.0f, 1000);
	space->iterations = 10;
	space->sleepTimeThreshold = 0.5f;
    space->gravity = cpv(0, 300); 
    
    
    cpBody *staticBody = &space->staticBody;
	cpShape *shape;
    
	// Create segments around the edge of the screen.
	shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(0,getWindowHeight()), cpv(getWindowWidth(),getWindowHeight()), 0.0f));
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;
    
	shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(0,0), cpv(getWindowWidth(),0), 0.0f));
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;
    
	shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(0,0), cpv(0,getWindowHeight()), 0.0f));
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;
    
	shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(getWindowWidth(),0), cpv(getWindowWidth(),getWindowHeight()), 0.0f));
	shape->e = 1.0f; shape->u = 1.0f;
	shape->layers = NOT_GRABABLE_MASK;
    
    
    
}

void chipmunkApp::mouseDown( MouseEvent event )
{
    for (int i=0;i<10;i++)
    {
        cpBody *ballBody = cpBodyNew(100.0, cpMomentForCircle(100, 0.0f, 20, cpvzero));
        ballBody->p = cpv(event.getX(), event.getY());
        cpSpaceAddBody(space, ballBody);
        
        // Create our shape associated with the ball's body  
        
        cpShape *ballShape = cpCircleShapeNew(ballBody, 20.0, cpvzero);  
        
        ballShape->e = 0; // Elasticity  
        
        ballShape->u = 0.5; // Friction  
        
        //ballShape->data = ball; // Associate with out ball's UIImageView  
        
        ballShape->collision_type = 1; // Collisions are grouped by types  
        
        // Add the shape to out space  
        
        cpSpaceAddShape(space, ballShape);
    }
}

void chipmunkApp::update()
{
    cpSpaceStep(space, 1.0f/60.0f);
}


void chipmunkApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0.5, 0.5, 0.5 ) ); 
    drawSpace(space, &options);
}


CINDER_APP_BASIC( chipmunkApp, RendererGl )
