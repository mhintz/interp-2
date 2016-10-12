#include <memory>
#include <vector>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Color.h"

#include "glm/gtc/constants.hpp"

using namespace ci;
using namespace ci::app;

class ReactionDiffusionApp : public App {
public:
	static void prepSettings(Settings * settings);
	void setup() override;
	void update() override;
	void draw() override;

	void drawVectorToFBO(std::vector<Color> const & pixelBuffer);
	void setupCircleRD(int rad);
	void setupSquareRD(int side);

	void updateRD();
	void drawRD();

	int mWidth, mHeight;
	gl::FboRef mSourceFbo;
	gl::FboRef mDestFbo;

	gl::GlslProgRef mRDProgram;
	gl::GlslProgRef mRenderRDProgram;

	int mRDReadFboBinding = 0;
	int mRDRenderFboBinding = 1;
};

void ReactionDiffusionApp::prepSettings(Settings * settings) {
	// settings->setFullScreen();
	settings->setTitle("Reaction Diffusion Interpretation");
	// settings->setHighDensityDisplayEnabled();
}

void ReactionDiffusionApp::setup()
{
	mWidth = getWindowWidth();
	mHeight = getWindowHeight();

	gl::Texture2d::Format colorTextureFormat = gl::Texture2d::Format().internalFormat(GL_RGB32F).wrap(GL_REPEAT);
	gl::Fbo::Format gridFboFmt = gl::Fbo::Format().colorTexture(colorTextureFormat);
	mSourceFbo = gl::Fbo::create(mWidth, mHeight, gridFboFmt);
	mDestFbo = gl::Fbo::create(mWidth, mHeight, gridFboFmt);

	mRDProgram = gl::GlslProg::create(loadAsset("v_passThrough.glsl"), loadAsset("f_reactionDiffusion.glsl"));
	mRDProgram->uniform("gridWidth", mWidth);
	mRDProgram->uniform("gridHeight", mHeight);
	mRDProgram->uniform("uPrevFrame", mRDReadFboBinding);

	mRenderRDProgram = gl::GlslProg::create(loadAsset("v_passThrough.glsl"), loadAsset("f_renderGrid.glsl"));
	mRenderRDProgram->uniform("uGridSampler", mRDRenderFboBinding);

	gl::setMatricesWindow(mWidth, mHeight);

	// setupCircleRD(20.0f);
	setupSquareRD(40);
}

void ReactionDiffusionApp::update()
{
	// Update the reaction-diffusion system multiple times per frame to speed things up
	for (int i = 0; i < 10; i++) {
		// Bind the source FBO to read the previous state
		gl::ScopedTextureBind scpTex(mSourceFbo->getColorTexture(), mRDReadFboBinding);
		// Bind the destination FBO to receive the new state
		gl::ScopedFramebuffer scpFB(mDestFbo);
		// Bind the update program
		gl::ScopedGlslProg scpShader(mRDProgram);
		// Draw a full screen rectangle
		gl::drawSolidRect(getWindowBounds());
		// Swap the source and destination FBOs for the next frame
		std::swap(mSourceFbo, mDestFbo);
	}
}

void ReactionDiffusionApp::draw()
{
	gl::clear(Color(0, 0, 0));

	// For debugging initial state
	// gl::ScopedTextureBind scpTex(mSourceFbo->getColorTexture(), mRDRenderFboBinding);

	// Draws the reaction-diffusion FBO
	gl::ScopedTextureBind scpTex(mDestFbo->getColorTexture(), mRDRenderFboBinding);
	// The reaction-diffusion render shader
	gl::ScopedGlslProg scpShader(mRenderRDProgram);
	// Draw a full screen rectangle
	gl::drawSolidRect(getWindowBounds());

	// Draw the framerate
	gl::drawString(std::to_string(getAverageFps()), vec2(10.0f, 20.0f), ColorA(0.0f, 0.0f, 0.0f, 1.0f));
}

Color & idxGrid(std::vector<Color> * grid, int width, int x, int y) {
	return (*grid)[x + width * y];
}

void ReactionDiffusionApp::drawVectorToFBO(std::vector<Color> const & pixelBuffer) {
	auto initTexFmt = gl::Texture2d::Format().dataType(GL_FLOAT).internalFormat(GL_RGB32F);
	auto initTexture = gl::Texture2d::create(pixelBuffer.data(), GL_RGB, mWidth, mHeight, initTexFmt);

	gl::ScopedFramebuffer scpFB(mSourceFbo);
	gl::draw(initTexture, Rectf(0, 0, mWidth, mHeight));
}

void ReactionDiffusionApp::setupCircleRD(int rad) {
	auto initVector = std::vector<Color>(mWidth * mHeight, Color(0.0f, 1.0f, 0.0f));

	vec2 center = vec2(mWidth / 2.0f, mHeight / 2.0f);
	int const steps = 100;
	float angleStep = 2.0f * glm::pi<float>() / (float) steps;
	for (int s = 0; s < steps; s++) {
		vec2 radialDirection = normalize(vec2(cos(s * angleStep), sin(s * angleStep)));
		for (int mul = -4; mul <= 4; mul++) {
			ivec2 pos(center + radialDirection * (float) rad + radialDirection * (float) mul);
			idxGrid(& initVector, mWidth, pos.x, pos.y) = Color(0.0f, 0.0f, 1.0f);
		}
	}

	drawVectorToFBO(initVector);
}

void ReactionDiffusionApp::setupSquareRD(int side) {
	auto initVector = std::vector<Color>(mWidth * mHeight, Color(0.0f, 1.0f, 0.0f));

	ivec2 const center = ivec2(mWidth / 2, mHeight / 2);
	int const halfSide = side / 2;

	// Iterate modifier = -1, modifier = 1
	for (int modifier = -1; modifier <= 1; modifier += 2) {
		// Will be upper left, then lower right
		ivec2 const start = center + ivec2(modifier * halfSide, modifier * halfSide);
		int pad = 4;

		// Iterate along x-direction, fill out along y-direction
		for (int xoff = -pad; xoff <= side + pad; xoff++) {
			for (int yoff = -pad; yoff <= pad; yoff++) {
				ivec2 pos = start + ivec2(-modifier * xoff, yoff);
				idxGrid(& initVector, mWidth, pos.x, pos.y) = Color(0.0f, 0.0f, 1.0f);
			}
		}

		// Iterate along the y-direction, fill out along x-direction
		for (int yoff = -pad; yoff <= side + pad; yoff++) {
			for (int xoff = -pad; xoff <= pad; xoff++) {
				ivec2 pos = start + ivec2(xoff, -modifier * yoff);
				idxGrid(& initVector, mWidth, pos.x, pos.y) = Color(0.0f, 0.0f, 1.0f);
			}
		}
	}

	drawVectorToFBO(initVector);
}

CINDER_APP( ReactionDiffusionApp, RendererGl, & ReactionDiffusionApp::prepSettings )
