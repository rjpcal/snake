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
  Element getElement(int n) const;

private:
  const int itsLength;
  const double itsSpacing;
  Element* const itsElem;

  void center();
  void computeTheta();
  void jiggle();
  void replaceNodes(int i1, const Vector& new1,
                    int n2, const Vector& new2);
};

#endif // !SNAKE_H_DEFINED
