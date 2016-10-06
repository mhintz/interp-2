#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class reaction_diffusionApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void reaction_diffusionApp::setup()
{
}

void reaction_diffusionApp::mouseDown( MouseEvent event )
{
}

void reaction_diffusionApp::update()
{
}

void reaction_diffusionApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( reaction_diffusionApp, RendererGl )
