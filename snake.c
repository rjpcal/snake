#include "snake.h"

#include "defs.h"
#include "geom.h"

#include <math.h>
#include <stdlib.h>

#define      HIDELTA               (M_PI/4.)
#define      LODELTA              (-M_PI/4.)
#define      TEMPERATURE               0.05
#define      INCREMENT                 0.1

namespace
{
  int          FOREGROUND_ITERATION =    400;

  struct Vector
  {
    float x;
    float y;
  };

  void EllipsoidSnake( int first, int length );
  void Center_snake(Snake* s);
  void New_delta_theta(Snake* s);
  void Jiggle_snake(Snake* s);
  void Pick_nodes( int length, int ni[] );
  void Swap( int *a, int *b );
  void Get_sides( Vector no[], float a[] );
  void Get_angles( Vector no[], float alpha[], float theta[] );
  int Squash_quadrangle( Vector *no0, Vector *no1, Vector *no2, Vector *no3, Vector *new_no0, Vector *new_no1, Vector *new_no2, Vector *new_no3, float theta0, float incr );
  int New_apex( Vector *no1, Vector *no2, Vector *no3, float b, float c );
  int Monte_Carlo( float old_alpha[], float new_alpha[], float lo_alpha[], float hi_alpha[] );
  void New_nodes(Snake* s, int ni, int nj, Vector new_no_i, Vector new_no_j );

  void Center_snake(Snake* s)
  {
    float xc = 0.0f;
    float yc = 0.0f;

    for(int n = 0; n < s->length; ++n)
      {
        xc += s->foreg[n].xpos;
        yc += s->foreg[n].ypos;
      }

    xc /= s->length;
    yc /= s->length;

    for(int n = 0; n < s->length; ++n)
      {
        s->foreg[n].xpos -= xc;
        s->foreg[n].ypos -= yc;
      }
  }

  void New_delta_theta(Snake* s)
  {
    for(int n0 = 0; n0 < s->length; ++n0 )
      {
        const int n1 = (n0+1)%s->length;

        const float dx = s->foreg[n1].xpos - s->foreg[n0].xpos;
        const float dy = s->foreg[n1].ypos - s->foreg[n0].ypos;

        s->foreg[n0].theta = Zerototwopi( atan2(dy, dx) );

      }

    for(int n0 = 0; n0 < s->length; ++n0)
      {
        const int n1 = (n0+1)%s->length;

        s->foreg[n1].delta = Minuspitopi( s->foreg[n1].theta - s->foreg[n0].theta );
      }
  }



  /*        (xpos[n],ypos[n])     position n                                   */
  /*        (xpos[n+1],ypos[n+1]) position n+1                                 */
  /*        (0.5*(xpos[n]+xpos[n+1]), 0.5*(ypos[n]+ypos[n+1]))                 */
  /*                              location of patch n                          */
  /*        theta                 angle between pos n and pos n+1              */
  /*                              and orientation of patch n                   */
  /*        delta                 angle difference between patch n and n+1     */
  /*                                                                           */
  /*                      theta[0]                                             */
  /*                             no[0]__________                               */
  /*                           .     .                                         */
  /*                        .  alpha[0] .                                      */
  /*                     .                 .                                   */
  /*             a[0] .                       .  a[3]                          */
  /*               .                             .                             */
  /*            .                                   .                          */
  /*         .                                         .                       */
  /*      .                                               .     theta[3]       */
  /*no[1] alpha[1]                                           .                 */
  /*    .                                                     no[3]_________   */
  /*     .                                              alpha[3].              */
  /*      .                                                  .                 */
  /*       .                                             .                     */
  /*        .                                        .                         */
  /*         .                                   .                             */
  /*      a[1].                              .                                 */
  /*           .                         .                                     */
  /*            .                    .     a[2]                                */
  /*             .               .                                             */
  /*              . alpha[2] .                                                 */
  /*               .     .  theta[2]                                           */
  /*              no[2]__________                                              */

  float increment;

  void Jiggle_snake(Snake* s)
  {
    int n, i[4];
    int ok = 0;
    float a[4], delta[4], alpha[4], theta[4], lo_alpha[4], hi_alpha[4],
      incr, new_alpha[4], new_theta[4], new_a[4];
    Vector no[4], new_no[4];

    Pick_nodes( s->length, i );

    increment = INCREMENT;

    for( n=0; n<4; n++ )
      {
        no[n].x  = s->foreg[i[n]].xpos;
        no[n].y  = s->foreg[i[n]].ypos;
        delta[n] = s->foreg[i[n]].delta;
      }

    Get_sides( no, a );

    Get_angles( no, alpha, theta );

    for( n=0; n<4; n++ )
      {
        lo_alpha[n] = alpha[n] - ( HIDELTA - delta[n] );
        hi_alpha[n] = alpha[n] + ( delta[n] - LODELTA );
      }

    for( ; ; )
      {
        incr = ( drand48()<0.5 ) ? -increment : increment;

        switch( n = (int)( 4 * drand48() ) )
          {
          case 0:
            ok = Squash_quadrangle( &no[0], &no[1], &no[2], &no[3],
                                    &new_no[0], &new_no[1], &new_no[2], &new_no[3],
                                    theta[0], incr );
            break;

          case 1:
            ok = Squash_quadrangle( &no[1], &no[2], &no[3], &no[0],
                                    &new_no[1], &new_no[2], &new_no[3], &new_no[0],
                                    theta[1], incr );
            break;

          case 2:
            ok = Squash_quadrangle( &no[2], &no[3], &no[0], &no[1],
                                    &new_no[2], &new_no[3], &new_no[0], &new_no[1],
                                    theta[2], incr );
            break;

          case 3:
            ok = Squash_quadrangle( &no[3], &no[0], &no[1], &no[2],
                                    &new_no[3], &new_no[0], &new_no[1], &new_no[2],
                                    theta[3], incr );
            break;

          default:
            break;
          }

        if( !ok )
          {
            increment /= 2.;
            continue;
          }

        Get_sides( new_no, new_a );
        Get_angles( new_no, new_alpha, new_theta );

        if( Monte_Carlo( alpha, new_alpha, lo_alpha, hi_alpha ) )
          break;
      }

    for( n=0; n<4; n++ )
      New_nodes(s, i[n], i[(n+1)%4], new_no[n], new_no[(n+1)%4]);

    for( n=0; n<4; n++ )
      {
        s->foreg[i[n]].xpos = new_no[n].x;
        s->foreg[i[n]].ypos = new_no[n].y;
      }

    New_delta_theta(s);
  }

  void Pick_nodes( int length, int ni[] )
  {

    int i0, i1, i2, i3;

    i0 = (int)( length * drand48() );

    do
      {
        i1 = (int)( length * drand48() );
      }
    while( i1 == i0 );

    do
      {
        i2 = (int)( length * drand48() );
      }
    while( i2 == i0 || i2 == i1);

    do
      {
        i3 = (int)( length * drand48() );
      }
    while( i3 == i0 || i3 == i1 || i3 == i2 );

    if( i0 > i1 ) Swap( &i0, &i1 );
    if( i0 > i2 ) Swap( &i0, &i2 );
    if( i0 > i3 ) Swap( &i0, &i3 );
    if( i1 > i2 ) Swap( &i1, &i2 );
    if( i1 > i3 ) Swap( &i1, &i3 );
    if( i2 > i3 ) Swap( &i2, &i3 );

    ni[0] = i0;
    ni[1] = i1;
    ni[2] = i2;
    ni[3] = i3;
  }

  void Swap( int *a, int *b )
  {
    int t;
    t  = *a;
    *a = *b;
    *b = t;
  }

  void Get_sides( Vector no[], float a[] )
  {
    float dx0, dy0, dx1, dy1, dx2, dy2, dx3, dy3;

    dx0     = no[1].x - no[0].x;
    dy0     = no[1].y - no[0].y;
    dx1     = no[2].x - no[1].x;
    dy1     = no[2].y - no[1].y;
    dx2     = no[3].x - no[2].x;
    dy2     = no[3].y - no[2].y;
    dx3     = no[0].x - no[3].x;
    dy3     = no[0].y - no[3].y;

    a[0]    = sqrt((double)(dx0*dx0+dy0*dy0));
    a[1]    = sqrt((double)(dx1*dx1+dy1*dy1));
    a[2]    = sqrt((double)(dx2*dx2+dy2*dy2));
    a[3]    = sqrt((double)(dx3*dx3+dy3*dy3));
  }

  void Get_angles( Vector no[], float alpha[], float theta[] )
  {
    float dx0, dy0, dx1, dy1, dx2, dy2, dx3, dy3, b0, b1, b2, b3;

    dx0     = no[1].x - no[0].x;
    dy0     = no[1].y - no[0].y;
    dx1     = no[2].x - no[1].x;
    dy1     = no[2].y - no[1].y;
    dx2     = no[3].x - no[2].x;
    dy2     = no[3].y - no[2].y;
    dx3     = no[0].x - no[3].x;
    dy3     = no[0].y - no[3].y;

    theta[0] = b0 = atan2((double) dy0, (double) dx0 );
    theta[1] = b1 = atan2((double) dy1, (double) dx1 );
    theta[2] = b2 = atan2((double) dy2, (double) dx2 );
    theta[3] = b3 = atan2((double) dy3, (double) dx3 );

    alpha[0]= Zerototwopi( M_PI - b0 + b3 );
    alpha[1]= Zerototwopi( M_PI - b1 + b0 );
    alpha[2]= Zerototwopi( M_PI - b2 + b1 );
    alpha[3]= Zerototwopi( M_PI - b3 + b2 );
  }

  int Squash_quadrangle( Vector *no0, Vector *no1, Vector *no2, Vector *no3, Vector *new_no0, Vector *new_no1, Vector *new_no2, Vector *new_no3, float theta0, float incr )
  {
    int ok;
    float a[4];
    Vector no[4], nn[4];

    no[0] = *new_no0 = *no0;
    no[1] =            *no1;
    no[2] = *new_no2 = *no2;
    no[3] = *new_no3 = *no3;

    Get_sides( no, a );

    new_no1->x = new_no0->x + a[0] * cos((double)(theta0-incr));
    new_no1->y = new_no0->y + a[0] * sin((double)(theta0-incr));

    ok = New_apex( new_no1, new_no2, new_no3, a[1], a[2] );

    nn[0] = *new_no0;
    nn[1] = *new_no1;
    nn[2] = *new_no2;
    nn[3] = *new_no3;

    return( ok );
  }

  int New_apex( Vector *no1, Vector *no2, Vector *no3, float b, float c )
  {
    float dx, dy, e, aleph, bet, gimel, xp, yp, dis_one, dis_two;
    Vector no2_one, no2_two;

    dx    = no3->x - no1->x;
    dy    = no3->y - no1->y;

    e     = sqrt((double)(dx*dx+dy*dy));

    /*                                                    */
    /*   x'      dx/e   dy/e     x - no1->x               */
    /* (   ) = (             ) (            )             */
    /*   y'     -dy/e   dx/e     y - no1->y               */
    /*                                                    */

    /*                                                    */
    /*   x' =          aleph + bet                        */
    /*   y' = +- sqrt( gimel - bet**2 - aleph**2 )        */
    /*                                                    */
    /*   aleph = (b**2-c**2)/(2*e)                        */
    /*   bet   = e/2                                      */
    /*   gimel = (b**2+c**2)/2                            */
    /*                                                    */

    /*                                                    */
    /*   x       no1->x       dx/e  -dy/e     x'          */
    /* (   ) = (        ) + (             ) (   )         */
    /*   y       no1->y       dy/e   dx/e     y'          */
    /*                                                    */

    aleph = (b*b-c*c)/(2.*e);
    bet   = e/2.;
    gimel = (b*b+c*c)/2.;

    if( gimel >= bet*bet + aleph*aleph )
      {
        xp = aleph + bet;
        yp = sqrt((double)( gimel - bet*bet - aleph*aleph ));

        no2_one.x = no1->x + xp*dx/e - yp*dy/e;
        no2_one.y = no1->y + xp*dy/e + yp*dx/e;

        no2_two.x = no1->x + xp*dx/e + yp*dy/e;
        no2_two.y = no1->y + xp*dy/e - yp*dx/e;

        dis_one = (no2_one.x - no2->x)*(no2_one.x - no2->x )
          + (no2_one.y - no2->y)*(no2_one.y - no2->y );

        dis_two = (no2_two.x - no2->x)*(no2_two.x - no2->x )
          + (no2_two.y - no2->y)*(no2_two.y - no2->y );

        *no2 = ( dis_one < dis_two ) ? no2_one : no2_two;

        return( 1 );
      }
    return( 0);
  }

  int Monte_Carlo( float old_alpha[], float new_alpha[], float lo_alpha[], float hi_alpha[] )
  {
    int n, zero_probability_flag;
    float energy_difference, zero_point, old, anew, probability;

    zero_probability_flag = 0;

    energy_difference = 0.;
    for( n=0; n<4; n++ )
      {
        zero_point = 0.5 * ( hi_alpha[n] + lo_alpha[n] );

        old = fabs( old_alpha[n] - zero_point );
        anew = fabs( new_alpha[n] - zero_point );

        if( anew > HIDELTA )
          {
            energy_difference = 999.;
            zero_probability_flag = 1;
            increment = 0.8 * increment;
            break;
          }
        else
          {
            energy_difference += anew*anew - old*old;
          }
      }

    if( zero_probability_flag )
      {
        probability = 0.;
      }
    else
      if( energy_difference < 0. )
        {
          probability = 1.;
        }
      else
        {
          probability = exp((double)(-energy_difference/TEMPERATURE));
        }

    return( drand48() <= probability );
  }

  void New_nodes(Snake* s, int ni, int nj,
                 Vector new_no_i, Vector new_no_j)
  {
    int n;
    float dx, dy, dxp, dyp, l_2;
    float a11, a12, a21, a22, b1, b2, c1, c2;

    /*                                              */
    /*   x'      c1        a11   a12     x - b1     */
    /* (   ) = (    ) + (             ) (      )    */
    /*   y'      c2        a21   a12     y - b2     */
    /*                                              */

    b1  = s->foreg[ni].xpos;
    b2  = s->foreg[ni].ypos;

    dx  = s->foreg[nj].xpos - b1;
    dy  = s->foreg[nj].ypos - b2;

    c1  = new_no_i.x;
    c2  = new_no_i.y;

    dxp = new_no_j.x - c1;
    dyp = new_no_j.y - c2;

    l_2 = dx*dx + dy*dy;

    a11 = ( dxp*dx + dyp*dy )/l_2;
    a12 = ( dxp*dy - dyp*dx )/l_2;

    a21 = ( dyp*dx - dxp*dy )/l_2;
    a22 = ( dyp*dy + dxp*dx )/l_2;

    for( n=(ni+1)%s->length; n!=nj; n=(n+1)%s->length )
      {
        dx = s->foreg[n].xpos - b1;
        dy = s->foreg[n].ypos - b2;

        s->foreg[n].xpos = c1 + a11*dx + a12*dy;
        s->foreg[n].ypos = c2 + a21*dx + a22*dy;
      }
  }
}

Snake::Snake(int l, float spacing) :
  length(l)
{
  const float radius = (float)(length * spacing) / TWOPI;

  const float alpha_off = TWOPI * drand48();

  for(int n = 0; n < length; ++n)
    {
      const float alpha = TWOPI * n / length;

      foreg[n].xpos   =  radius * cos((double) alpha+alpha_off );
      foreg[n].ypos   = -radius * sin((double) alpha+alpha_off );
    }

  New_delta_theta(this);

  for(int count = 0; count < FOREGROUND_ITERATION; ++count)
    {
      Jiggle_snake(this);
    }

  Center_snake(this);
}

int Snake::getElement(int n, float* x, float* y, float* theta) const
{
  if( n < length )
    {
      *x = 0.5 * ( foreg[n].xpos + foreg[(n+1)%length].xpos );
      *y = 0.5 * ( foreg[n].ypos + foreg[(n+1)%length].ypos );
      *theta = Zerototwopi( -foreg[n].theta );
      return 1;
    }

  return 0;
}
