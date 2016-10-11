#include "RDGrid.hpp"

#include <glm/gtc/constants.hpp>

using std::vector;

RDGridPtr RDGrid::create(int width, int height) {
	return RDGridPtr(new RDGrid(width, height));
}

RDGrid::RDGrid(int width, int height):
mWidth(width), mHeight(height) {
	gl::Texture2d::Format colorTextureFormat = gl::Texture2d::Format()
		.internalFormat(GL_RGB32F)
		.wrap(GL_REPEAT);
	gl::Fbo::Format gridFboFmt = gl::Fbo::Format().colorTexture(colorTextureFormat);
	mSourceFbo = gl::Fbo::create(width, height, gridFboFmt);
	mDestFbo = gl::Fbo::create(width, height, gridFboFmt);

	mRDProgram = gl::GlslProg::create(app::loadAsset("v_passThrough.glsl"), app::loadAsset("f_reactionDiffusion.glsl"));
	mRDProgram->uniform("gridWidth", mWidth);
	mRDProgram->uniform("gridHeight", mHeight);
	mRDProgram->uniform("uPrevFrame", mRDReadFboBinding);

	mRenderRDProgram = gl::GlslProg::create(app::loadAsset("v_passThrough.glsl"), app::loadAsset("f_renderGrid.glsl"));
	mRenderRDProgram->uniform("uGridSampler", mRDRenderFboBinding);
}

Color & goIndexGrid(vector<Color> * grid, int width, int x, int y) {
	return (*grid)[x + width * y];
}

void RDGrid::setupCircle(int rad) {
	auto initVector = vector<Color>(mWidth * mHeight, Color(0.0f, 1.0f, 0.0f));

	vec2 center = vec2(mWidth / 2.0f, mHeight / 2.0f);
	int const steps = 100;
	float angleStep = 2.0f * glm::pi<float>() / (float) steps;
	for (int s = 0; s < steps; s++) {
		vec2 radialDirection = normalize(vec2(cos(s * angleStep), sin(s * angleStep)));
		for (int mul = -4; mul <= 4; mul++) {
			vec2 pos = center + radialDirection * (float) rad + radialDirection * (float) mul;
			int x = (int) pos.x;
			int y = (int) pos.y;
			goIndexGrid(& initVector, mWidth, x, y) = Color(0.0f, 0.0f, 1.0f);
		}
	}

	auto initTexture = gl::Texture2d::create(initVector.data(), GL_RGB, mWidth, mHeight, gl::Texture2d::Format().dataType(GL_FLOAT).internalFormat(GL_RGB32F));

	gl::ScopedFramebuffer scpFB(mSourceFbo);

	gl::clear(Color( 0, 1, 1 ));

	gl::setMatricesWindow(mWidth, mHeight);

	gl::draw(initTexture, Rectf(0, 0, mWidth, mHeight));
}

// What do you do at the edges of the image?
void RDGrid::update() {
	gl::ScopedTextureBind scpTex(mSourceFbo->getColorTexture(), mRDReadFboBinding);
	gl::ScopedGlslProg scpShader(mRDProgram);

	gl::ScopedFramebuffer scpFB(mDestFbo);

	gl::drawSolidRect(Rectf(0, 0, mWidth, mHeight));

	std::swap(mSourceFbo, mDestFbo);
}

void RDGrid::draw() {
	gl::ScopedTextureBind scpTex(mDestFbo->getColorTexture(), mRDRenderFboBinding);
	gl::ScopedGlslProg scpShader(mRenderRDProgram);

	gl::drawSolidRect(Rectf(0, 0, mWidth, mHeight));

	// gl::draw(mDestFbo->getColorTexture(), Rectf(0, 0, mWidth, mHeight));
}