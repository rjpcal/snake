#ifndef SNAKE_H_DEFINED
#define SNAKE_H_DEFINED

struct Vector
{
  double x;
  double y;
};

struct Element
{
  enum Type { OUTSIDE, INSIDE, CONTOUR };

  Vector pos;    /* position of node */
  double theta;  /* angle to x-axis, 0 to TWOPI */
  Type  type;
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

  Element& elem(int i) const
  {
    i = i % itsLength;
    if (i < 0) i += itsLength;
    return itsElem[i];
  }

  void center();
  void computeTheta();
  void jiggle();
  void replaceNodes(int i1, const Vector& new1,
                    int n2, const Vector& new2);
};

#endif // !SNAKE_H_DEFINED
