#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "RDGrid.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class ReactionDiffusionApp : public App {
public:
	static void prepSettings(Settings * settings);
	void setup() override;
	void update() override;
	void draw() override;

	RDGridPtr mTheGrid;
};

void ReactionDiffusionApp::prepSettings(Settings * settings) {
	// settings->setFullScreen();
	settings->setTitle("Reaction Diffusion Interpretation");
	// settings->setHighDensityDisplayEnabled();
}

void ReactionDiffusionApp::setup()
{
	mTheGrid = RDGrid::create(getWindowWidth(), getWindowHeight());
	mTheGrid->setupCircle(20.0f);
}

void ReactionDiffusionApp::update()
{
	for (int i = 0; i < 12; i++) {
	    mTheGrid->update();
	}
}

void ReactionDiffusionApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );

	mTheGrid->draw();

	gl::drawString(std::to_string(getAverageFps()), vec2(10.0f, 20.0f), ColorA(0.0f, 0.0f, 0.0f, 1.0f));
}

CINDER_APP( ReactionDiffusionApp, RendererGl, & ReactionDiffusionApp::prepSettings )
