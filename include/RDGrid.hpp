#include <memory>
#include <vector>

#include "cinder/Color.h"

using namespace ci;

class RDGrid;
typedef std::shared_ptr<RDGrid> RDGridPtr;
typedef std::unique_ptr<RDGrid> RDGridBox;

class RDGrid {
public:
  RDGrid(int width, int height);

  void setupCircle(int rad);
  Color & indexGrid(int x, int y);

  void update();
  void draw();

private:
  std::vector<Color> mGrid;
  gl::Texture2dRef mViewTex;
  int mWidth, mHeight;
};
