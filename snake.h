#ifndef SNAKE_H_DEFINED
#define SNAKE_H_DEFINED

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
  ~Snake();

  int getLength() const { return itsLength; }
  float getSpacing() const { return itsSpacing; }
  int getElement(int n, float* x, float* y, float* theta) const;

private:
  const int itsLength;
  const float itsSpacing;
  Element* const itsElem;

  void center();
  void newDeltaTheta();
  void jiggle();
  void newNodes(int ni, int nj, Vector new_no_i, Vector new_no_j);
};

#endif // !SNAKE_H_DEFINED
