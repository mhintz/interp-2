#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "RDGrid.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class ReactionDiffusionApp : public App {
  public:
	void setup() override;
	void update() override;
	void draw() override;

	RDGridPtr theGrid;
};

void ReactionDiffusionApp::setup()
{
	theGrid = RDGridPtr(new RDGrid(getWindowWidth(), getWindowHeight()));
	theGrid->setupCircle(20.0f);
}

void ReactionDiffusionApp::update()
{
	theGrid->update();
}

void ReactionDiffusionApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );

	theGrid->draw();

	std::cout << getAverageFps() << std::endl;
}

CINDER_APP( ReactionDiffusionApp, RendererGl )
