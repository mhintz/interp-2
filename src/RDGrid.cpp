#include "RDGrid.hpp"

#include <glm/gtc/constants.hpp>

using std::vector;

RDGrid::RDGrid(int width, int height):
mWidth(width), mHeight(height) {
	mGrid = vector<Color>(width * height, Color(0.0f, 1.0f, 0.0f));
	mViewTex = gl::Texture2d::create(width, height);
}

Color & RDGrid::indexGrid(int x, int y) {
	return mGrid[x + mWidth * y];
}

void RDGrid::setupCircle(int rad) {
	vec2 center = vec2(mWidth / 2.0f, mHeight / 2.0f);
	int steps = 100;
	float angleStep = 2.0f * glm::pi<float>() / (float) steps;
	for (int s = 0; s < steps; s++) {
		vec2 radialDirection = normalize(vec2(cos(s * angleStep), sin(s * angleStep)));
		for (int mul = -1; mul <= 1; mul++) {
			vec2 pos = center + radialDirection * (float) rad + radialDirection * (float) mul;
			indexGrid((int) pos.x, (int) pos.y) = Color(0.0f, 0.0f, 1.0f);
		}
	}
}

float convoluteA(float ul, float u, float ur, float l, float c, float r, float bl, float b, float br) {
	return (
		0.05f * ul +
		0.2f * u +
		0.05f * ur +
		0.2f * l +
		-1.f * c +
		0.2f * r +
		0.05f * bl +
		0.2f * b +
		0.05f * br
	);
}

float convoluteB(float ul, float u, float ur, float l, float c, float r, float bl, float b, float br) {
	return (
		0.05f * ul +
		0.2f * u +
		0.05f * ur +
		0.2f * l +
		-1.f * c +
		0.2f * r +
		0.05f * bl +
		0.2f * b +
		0.05f * br
	);
}

static float diffusionRateA = 1.0f;
static float diffusionRateB = 0.5f;
// static float feedRateA = 0.055;
// static float killRateB = 0.062;
static float feedRateA = 0.0545;
static float killRateB = 0.062;

// What do you do at the edges of the image?
void RDGrid::update() {
	auto newGrid = vector<Color>(mWidth * mHeight, Color(0.0f, 1.0f, 0.0f));
	int const buffer = 1;
	for (int x = buffer; x < mWidth - buffer; x++) {
		for (int y = buffer; y < mHeight - buffer; y++) {
			float curA = indexGrid(x, y).g;
			float curB = indexGrid(x, y).b;
			float ABB = curA * curB * curB;
			float diffA = diffusionRateA * convoluteA(indexGrid(x-1, y-1).g, indexGrid(x, y-1).g, indexGrid(x+1, y-1).g, indexGrid(x-1, y).g, indexGrid(x, y).g, indexGrid(x+1, y).g, indexGrid(x-1, y+1).g, indexGrid(x, y+1).g, indexGrid(x+1, y+1).g);
			float diffB = diffusionRateB * convoluteB(indexGrid(x-1, y-1).b, indexGrid(x, y-1).b, indexGrid(x+1, y-1).b, indexGrid(x-1, y).b, indexGrid(x, y).b, indexGrid(x+1, y).b, indexGrid(x-1, y+1).b, indexGrid(x, y+1).b, indexGrid(x+1, y+1).b);

			// newGrid[x + mWidth * y].g = curA;
			// newGrid[x + mWidth * y].b = curB;

			newGrid[x + mWidth * y].g = curA + (diffA - ABB + feedRateA * (1.0f - curA));
			newGrid[x + mWidth * y].b = curB + (diffB + ABB - (feedRateA + killRateB) * curB);

			// newGrid[x + mWidth * y].g = curA + diffA;
			// newGrid[x + mWidth * y].b = curB + diffB;
		}
	}
	mGrid = newGrid;
}

void RDGrid::draw() {
	mViewTex->update(mGrid.data(), GL_RGB, GL_FLOAT, 0, mWidth, mHeight);
	gl::draw(mViewTex);
}