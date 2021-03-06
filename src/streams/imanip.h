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
//imanip.h

#ifndef IMANIP_H
#define IMANIP_H

#include<iostream>
#include<iomanip>
#include<string>
#include<cctype>
#include<cstdlib>

template <class TP>
class imanip
  {
  public:
    std::istream& (*_f)(std::istream&, TP);
    TP _a;
    imanip(std::istream& (*f)(std::istream&, TP), TP a) : _f(f), _a(a) {}
    //friend std::istream& operator>> <>(std::istream& i, const imanip<TP>& m);
  };

template <class TP>
inline std::istream& operator>>(std::istream& i, const imanip<TP>& m)
  { return (*m._f)( i, m._a); }

void vuelca_stream(std::istream &is,std::ostream &os);

std::istream &ignorar_hasta(std::istream &is,char delim);


int busquedabruta(std::istream &a,const std::string &p);
//Coloca el puntero de lectura del stream despues de la
//primera ocurrencia de la cadena p.
//Esta rutina se puede mejorar (Ver Algoritmos en C++).
//Devuelve verdadero si encontro la cadena.

inline int busca_inicio(std::istream &is,const std::string &str)
//Devuelve la posici'on del primer car'acter de la cadena str
//en el stream is.
  {
    int encontrada= 0;
    if((encontrada = busquedabruta(is,str)))
      is.seekg(-(str.length()+1),std::ios::cur);
    return encontrada;
  }

inline std::streampos busca_fin(std::istream &is,const std::string &str)
//Devuelve la posici'on del primer car'acter de la cadena str
//en el stream is.
  { return busquedabruta(is,str); }

inline std::istream &ignorar_hasta(std::istream &is,const std::string &p)
  {
    busquedabruta(is,p);
    return is;
  }

inline imanip<char> ignore_to(char delim)
  { return imanip<char>(&ignorar_hasta,delim); }

inline imanip<const std::string &> ignore_to(const std::string &delim)
  { return imanip<const std::string &>(&ignorar_hasta,delim); }

std::istream &busca_alnum(std::istream &is);

std::istream &busca_notspace(std::istream &is);

template <class T>
struct token_pair
  {    
    T &t;
    const char delim;
    token_pair(T &s,const char d): t(s),delim(d){}
  };

inline std::istream &get_token(std::istream &is,token_pair<std::string> pair)
  {
    const size_t sz= 256;
    char c[sz];
    is.getline(c,sz,pair.delim);
    pair.t= c;
    return is;
  }

inline std::istream &get_token(std::istream &is,token_pair<int> pair)
  {
    std::string str;
    token_pair<std::string> pair_str(str,pair.delim);
    get_token(is,pair_str);
    pair.t= atoi(pair_str.t.data());
    return is;
  }

inline std::istream &get_token(std::istream &is,token_pair<bool> pair)
  {
    int i;
    token_pair<int> pair_i(i,pair.delim);
    get_token(is,pair_i);
    pair.t= (pair_i.t == 1);
    return is;
  }

inline std::istream &get_token(std::istream &is,token_pair<float> pair)
  {
    std::string str;
    token_pair<std::string> pair_str(str,pair.delim);
    get_token(is,pair_str);
    pair.t= atof(pair_str.t.data());
    return is;
  }

inline std::istream &get_token(std::istream &is,token_pair<double> pair)
  {
    std::string str;
    token_pair<std::string> pair_str(str,pair.delim);
    get_token(is,pair_str);
    pair.t= atof(pair_str.t.data());
    return is;
  }

template<class T> 
inline imanip<token_pair<T> > get_token(T &str,const char delim= ',')
  {
    token_pair<T> pair(str,delim);
    return imanip<token_pair<T> >(&get_token,pair);
  }

template <class T>
struct token_pair2
  {    
    T &t;
    const std::string delim;
    token_pair2(T &s,const std::string d): t(s),delim(d){}
  };

std::istream &get_tok(std::istream &is,token_pair2<std::string> pair);

template<class T>
inline imanip<token_pair2<T> > get_tok(T &str,const std::string delim= ",")
  {
    token_pair2<T> pair(str,delim);
    return imanip<token_pair2<T> >(&get_tok,pair);
  }

#endif

