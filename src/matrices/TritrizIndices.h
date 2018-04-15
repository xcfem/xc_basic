//----------------------------------------------------------------------------
//  biblioteca bibXCLcmd; bilioteca de comandos para el intérprete del lenguaje
//  de entrada de datos.
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
//TritrizIndices.h
//Vector de matrices de puntos

#ifndef TRITRIZINDICES_H
#define TRITRIZINDICES_H

#include "MatrizIndices.h"

//! @ingroup GEOM
//
//! @brief Clase base para las «tritrices» de posiciones.
class TritrizIndices: public std::vector<MatrizIndices >
  {
  public:
    typedef MatrizIndices m_indices;
    typedef MatrizIndices::reference reference;
    typedef MatrizIndices::const_reference const_reference;

  protected:
    inline MatrizIndices &get_capa(const size_t &capa)
      { return (*this)[capa-1]; }
    inline const MatrizIndices &get_capa(const size_t &capa) const
      { return (*this)[capa-1]; }
  public:
    TritrizIndices(const size_t &capas= 1,const size_t &n_rows= 1,const size_t &n_cols= 1);
    inline size_t GetCapas(void) const
      { return this->size(); }
    size_t getNumberOfRows(void) const;
    size_t getNumberOfColumns(void) const;
    size_t NumIndices(void) const;

    inline const MatrizIndices &operator()(const size_t &capa) const
      { return get_capa(capa); }
    inline MatrizIndices &operator()(const size_t &capa)
      { return get_capa(capa); }
    inline const VIndices &operator()(const size_t &i,const size_t &j,const size_t &k) const
      { return get_capa(i)(j,k); }
    inline VIndices &operator()(const size_t &i,const size_t &j,const size_t &k)
      { return get_capa(i)(j,k); }

    void Offset(const VIndices &);
  };



#endif
