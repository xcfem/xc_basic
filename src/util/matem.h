//----------------------------------------------------------------------------
//  biblioteca xc_basic; funciones y clases de uso general
//
//  Copyright (C)  Luis Claudio Pérez Tato
//
//  Este software es libre: usted puede redistribuirlo y/o modificarlo 
//  bajo los términos de la Licencia Pública General GNU publicada 
//  por la Fundación para el Software Libre, ya sea la versión 3 
//  de la Licencia, o (a su elección) cualquier versión posterior.
//
//  Este software se distribuye con la esperanza de que sea útil, pero 
//  SIN GARANTÍA ALGUNA; ni siquiera la garantía implícita 
//  MERCANTIL o de APTITUD PARA UN PROPÓSITO DETERMINADO. 
//  Consulte los detalles de la Licencia Pública General GNU para obtener 
//  una información más detallada. 
//
// Debería haber recibido una copia de la Licencia Pública General GNU 
// junto a este programa. 
// En caso contrario, consulte <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------------
//Matem.h

#ifndef MATEM_H
#define MATEM_H

#include <cmath>
#include <climits>
#include <cfloat>
#include <algorithm>
#include <boost/math/special_functions/round.hpp>

//Elementos neutros para la suma y el producto.
inline double neutro_suma(const double &)
  { return 0.0; }
inline double neutro_producto(const double &)
  { return 1.0; }

template <class tint>
inline int es_multiplo(const tint &v,const tint &i)
  { return ((v%i) == 0); }

template <class tint>
inline tint num_multiplos(const tint &n,const tint &org,const tint &fin)
//Devuelve el numero de multiplos de n
//desde org hasta fin ambos inclusive.
  {
    if (fin< org)
      return 0;
    else
      {
        tint nm= (fin/n) - (org/n);
        if(es_multiplo(org,n)) nm++;
        return nm;
      }
  }

template <class Z>
inline Z Abs(const Z &x) { return x >= 0 ? x : -x; }

//secante
inline double sec(const double &x)
  { return 1.0/cos(x); }
//cosecante
inline double cosec(const double &x)
  { return 1.0/sin(x); }


//Signo
inline short int sgn(const double &value)
  { return (value<0 ? -1 : 1); }
inline short int sgn_prod(const double &num,const double &denom)
  { return sgn(num) * sgn(denom); }

//Division por cero  
inline double divide(const double &num,const double &denom)
  {
    register double retval(num);
    if(fabs(denom) > 0 )
      retval/= denom;
    else
      {
        retval= double(sgn_prod(num,denom));
        if(fabs(num)>0) retval*=DBL_MAX;
      }
    return retval;
  }

//Cuadrado
template <class Z>
inline Z sqr(const Z &value)
  { return (value * value); }

//Partes entera y fraccionaria.
inline double ip(long double v)
  { return (floor(v)); }
inline int INT(double v)
  { return (int) ip(v); }
inline long int LONG_INT(double v)
  { return (long int) ip(v); }
inline long int U_LONG_INT(double v)
  { return (unsigned long int) LONG_INT(ip(fabs(v))); }
inline double fp(long double v)
  { return (v-floor(v)); }

inline double pow10(const int &i)
  { return pow((double)i,10); }
inline long double pow10(const long double &v)
  { return pow(v,10); }
//definida en mathcalls.h 
//inline long double exp10(const long double &v) 
//{ return pow(10,v); }


//Redondeo
inline long iround(const long double &v)
  { return boost::math::lround(v); }
inline long double round(const long double &v, short unsigned int dec)
     //xxx funciona mal
  {
    const double p= pow10(dec);
    double d= v * p;
    d= round(d);
    d/= p;
    return d;
  }

inline int entre(const double &inf,const double &v,const double &sup)
  {
    if ((inf < v) && (v < sup))
      return 1;
    else
      return 0;
  }
inline int entre_ext(const double &inf,const double &v,const double &sup)
  {
    if (entre(inf,v,sup) || entre(sup,v,inf))
      return 1;
    else
      return 0;
  }

inline bool impar(const long &l)
  { return l%2; }
inline bool par(const long &l)
  { return !impar(l); }


//Logs.
inline double LN(double value)
  { return (log(value)); }
inline double LOG(double value)
  { return (log10(value)); }
inline double EXP(double value)
  { return (exp(value)); }
inline double ALOG(double value)
  { return (pow(10,value)); }




//Raices.
inline double dicot_zero(double f(double), double a, double b,double &err)
  {
    double epsi= err;
    enum {inf=0,sup=1} modif;
    double c,xx;
    double x[2];
    x[0]= a;
    x[1]= b;
    if (f(a) > 0)
      c=1;
    else
      c=-1;
    err= x[sup]-x[inf];
    while ( err > epsi )
      {
	xx = 0.5*(x[inf]+x[sup]);
	if ( (c*f(xx)) > 0)
	  modif= inf;
	else
	  modif= sup;
	x[modif]= xx;
	err= x[sup]-x[inf];
      }
    return xx; 
  }

//Interpolación.
inline double IntRecta(double x0,double y0,double x1,double y1, double x)
  {
    double a= (y1-y0)/(x1-x0);
    return (y0-a*(x0-x)); 
  }


//Angulos.
inline double HMS_C(const double &hms)
  {
    double h= floor(hms);
    double mnt= floor((hms-h)*100.0);
    double sec= (((hms-h)*100.0)-mnt)*100;
    return ((h*60.0+mnt)*60.0+sec)/3600.0;
  }

inline double RadToDeg(const double &ang)
  { return (180.0/M_PI)*ang; }
inline double RadToCent(const double &ang)
  { return (200.0/M_PI)*ang; }
inline double DegToRad(const double &ang)
  { return (M_PI/180.0)*ang; }
inline double CentToRad(const double &ang)
  { return (M_PI/200.0)*ang; }

inline double HMSToRad(const double &hms)
  { return DegToRad(HMS_C(hms)); } 

#endif






