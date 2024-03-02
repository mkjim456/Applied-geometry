#ifndef TEST2_H
#define TEST2_H

#include "parametrics/gmpcurve.h"
#include "scene/gmsceneobject.h"
namespace GMlib {


template <typename T>
class Test_Curve_2 : public PCurve<T,3> {
    GM_SCENEOBJECT(Test_Curve_2);

public:
    Test_Curve_2(  T k = 3 , T a = 2, T b = 3 );
    Test_Curve_2( const Test_Curve_2<T>& copy );
    virtual ~Test_Curve_2();


    //****************************************
    //****** Virtual public functions   ******
    //****************************************

    // from PCurve
    bool            isClosed() const ;

protected:
    // Virtual functions from PCurve, which have to be implemented locally
    void            eval(T t, int d,  bool l) const override;
    T               getStartP() const override;
    T               getEndP()   const override;

    T a, b, k;


}; // END class Test_Curve_2_2

} // END namepace GMlib

// Include Test_Curve_2 class function implementations

//*****************************************
// Constructors and destructor           **
//*****************************************

/*! Test_Curve_2<T>::Test_Curve_2( T radius )
 *  Default constructor, to make a circle with the radius = radius.
 *
 *  \param[in] radius      (default 20) The radius of the circle
 */
template <typename T>
inline
    GMlib::Test_Curve_2<T>::Test_Curve_2(  T k, T a, T b) : PCurve<T,3>(2, 4, 7) {
    // Note that the last parameter in the PCurve constructor is 7,
    // this because 7 derivatives in eval() is implemented!!!!
    this->k = k;
    this->a = a;
    this->b = b;


}


/*! Test_Curve_2<T>::Test_Curve_2(const Test_Curve_2<T>& copy )
   *  A copy constructor
   *  Making a copy of the curve (circle)
   *
   *  \param[in] copy The curve to copy
   */
template <typename T>
inline
    GMlib::Test_Curve_2<T>::Test_Curve_2( const Test_Curve_2<T>& copy ) : PCurve<T,3>(copy) {}



/*! Test_Curve_2<T>::~Test_Curve_2()
   *  The destructor
   *  clean up and destroy all private data
   */
template <typename T>
GMlib::Test_Curve_2<T>::~Test_Curve_2() {}








//***************************************************
// Overrided (public) virtual functons from PCurve **
//***************************************************

/*! bool Test_Curve_2<T>::isClosed() const
   *  To tell that this curve (circle) is closed.
   */
template <typename T>
bool GMlib::Test_Curve_2<T>::isClosed() const {
    return true;
}



//******************************************************
// Overrided (protected) virtual functons from PCurve **
//******************************************************

/*! void Test_Curve_2<T>::eval( T t, int d, bool l ) const
   *  Evaluation of the curve at a given parameter value
   *  To compute position and d derivatives at parameter value t on the curve.
   *  7 derivatives are implemented
   *
   *  \param  t[in]  The parameter value to evaluate at
   *  \param  d[in]  The number of derivatives to compute
   *  \param  l[in]  (dummy) because left and right are always equal
   */
template <typename T>
void GMlib::Test_Curve_2<T>::eval( T t, int d, bool /*l*/ ) const {

    this->_p.setDim( d + 1 );


    /*
     x = a cos(kx * t)
     y = b sin(ky *t)

    */


    this->_p[0][0] =  a * cos(t) - (a * pow(sin(t), 2))/sqrt(2);
    this->_p[0][1] = a * cos(t) * sin(t);
    this->_p[0][2] = T(0);





}



/*! T Test_Curve_2<T>::getStartP() const
   *  Provides the start parameter value associated with
   *  the eval() function implemented above.
   *  (the start parameter value = 0).
   *
   *  \return The parametervalue at start of the internal domain
   */
template <typename T>
T GMlib::Test_Curve_2<T>::getStartP() const {
    return T(0);
}



/*! T Test_Curve_2<T>::getEndP() const
   *  Provides the end parameter value associated with
   *  the eval() function implemented above.
   *  (the end parameter value = 2*Pi).
   *
   *  \return The parametervalue at end of the internal domain
   */
template <typename T>
T GMlib::Test_Curve_2<T>::getEndP()const {
    return T(    M_2PI );
}


#endif // TEST2_H
