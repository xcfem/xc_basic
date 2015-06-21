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
//MatsimZ.h
//Matrices simétricas y hemisimétricas.
//Se almacena el triángulo inferior.
#ifndef MATSIMZ_H
#define MATSIMZ_H

#include "matrizZ.h"
#include <algorithm>

template <class numero>
class matsimZ : public matrizZ<numero>
  {
  public:
    typedef matrizZ<numero> matrizZ_numero;
    typedef typename matrizZ_numero::size_type size_type;
  protected:
    //Multiplica a los elementos del triangulo inferior
    //sirve para matrices hemisimEtricas.
    int signo;
    inline virtual size_type Tam(size_type filas,size_type cols)
      { return (filas*(filas+1)/2); }
    inline virtual size_type Indice( const size_type &fila,
                                     const size_type &col) const
      {
        size_type f= fila, c= col;
        if (f < c) std::swap(f,c);
        return (c-1)*(2*this->fls-c)/2+f-1;
      }
  public:
    matsimZ(void) : matrizZ_numero (1,1) {}
    matsimZ(size_type dim) : matrizZ_numero(1,Tam(dim,dim))
      { PutDim(dim,dim); }
    matsimZ(size_type dim,numero val) : matrizZ_numero(1,Tam(dim,dim),val)
      { PutDim(dim,dim); }
    matsimZ(const matsimZ<numero> &otra) : matrizZ_numero(otra)
      { PutDim(otra.fls,otra.cls); }
    matsimZ<numero>& operator =(const matsimZ<numero> &m)
      {
        matrizZ_numero::operator =(m);
        return *this;
      }
    matsimZ<numero>& operator +=(const matsimZ<numero> &m)
      {
        matrizZ_numero::Suma(m);
        return *this;
      }
    matsimZ<numero>& operator -=(const matsimZ<numero> &m)
      {
        matrizZ_numero::Resta(m);
        return *this;
      }
    inline void Trn(void) {}
    inline bool Cuadrada(void) const
      { return 1; }
    matrizZ_numero GetCompleta(void) const
      {
        matrizZ_numero retval(this->fls,this->cls);
        for(size_t i=1;i<=this->fls;i++)
          for(size_t j=1;j<=this->cls;j++)
            retval(i,j)= (*this)(i,j);
        return retval;
      }
    friend matsimZ<numero> operator *(const matsimZ<numero> &m,const numero &p)
      {
        matsimZ<numero> producto(m);
        producto.Prod(p);
        return producto;
      }
    friend matsimZ<numero> operator *(const numero &p,const matsimZ<numero> &m)
      { return m*p; }
    friend matsimZ<numero> operator +(const matsimZ<numero> &m1,const matsimZ<numero> &m2)
      {
        check_sum(m1,m2);
        matsimZ<numero> suma(m1.fls);
        suma.Suma(m1,m2);
        return suma;
      }
    friend matsimZ<numero> operator -(const matsimZ<numero> &m1,const matsimZ<numero> &m2)
      {
        check_dif(m1,m2);
        matsimZ<numero> resta(m1.fls,m1.cls);
        resta.Resta(m1,m2);
        return resta;
      }
    friend std::ostream &operator << (std::ostream &stream,const matsimZ<numero> &m)
      { return ::operator <<(stream,(const matrizZ_numero &) m); }
    friend std::istream &operator >> (std::istream &stream,matsimZ<numero> &m)
      { return ::operator >>(stream,(matrizZ_numero &) m); }
    friend bool operator ==(const matsimZ<numero> &m1,const matsimZ<numero> &m2)
      { return ((const matrizZ_numero &) m1 == (const matrizZ_numero &) m2); }
  };

#endif


