#pragma once

#include <memory>
#include <vector>

#include "cinder/Color.h"

using namespace ci;

class RDGrid;
typedef std::shared_ptr<RDGrid> RDGridPtr;
typedef std::unique_ptr<RDGrid> RDGridBox;

class RDGrid {
public:
	static RDGridPtr create(int width, int height);

  RDGrid(int width, int height);

  void setupCircle(int rad);
  Color & indexGrid(int x, int y);

  void update();
  void draw();

private:
  int mWidth, mHeight;
  gl::FboRef mSourceFbo;
  gl::FboRef mDestFbo;

  gl::GlslProgRef mRDProgram;
  gl::GlslProgRef mRenderRDProgram;

  int mRDReadFboBinding = 0;
  int mRDRenderFboBinding = 1;
};
