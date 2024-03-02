#ifndef SPLINE_H
#define SPLINE_H



#include "D:\Qt Projects\Applied Geometry\gmlib\modules\parametrics/gmpcurve.h"



using namespace GMlib;


template <typename T>

class spline : public PCurve<T,3> {

    GM_SCENEOBJECT(spline)

public:
    spline( const DVector<Vector<float,3>>& c);
    spline(const DVector<Vector<float,3>>& p, int n );
    spline( const spline<T>& copy );
    virtual ~spline();

    //****************************************
    //****** Virtual public functions   ******
    //****************************************

    // from PCurve
    bool                isClosed() const override;


protected:
    // Virtual functions from PCurve, which have to be implemented locally

    void                eval(T t, int d, bool l) const override;

    T                   getStartP() const override;

    T                   getEndP()   const override;

    T                   findI(T t) const;

    void                create_knot_vector(int n);

    Vector<T,4>         makeB(T t, int i) const;

    T                   findW(int d, int i, T t) const;

    // Protected data for the curve

    DVector<T>      _t; //knot vector

    DVector<Vector<T,3>>     _c; //control curve

    int _d; // degree

    int _k ; //order

}; // END class PLine



// Include PLine class function implementations

template <typename T>
inline
    spline<T>::spline( const DVector<Vector<float,3>>& c): PCurve<T,3>(20, 0, 0) {
    _c = c;
    _d = 3;
    _k = 4;
    create_knot_vector(_c.getDim());

    T interval = _t[_c.getDim()] - _t[3];
    for (int i = 0; i< 20; ++i){
        T r = _t[3] + i*interval/19;
        std::cout<<" ";
        std::cout<< findI(r)<<std::endl;
    }
    std::cout<< _t << std::endl;
      };



/*! PLine<T>::PLine( const Point<T,3>& p, const Vector<T,3>& v  )
   *  Default constructor, to create a line starting in p1 and ending in p2.
   *
   *  \param[in] p1      The start point
   *  \param[in] p2      The end point
   */
template <typename T>
inline
    spline<T>::spline( const DVector<Vector<float,3>>& p, int n ) : PCurve<T,3>(20, 0, 0)  {}



/*! PLine<T>::PLine(const PLine<T>& copy )
   *  A copy constructor
   *  Making a copy of the curve (line)
   *
   *  \param[in] copy The curve to copy
   */
template <typename T>
inline
    spline<T>::spline( const spline<T>& copy ) : PCurve<T,3>(copy) {}



/*! PLine<T>::~PLine()
   *  The destructor
   *  clean up and destroy all private data
   */
template <typename T>
spline<T>::~spline() {}




//***************************************************
// Overrided (public) virtual functons from PCurve **
//***************************************************

/*! bool spline<T>::isClosed() const
   *  To tell that this curve (line) is always open.
   *
   */
template <typename T>
bool spline<T>::isClosed() const {
    return false;
}



//******************************************************
// Overrided (protected) virtual functons from PCurve **
//******************************************************

/*! void PCircle<T>::eval( T t, int d, bool l ) const
   *  Evaluation of the curve at a given parameter value
   *  To compute position and d derivatives at parameter value t on the curve.
   *  7 derivatives are implemented
   *
   *  \param  t[in]  The parameter value to evaluate at
   *  \param  d[in]  The number of derivatives to compute
   *  \param  l[in]  (dummy) because left and right are always equal
   */
template <typename T>
void spline<T>::eval( T t, int d, bool /*l*/ ) const {


    this->_p.setDim( d + 1 );
    int i = findI(t);
    Vector<T,4> b= makeB(t, i);

    this->_p[0] = b[0] * _c[i-3] + b[1] * _c[i-2] + b[2] * _c[i-1] + b[3] * _c[i];


   /* if( this->_dm == GM_DERIVATION_EXPLICIT ) {
        if( d )     this->_p[1] = _v;
        if( d > 1 ) this->_p[2] = Vector<T,3>(T(0));
        if( d > 2 ) this->_p[3] = Vector<T,3>(T(0));
        if( d > 3 ) this->_p[4] = Vector<T,3>(T(0));
        if( d > 4 ) this->_p[5] = Vector<T,3>(T(0));
        if( d > 5 ) this->_p[6] = Vector<T,3>(T(0));
        if( d > 6 ) this->_p[7] = Vector<T,3>(T(0));
    }*/
}



/*! T PLine<T>::getStartP() const
   *  Provides the start parameter value associated with
   *  the eval() function implemented above.
   *  (the start parameter value = 0).
   *
   *  \return The parametervalue at start of the internal domain
   */
template <typename T>
T spline<T>::getStartP() const {
    return _t[_d];
}



/*! T PLine<T>::getEndP() const
   *  Provides the end parameter value associated with
   *  the eval() function implemented above.
   *  (the end parameter value = 1).
   *
   *  \return The parametervalue at end of the internal domain
   */
template <typename T>
T spline<T>::getEndP() const {
    //int end = _t.size()-_k;
    return _t[_c.getDim()];
}

template <typename T>
T spline<T>:: findI(T t)const {
    for (int i=3; i<_c.getDim(); ++i){
        if(t >= _t[i] && t< _t[i+1]){
            return i;
        }
    }

    return _c.getDim()-1;

}

template<typename T>
T spline<T>::findW(int d, int i, T t) const{
    return (t - _t[i])/(_t[i+d] - _t[i]);
}

template <typename T>
Vector<T,4> spline<T>::makeB(T t, int i) const{

    T sub_b1 = (1- findW(1,i,t)) * (1-findW(2,i-1,t));
    T sub_b2 = (1- findW(1,i,t))* (1-findW(2,i-1,t)) + findW(1,i,t)*(1-findW(2,i,t));
    T sub_b3 =  findW(1,i,t)* findW(2,i,t);
    T b1 = (1 - findW(3, i-2, t))* sub_b1;
    T b2 = findW(3, i-2, t)* sub_b1 + (1-findW(3,i-1,t))* sub_b2;
    T b3 = findW(3,i-1,t)* sub_b2 + (1 - findW(3,i,t)) * sub_b3;
    T b4 =  findW(3,i,t) * sub_b3;
    Vector<T,4> B = {b1, b2, b3, b4};
    return B;
}

template <typename T>
void spline<T>::create_knot_vector(int n){
    for(int i = 0; i<4; i++){
        _t.push_back(0);
    }
    for (int i = 4; i<_c.getDim(); i++){
        _t.push_back(i-3);
    }
    for (int i=0; i<4; i++){
        _t.push_back(_t[_c.getDim()-1]+1);
    }
}




#endif // SPLINE_H
