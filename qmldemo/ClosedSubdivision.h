#ifndef CLOSEDSUBDIVISION_H
#define CLOSEDSUBDIVISION_H


#include "D:\Qt Projects\Applied Geometry\gmlib\modules\parametrics/gmpcurve.h"



using namespace GMlib;


template <typename T>

class closed_subdivision : public PCurve<T,3> {

    GM_SCENEOBJECT(closed_subdivision)

public:
    closed_subdivision( const DVector<Vector<float,3>>& c);

    closed_subdivision (const DVector<Vector<float,3>>& p, int n );

    closed_subdivision ( const closed_subdivision<T>& copy );

    void sample(int m, int d = 0);

    virtual ~closed_subdivision();

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


    // Protected data for the curve
    DVector<Vector<T,3>>     _c; //control curve

    void smoothPartClosed( std::vector<DVector<Vector<T, 3>>> &  P, int n, int d);
    int doublePart( std::vector<DVector<Vector<T, 3>>>  & P, int n);
    void LaneRiesenfeldClosed(  GMlib::DVector<GMlib::Vector<T, 3>>& P, int k, int d );



}; // END class closed_subdivision



// Include closed_subdivision class function implementations

template <typename T>
inline
    closed_subdivision<T>::closed_subdivision( const DVector<Vector<float,3>>& c): PCurve<T,3>(20, 0, 0) {

    _c = c;

};



/*! PLine<T>::PLine( const Point<T,3>& p, const Vector<T,3>& v  )
   *  Default constructor, to create a line starting in p1 and ending in p2.
   *
   *  \param[in] p1      The start point
   *  \param[in] p2      The end point
   */
template <typename T>
inline
    closed_subdivision<T>::closed_subdivision( const DVector<Vector<float,3>>& p, int n ) : PCurve<T,3>(20, 0, 0)  {}



/*! PLine<T>::PLine(const PLine<T>& copy )
   *  A copy constructor
   *  Making a copy of the curve (line)
   *
   *  \param[in] copy The curve to copy
   */
template <typename T>
inline
    closed_subdivision<T>::closed_subdivision( const closed_subdivision<T>& copy ) : PCurve<T,3>(copy) {
    _c = copy._c;
}



/*! PLine<T>::~PLine()
   *  The destructor
   *  clean up and destroy all private data
   */
template <typename T>
closed_subdivision<T>::~closed_subdivision() {

}




//***************************************************
// Overrided (public) virtual functons from PCurve **
//***************************************************

/*! bool spline<T>::isClosed() const
   *  To tell that this curve (line) is always open.
   *
   */
template <typename T>
bool closed_subdivision<T>::isClosed() const {
    return true;
}


template <typename T>
void closed_subdivision<T>::sample(int m, int d = 3){

    this-> _visu.resize(1);

    _checkSampleVal( m, d );

    LaneRiesenfeldClosed(_c, m,  d);

    //std::cout<<_visu[0].sample_val<<std::endl;

    Sphere<T, 3>& sp = _visu[0].sur_sphere;
    std::vector<DVector<Vector<T,3>>>& sv = _visu[0].sample_val;
    sp.resetPos(sv[0][0]);
    for(int i = 1; i< sv.size(); i++){
        sp += sv[0][i];
    }

  this->setEditDone();

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
void closed_subdivision<T>::eval( T t, int d, bool /*l*/ ) const {


    this->_p.setDim( d + 1 );
}



/*! T PLine<T>::getStartP() const
   *  Provides the start parameter value associated with
   *  the eval() function implemented above.
   *  (the start parameter value = 0).
   *
   *  \return The parametervalue at start of the internal domain
   */
template <typename T>
T closed_subdivision<T>::getStartP() const {
    return 0;
}



/*! T PLine<T>::getEndP() const
   *  Provides the end parameter value associated with
   *  the eval() function implemented above.
   *  (the end parameter value = 1).
   *
   *  \return The parametervalue at end of the internal domain
   */
template <typename T>
T closed_subdivision<T>::getEndP() const {
    return 1;
}

template <typename T>
int closed_subdivision<T>::doublePart( std::vector<DVector<Vector<T, 3>>> & P, int n) {

    //Insert new vertices at the midpoints (the splitting step)
    for ( int i=n-1; i > 0; i-- ){
        P[2*i][0] = P[i][0];
        P[2*i-1][0] = (P[i][0] + P[i-1][0])/2;
    }
    return 2*n-1;

}

template<typename T>
void closed_subdivision<T>::smoothPartClosed(std::vector<DVector<Vector<T, 3>>> & P, int n, int d){

    for ( int j=1; j < d; j++ ){
        for ( int i=0; i < n-1; i++ ){
            P[i][0] = (P[i][0] + P[i+1][0])/2;
        }
        P[n-1][0] = P[0][0];
    }

}

template <typename T>
void closed_subdivision<T>::LaneRiesenfeldClosed(DVector<Vector<T, 3>>& P, int k, int d ) {


    int n = P.getDim();
    int m = pow(2, k)*n + 1;
    std::vector<DVector<Vector<T, 3>>>& phi = this->_visu[0].sample_val;
    phi.resize(m);
    for ( int i=0; i < m; i++ ){
        phi[i].setDim(1);
    }

    for ( int i=0; i < n; i++ ){
        phi[i][0] = P[i];
    }
    phi[n++][0] = P[0];

    for ( int i=0; i < k; i++ ){
        n = doublePart(phi, n);
        smoothPartClosed(phi, n, d );
    }
    //std::cout<<phi<<std::endl;

}


#endif // CLOSEDSUBDIVISION_H
