#ifndef SNAKE_H_DEFINED
#define SNAKE_H_DEFINED

struct Vector
{
  Vector(double x_ = 0.0, double y_ = 0.0) : x(x_), y(y_) {}

  double x;
  double y;
};

struct Element
{
  enum Type { OUTSIDE, INSIDE, CONTOUR };

  Element() {}

  Element(double x, double y, double th, Type t) :
    pos(x,y), theta(th), type(t)
  {}

  Element(const Vector& v, double th, Type t) :
    pos(v), theta(th), type(t)
  {}

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
  void jiggle();
  void transformPath(int i1, const Vector& new1,
                     int n2, const Vector& new2);
};

#endif // !SNAKE_H_DEFINED
