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
//Gauss.h
//Procedimiento de Gauss para la solución de un sistema de ecuaciones lineal.

#ifndef GAUSS_H
#define GAUSS_H

#include <cmath>
#include "xc_basic/src/matrices/matrizZ.h"
#include "xc_basic/src/util/matem.h"
#include "xc_basic/src/matrices/solvers/pivote.h"
#include "xc_basic/src/matrices/solvers/calcularx.h"

template <class treal>
void eliminar( matrizZ<treal> &a,
               matrizZ<treal> &b,
               const size_t &j,
               size_t *pivot,
               const set_szt &fp)
  {
    size_t i,k,l,n= a.getNumFilas(),m=b.getNumCols();
    l=pivot[j-1]; treal r=gj_inv(a(l,j));
    for(k=j+1;k<= n;k++) a(l,k)*=r;
    for(k=1;k<=m;k++) b(l,k)*=r;
    for(i=1;i<=n;i++)
      if (!In(fp,i) && (gj_abs(a(i,j))>zero))
	{
          for(k=j+1;k<=n;k++) a(i,k)-= a(i,j)*a(l,k);
          for(k=1;k<=m;k++) b(i,k)-= a(i,j)*b(l,k);
        }
  }

template <class treal>
matrizZ<treal> gauss(matrizZ<treal> &a,matrizZ<treal> &b,int &regular)
  {
    size_t j;
    //Dimensionamos la matriz de indices de pivote.
    typename matrizZ<treal>::size_type n= a.getNumFilas();
    typename matrizZ<treal>::size_type *pivot= new typename matrizZ<treal>::size_type[n];
    set_szt fp;
    regular=1; j=0;
    while (regular && (j<n))
      {
        j++;
        pivot[j-1]= pivote(j,a,fp);
        regular=(pivot[j-1]>0);
        if(regular)
          {
            fp.insert(pivot[j-1]);
            eliminar(a,b,j,pivot,fp);
          }
      }
    matrizZ<treal> x;
    if(regular) x= calcularx(a,b,pivot);
    delete[] pivot;
    return x;
  }

template <class treal>
matrizZ<treal> gauss_const(const matrizZ<treal> &a,const matrizZ<treal> &b,int &regular)
  {
    matrizZ<treal> c=a,d=b;
    return gauss(c,d,regular);
  }

template <class treal>  
matrizZ<treal> operator /(const matrizZ<treal> &b, const matrizZ<treal> &a)
//Se le pasan copias de los valores de b y a.
  {
    if (b.getNumFilas() != a.getNumFilas())
      {
	std::cerr << "Matrices de dimensiones incompatibles en operador /" << std::endl;
        abort();      
      }
    int regular;
    matrizZ<treal> x= gauss_const(a,b,regular);
    if (!regular) std::cerr << " matriz singular" << std::endl;
    return x;
  }

template <class treal>
matrizZ<treal> operator /(const matrizZ<treal> &m,const treal &d)
  { return m*gj_inv(d); }

//! @brief Devuelve la matriz inversa.
template <class treal>
matrizZ<treal> gauss_invierte(const matrizZ<treal> &a,int &regular)
  {
    matrizZ<treal> b(a.getNumFilas(),a.getNumCols());
    b.Idn();
    matrizZ<treal> x= gauss_const(a,b,regular);
    return x;
  }

#endif

