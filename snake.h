#ifndef SNAKE_H_DEFINED
#define SNAKE_H_DEFINED

#define MAX_FOREG_NUMBER     128

struct Element
{
  enum Type { OUTSIDE, INSIDE, CONTOUR };

  Type  type;
  float theta;  /* angle to x-axis, 0 to TWOPI */
  float xpos;   /* x-position of node */
  float ypos;   /* y-position of node */
  float delta;  /* difference of thetas */
};

struct Vector
{
  float x;
  float y;
};

class Snake
{
public:
  Snake(int l, float sp);

  int getLength() const { return length; }
  float getSpacing() const { return spacing; }
  int getElement(int n, float* x, float* y, float* theta) const;

  const int length;
  const float spacing;
  Element foreg[ MAX_FOREG_NUMBER ];

private:
  void center();
  void newDeltaTheta();
  void jiggle();
  void newNodes(int ni, int nj, Vector new_no_i, Vector new_no_j);
};

#endif // !SNAKE_H_DEFINED
