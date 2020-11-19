#ifndef BASE
#define BASE

#include "eigentypes.h"
#include <boost/numeric/odeint.hpp>
#include <boost/numeric/odeint/external/eigen/eigen_algebra.hpp>

using namespace Eigen;

template<int n, int m> class base {
protected:
  int nloops;

public:
  // members
  gauge<n> g;                                         // vector of gauge couplings
  self<m> La;                                         // Higgs selfcoupling(s)
  yukawa Yu,Yd,Ye;                                 // yukawa matrices
  
  // constructors
 base() : g(), La(), Yu(), Yd(), Ye(), nloops(2) {};
 base(const gauge<n> g_in, const self<m> La_in, const yukawa Yu_in, const yukawa Yd_in, const yukawa Ye_in, const int nloops_in) : g(g_in), La(La_in), Yu(Yu_in), Yd(Yd_in), Ye(Ye_in), nloops(nloops_in) {};
 base(const gauge<n> g_in, const self<m> La_in, const yukawa Yu_in, const yukawa Yd_in, const yukawa Ye_in) : g(g_in), La(La_in), Yu(Yu_in), Yd(Yd_in), Ye(Ye_in), nloops(2) {};
  // constructor that sets all selfcouplings to zero
 base(const gauge<n> g_in, const yukawa Yu_in, const yukawa Yd_in, const yukawa Ye_in, const int nloops_in) : g(g_in), La(), Yu(Yu_in), Yd(Yd_in), Ye(Ye_in), nloops(nloops_in) {};
 base(const gauge<n> g_in, const yukawa Yu_in, const yukawa Yd_in, const yukawa Ye_in) : g(g_in), La(), Yu(Yu_in), Yd(Yd_in), Ye(Ye_in), nloops(2) {};
 
  // in place operations for vector space algebra
  base operator+=( const base<n,m> &X );
  base operator*=( const double a );
  
  // set ALL parameters to zero:
  void setZero();

  // set the number of loops (default: 2)
  void setNloops(const int nloops_in);

  // get the number of loops
  unsigned int getNloops() const;
  
  // check for Landau poles and Nan
  bool check();
  
  // macro for eigen member functions
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    
    };

// in place operations

template<int n, int m> base<n,m> base<n,m>::operator+=(const base<n,m> & X){
  g += X.g;  La += X.La;  Yu += X.Yu;  Yd += X.Yd;  Ye += X.Ye;
  return *this;
}
  
template<int n, int m> base<n,m> base<n,m>::operator*=(const double a){
  g *= a;  La *= a;  Yu *= a;   Yd *= a;   Ye *= a; 
  return *this;
}

// set all parameters to zero
template<int n, int m> void base<n,m>::setZero(){
  g.setZero();  La.setZero();  Yu.setZero();  Yd.setZero();  Ye.setZero();
}

// set number of loops
template<int n, int m> void base<n,m>::setNloops(const int nloops_in){
  nloops = nloops_in;
}

//return number of loops
template<int n, int m> unsigned int base<n,m>::getNloops() const {
  return nloops;
}


// check for Landau poles and Nan
template<int n, int m> bool base<n,m>::check(){
  return isnotnan(g) && ((&g)->template lpNorm<Infinity>() < 3.5)
    && isnotnan(La) && ((&La)->template lpNorm<Infinity>() < 3.5)
    && isnotnan(Yu) && (Yu.lpNorm<Infinity>() < 3.5)
    && isnotnan(Yd) && (Yd.lpNorm<Infinity>() < 3.5)
    && isnotnan(Ye) && (Ye.lpNorm<Infinity>() < 3.5)
    && (nloops > 0);
}


// operations for vector space algebra

template<int n, int m> base<n,m> operator+( const base<n,m> &lhs , const base<n,m> &rhs ){
  return base<n,m>(lhs.g+rhs.g, lhs.La+rhs.La, lhs.Yu+rhs.Yu, lhs.Yd+rhs.Yd, lhs.Ye+rhs.Ye, lhs.getNloops());
}

template<int n, int m> base<n,m> operator*( const base<n,m> &lhs , const double &a ){
  return base<n,m>(lhs.g*a, lhs.La*a, lhs.Yu*a, lhs.Yd*a, lhs.Ye*a, lhs.getNloops());
}

template<int n, int m> base<n,m> operator*( const double &a , const base<n,m> &rhs ){
  return base<n,m>(a*rhs.g, a*rhs.La, a*rhs.Yu, a*rhs.Yd, a*rhs.Ye, rhs.getNloops());
}

template<int n, int m> base<n,m> operator+( const base<n,m> &lhs , const double &a ){
  return base<n,m>( lhs.g+a, lhs.La+a, lhs.Yu+a, lhs.Yd+a, lhs.Ye+a, lhs.getNloops());
}

template<int n, int m> base<n,m> operator+( const double &a , const base<n,m> &rhs ){
  return base<n,m>(a+rhs.g, a+rhs.La, a+rhs.Yu, a+rhs.Yd, a+rhs.Ye, rhs.getNloops());
}

template<int n, int m> base<n,m> operator/( const base<n,m> &lhs , const base<n,m> &rhs ){
  return base<n,m>( lhs.g.cwiseQuotient(rhs.g),
	       lhs.La.cwiseQuotient(rhs.La),
	       lhs.Yu.cwiseQuotient(rhs.Yu),
	       lhs.Yd.cwiseQuotient(rhs.Yd),
	       lhs.Ye.cwiseQuotient(rhs.Ye),
               lhs.getNloops() );
}

template<int n, int m> base<n,m> abs( const base<n,m> &x ){
  return base<n,m>(abs(x.g), abs(x.La), abs(x.Yu), abs(x.Yd), abs(x.Ye), x.getNloops() );
}

// specialisation of vector_space_algebra for Eigen::Matrix
namespace boost { namespace numeric { namespace odeint {
template<typename B,int S1,int S2,int O, int M1, int M2>
	struct algebra_dispatcher< Eigen::Matrix<B,S1,S2,O,M1,M2> >{
	    typedef vector_space_algebra algebra_type;
	};
}}}


#endif
