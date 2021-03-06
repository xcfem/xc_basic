//----------------------------------------------------------------------------
//  xc_basic library; general purpose classes and functions.
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
//Rama.cc
#include "Rama.h"
#include "Operando.h"
#include "OpUnario.h"
#include "ExprFunctor.h"
#include "Lexico.h"
#include <cmath>
#include <iostream>
#include <cassert>

const double &Rama::no_numero= NAN;

//! @brief Constructor.
Rama::Rama(const Operando *s,Rama *i,Rama *j)
  : data(s),izdo(i), dcho(j) 
  {
    Lex().NuevaRefLiteral(data);
  }

//! @brief Constructor.
Rama::Rama(const double &d)
  : data(Lex().NuevoLiteral(d)), izdo(NULL), dcho(NULL) {}

//! @brief Constructor de copia.
Rama::Rama(const Rama &otra)
  : data(otra.data)
  {
    if(otra.izdo) izdo= otra.izdo->getCopy();
    if(otra.dcho) dcho= otra.dcho->getCopy();
    Lex().NuevaRefLiteral(data);
  }

//! @brief Operador asignación.
Rama &Rama::operator=(const Rama &otra)
  {
    Borra();
    data= otra.data;
    if(otra.izdo) izdo= otra.izdo->getCopy();
    if(otra.dcho) dcho= otra.dcho->getCopy();
    Lex().NuevaRefLiteral(data);
    return *this;
  }

//! @brief Destructor.
Rama::~Rama(void)
  {
    Lex().BorraSiLiteral(data);
    Borra();
  }

Clase Rama::GetClase(void) const
  { return data->GetClase(); }
bool Rama::EsToNum(void) const
  {
    bool retval= false;
    if(GetClase()==op_unitario)
      if(data->getName()==OpToNum().getName())
        retval= true;
    return retval;
  }
const std::string &Rama::StrClase(void) const
  { return data->StrClase(); }
int Rama::GetPrioridad(void) const
  { return data->GetPrioridad(); }

Rama *ContraeIzdo(Rama *raiz)
  {
    raiz->BorraIzdo(); //Borra toda la rama izquierda.
    Rama *retval= raiz->dcho;
    raiz->dcho= NULL;
    delete raiz;
    raiz= NULL;
    return retval;
  }
Rama *ContraeDcho(Rama *raiz)
  {
    raiz->BorraDcho(); //hay que borrar toda la rama.
    Rama *retval= raiz->izdo;
    raiz->izdo= NULL;
    delete raiz;
    raiz= NULL;
    return retval;
  }

//! @brief Ejecuta el operador de éste objeto.
const Operando &Rama::Opera(void)
  {
    if(izdo) //Es operador binario.
      return data->Opera(izdo->data,dcho->data);
    else
      if(dcho) //Es operador unario.
        return data->Opera(NULL,dcho->data);
      else //Es valor.
        return data->Opera(NULL,NULL);
  }
//! @brief Ejecuta todos los operadores desde raíz.
void Rama::Opera(Rama *raiz)
  {
    if(raiz->Evaluable())
      {
        raiz->data= Lex().NuevoLiteral(raiz->Opera().GetValor());
        Lex().BorraSiLiteral(raiz->data); //Para no incrementar otra vez el contador de referencias a literal.
        raiz->BorraHijos();
      }
  }
//! @brief Devuelve verdadero si la rama es evaluable.
bool Rama::Evaluable(void) const
  {
    if(data->Evaluable()) return true; //Es evaluable directamente.
    if(dcho)
      {
        if(izdo) //Existe la rama izquierda y por tanto debe existir la derecha (op. binario).
          if(izdo->Evaluable())
            return dcho->Evaluable();
          else //El izquierdo existe y no es evaluable.
            return false;
        else //No existe la rama izquierda (op unario).
          return dcho->Evaluable();
      }
    else
      return false;
  }

Rama diferencia(const Rama &raiz,const Variable &v)
  {
    if(raiz.GetData())
      return raiz.GetData()->Diferencia(v,raiz);
    else
      {
        std::cerr << "Se intentó diferenciar un operando nulo." << std::endl;
        return Rama();
      }
  }
Rama *simplifica(Rama *raiz)
  {
    if(raiz)
      {
        raiz->PutDcho(simplifica(raiz->GetDcho()));
        raiz->PutIzdo(simplifica(raiz->GetIzdo()));
        if(raiz->GetData())
          return raiz= raiz->GetData()->Simplifica(raiz);
        else
          std::cerr << "Se intentó simplificar un operando nulo." << std::endl;
      }
    return raiz;
  }

//! @breif Aplica la propiedad distributiva a través del arbol.
Rama *distrib(Rama *raiz)
  {
    if(raiz)
      {
        Rama *ri= raiz->GetIzdo();
        if(ri)
          {
            Rama *rid= distrib(ri);
            raiz->izdo= rid;
          }
        Rama *rd= raiz->GetDcho();
        if(rd)
          {
            Rama *rdd= distrib(rd);
            raiz->dcho= rdd;
          }
        const Operando *data= raiz->GetData();
        if(data)
          raiz= data->Distrib(raiz);
        else
          std::cerr << "Se intentó distribuir un operando nulo." << std::endl;
      }
    return raiz;
  }

void RecorrePreorden(Rama *raiz,const ExprFunctor &f)
  {
    if(raiz)
      {
        f(raiz);
        RecorrePreorden(raiz->GetIzdo(),f);
        RecorrePreorden(raiz->GetDcho(),f);
      }
  }
void RecorreEnorden(Rama *raiz,const ExprFunctor &f)
  {
    if(raiz)
      {
        RecorreEnorden(raiz->GetIzdo(),f);
        f(raiz);
        RecorreEnorden(raiz->GetDcho(),f);
      }
  }
void RecorrePostorden(Rama *raiz,const ExprFunctor &f)
  {
    if(raiz)
      {
        RecorrePostorden(raiz->GetIzdo(),f);
        RecorrePostorden(raiz->GetDcho(),f);
        f(raiz);
      }
  }

void Escribe(const Rama *raiz,const int &prior, std::ostream &stream)
  {
    if(raiz)
      {
        //int parent= ( (raiz->GetPrioridad() <= prior) && (raiz->GetClase()!=operando));
        bool parent= true;
        if(parent) stream << '(';
        Escribe(raiz->GetIzdo(),raiz->GetPrioridad(),stream);
        stream << *raiz;
        Escribe(raiz->GetDcho(),raiz->GetPrioridad(),stream);
        if(parent) stream << ')';
      }
  }

//! @brief Devuelve una cadena de caracteres que representa a la expresión
//! representando los literales con la máxima precisión posible.
const std::string &Rama::GetFullString(void) const
  {
    static std::string retval= "";

    std::string str_izdo;
    int prioridad_izdo= 100;
    if(izdo)
      {
        str_izdo= retval= izdo->GetFullString();
        prioridad_izdo= izdo->GetPrioridad();
      }

    std::string str_data;
    int prioridad_data= 100;
    if(data)
      {
        str_data= data->GetFullString();
        prioridad_data= data->GetPrioridad();
      }

    std::string str_dcho;
    int prioridad_dcho= 100;
    if(dcho)
      {
        str_dcho= retval= dcho->GetFullString();
        prioridad_dcho= dcho->GetPrioridad();
      }

    if(data)
      {
        const Posicion pos= data->GetPosicion();
        switch(pos)
          {
          case prefija:
            retval= str_data;
            if(data->GetClase()==op_unitario)
              retval+= "(" + str_dcho + ")";
            else
              if(data->GetClase()==op_binario) //Operador binario prefijo (max,min,...)
                retval+= "(" + str_izdo + "," + str_dcho + ")";
              else
                retval+= str_izdo + str_dcho;
            break;
          case infija:
            if(prioridad_dcho<prioridad_data)
              str_dcho= "(" + str_dcho + ")";
            if(prioridad_izdo<prioridad_data)
              str_izdo= "(" + str_izdo + ")";
            retval= str_izdo + str_data + str_dcho;
            break;
          case sufija:
            retval= str_izdo + str_dcho + str_data;
            break;
          }
      }
    else
      retval= str_izdo+str_dcho;
    return retval;
  }

bool operator==(const Rama &r1,const Rama &r2)
  {
    bool eq_izdo= false;
    bool eq_dcho= false;
    if(r1.data == r2.data)
      {
        if(r1.izdo)
          if(r2.izdo)
            eq_izdo= ((*r1.izdo) == (*r2.izdo));
          else
            return 0;
        else
          if(r2.izdo) return 0;
        if(r1.dcho)
          if(r2.dcho)
            eq_izdo= ((*r1.dcho) == (*r2.dcho));
          else
            return 0;
        else
          if(r2.dcho) return 0;
      }
    return (eq_izdo && eq_dcho);
  }

bool operator==(const Rama &r1,const double &d)
  {
    if(r1.data)
      return (r1.data->IgualA(d));
    else
      return false;
  }

void Rama::GetVariables(ConjVariables &cv) const
  { data->GetVariables(cv); }

std::ostream &operator <<(std::ostream &stream,const Rama &e)
  {
    if(e.data) stream << (*e.data);
    return stream;
  }
