#ifndef SNAKE_H_DEFINED
#define SNAKE_H_DEFINED

struct Element
{
  enum Type { OUTSIDE, INSIDE, CONTOUR };

  Type  type;
  double theta;  /* angle to x-axis, 0 to TWOPI */
  double xpos;   /* x-position of node */
  double ypos;   /* y-position of node */
};

struct Vector
{
  double x;
  double y;
};

class Snake
{
public:
  Snake(int l, double sp);
  ~Snake();

  int getLength() const { return itsLength; }
  double getSpacing() const { return itsSpacing; }
  void getElement(int n, double* x, double* y, double* theta) const;

private:
  const int itsLength;
  const double itsSpacing;
  Element* const itsElem;
  double* const itsDeltas;  /* difference of thetas */

  void center();
  void computeDeltaTheta();
  void jiggle();
  void replaceNodes(int i1, const Vector& new1,
                    int n2, const Vector& new2);
};

#endif // !SNAKE_H_DEFINED
