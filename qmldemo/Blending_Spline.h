#ifndef MYBLENDINGSPLINE_H
#define MYBLENDINGSPLINE_H
#include "parametrics/curves/gmpcircle.h"
#include "parametrics/curves/gmpsubcurve.h"
#include "parametrics/gmpcurve.h"
#include "scene/gmsceneobject.h"
namespace GMlib {

//Blending Spline = special B-Spline
// Use sub curve construction 8.3
//Implement function
//Constructor points to model curves, and int local curves
//Open/Closed based on model curve
//Use subcurve

//Blending spline = b spline 1st degree seond order
//We are to make a copy of a different curve wth a different domain
//equivalent to first degree second order
//times with B as usual polynomial, therefore must add b function for w and must multiply with what I get from local curves


template <typename T>
class MyBlending_Spline : public PCurve<T,3> {
    GM_SCENEOBJECT(MyBlending_Spline);

public:

    MyBlending_Spline(  PCurve<T,3>* p, int n);
    MyBlending_Spline( const MyBlending_Spline<T>& copy );

    virtual ~MyBlending_Spline();


    //****************************************
    //****** Virtual public functions   ******
    //****************************************

    // from PCurve
    bool            isClosed() const override ;
    void            sample(int m, int d = 0) override;

protected:
    // Virtual functions from PCurve, which have to be implemented locally
    void            eval(T t, int d,  bool l) const override;
    T               getStartP() const override;
    T               getEndP()   const override;

    // void            localcurves(int n, bool closed);
    /*
     * For B_Spline we need Degree, Coordinates, Order
     */
    int                   _d;
    int                   _k;

    /*
     * Big T = typename, decides if it wnat to be double or float or anything else
     */

    std::vector<T>        _t;
    void            localSimulate(double dt) override;

    /*
     * Controll curve
     */

    PCurve<T,3>*  model_curve;
    DVector<PCurve<T, 3>*> _c, loc;
    /*
     * Number of controll points
     */
    int                    _n;

    int                    _sample;

    /*
     * Help functions
     */
    void             makeKnotVectors(int n, T s, T e);
    void             makeKnotVectorsClosed(int n, T s, T e);
    T                getW(int d, int i, T t) const;
    int              findI(T t)const;
    //Create a getBlend(t){ return b} Function and use getBlend(getW(d, i, t)) instead og getW
    //getB computes
    T                getBlend(T t) const;
    Vector<T, 2>     getB( int i, T t) const;
    //void             makeMatix(T t) const;
   // void             getTest();




}; // END class MyBlending_Spline

} // END namepace GMlib

// Include MyBlending_Spline class function implementations

//*****************************************
// Constructors and destructor           **
//*****************************************



template <typename T>
inline
    //_d = degree     _k order
    GMlib::MyBlending_Spline<T>::MyBlending_Spline( PCurve<T,3>* p, int n ) : PCurve<T,3>(20, 0, 0), _d(1), _k(2)  {

    model_curve = p;
    _n = n;

    if (p->isClosed()) {
        _c.setDim(n +1);
        makeKnotVectorsClosed(n, p->getParStart(), p->getParEnd() );
        for (int i =0; i<n; i++){
            _c[i] = new PSubCurve<T>(p, _t[i], _t[i+2], _t[i+1]);
            _c[i]->toggleDefaultVisualizer();
            _c[i]->sample(10,0);
            this->insert(_c[i]);
            _c[i]->setCollapsed(true);
        }
        _c[n] = _c[0];
    }
    else{
        _c.setDim(n);
        makeKnotVectors(n, p->getParStart(), p->getParEnd());
        for (int i =0; i<n; i++){
            _c[i] = new PSubCurve<T>(p, _t[i], _t[i+2], _t[i+1]);
            _c[i]->toggleDefaultVisualizer();
            _c[i]->sample(10,0);
            this->insert(_c[i]);
            _c[i]->setCollapsed(true);
        }
    }

}

template <typename T>
inline
    GMlib::MyBlending_Spline<T>::MyBlending_Spline( const MyBlending_Spline<T>& copy) : PCurve<T,3>(20, 0, 0), _d(1), _k(2)  {

    _d = copy._d;
    _k = copy._k;
    _t = copy._t;
    _c = copy._c;
    _n = copy._n;
}


template <typename T>
GMlib::MyBlending_Spline<T>::~MyBlending_Spline() {}


//***************************************************
// Overrided (public) virtual functons from PCurve **
//***************************************************

template <typename T>
bool GMlib::MyBlending_Spline<T>::isClosed() const {
    return model_curve->isClosed();
}



//******************************************************
// Overrided (protected) virtual functons from PCurve **
//******************************************************


template <typename T>
void GMlib::MyBlending_Spline<T>::eval( T t, int d, bool /*l*/ ) const {

    this->_p.setDim( d + 1 );

    int i = findI(t);

    Vector<T, 2> b = getB(i, t);


    //use evaluete parent because it uses the matrix
    DVector<Vector<T,3>> c1 = _c[i-1]->evaluateParent(t, d);
    DVector<Vector<T,3>> c2 = _c[i]->evaluateParent(t, d);

    this->_p[0] = b[0]* c1[0] + b[1]* c2[0];

}

template <typename T>
T GMlib::MyBlending_Spline<T>::getStartP() const {
    /*
     * Strtknot is at D
     */
    return _t[_d];
}


template <typename T>
T GMlib::MyBlending_Spline<T>::getEndP()const {
    /*
       * Endknot is equal to n
       */
    return _t[_c.getDim()];
}

template <typename T>
void GMlib::MyBlending_Spline<T>::makeKnotVectors(int n, T s, T e){

    //Ask model_curve if open /n-1 if closed /n


    for( int i = 0; i < 2; i++)
        _t.push_back(s);


    T dt = (e-s)/(n-1);
    for( int i = 2; i < _c.getDim(); i++)
        _t.push_back(s + (i-1)* dt);


    for( int i = 0; i < 2; i++)
        _t.push_back(s);



}


template <typename T>
void GMlib::MyBlending_Spline<T>::makeKnotVectorsClosed(int n, T s, T e){


    T dt = (e-s)/(n);
    _t.push_back(s-dt);

    for( int i = 1; i < n+_k; i++)
        _t.push_back(s + (i-1)* dt);



}





template <typename T>
//function that maps the interior knot interval
T GMlib::MyBlending_Spline<T>::getW(int d, int i, T t)const{
    return (t-_t[i])/(_t[i+d]-_t[i]);
}


template <typename T>
int GMlib::MyBlending_Spline<T>::findI(T t)const{

    for(int i = _d; i <_c.getDim(); i++)
        if (t >=_t[i] && t < _t[i+1]){
            return i;
        }
    return _c.getDim()-1;


}


template <typename T>
GMlib::Vector<T, 2>  GMlib::MyBlending_Spline<T>::getB( int i, T t)const{
    //in order to aplly the B function to the knot interval (getblend(getw())
    T w1 = getBlend(getW(1, i, t));
    Vector<T, 3> b;
    b[0] = (1-w1);
    b[1] = w1;

    return b;
}
// symmetric B-function of polynomial function of first order
template <typename T>
T  GMlib::MyBlending_Spline<T>::getBlend(T t)const{
    T b = 3*pow(t, 2) - 2*pow(t, 3);
    return b;
}



template <typename T>
void GMlib::MyBlending_Spline<T>::localSimulate(double dt){

    static int cont = 1;
    static bool out = true;
    static double in = 0.3;
    static bool start = true;
    static double xin = 0.2;
    static double yin = 0.1;
    static double pi = 3.14;
    static double omega = 0.01;
    static double radius = 2;
    static double t = 1;
    static double k = 2;
    static double x, y;
    static double degree = 70;
    static int test = 0;
    static bool test2;

    if (cont % 25 == 0 && cont % 50 != 0)
        test2 = true;

    else if (cont % 50 == 0)
        test2 = false;


    double scale= 500.0/cont;
    GMlib::Color scarlet = scale * GMlib::GMcolor::black() + (1 - scale) * GMlib::GMcolor::red();
    this->setColor(scarlet);


    if (start){
        this->rotateParent(10, UnitVector<T, 3>(0, 0, 1) );
        start = !start;

    }



    if (cont == 126){
        xin = xin;
        yin = -yin;
        this->rotateParent(-100, UnitVector<T, 3>(0, 0, 1) );

    }

    if (cont == 251){
        xin = -xin;
        yin = yin;
        this->rotateParent(-80, UnitVector<T, 3>(0 , 0, 1) );
    }
    if (cont == 376){
        xin = xin;
        yin = -yin;
        this->rotateParent(-90, UnitVector<T, 3>(0 , 0, 1) );
    }

    if (cont == 500 || cont == 0){
        out = !out;
        cont = 0;
        yin = yin;
        xin = -xin;
        this->rotateParent(-90, UnitVector<T, 3>(0 , 0, 1) );
    }





    if(model_curve) {
        std::vector<Vector<T, 3>> v;

        for (int j = 0; j <_n; j++){
            v.push_back(UnitVector<T, 3>(j,0, j+1));
        }

        for(int i=0; i<_n; i++){
            _c[3]->rotateParent(0.5*dt, v[i]);
            _c[4]->rotateParent(0.5*dt, v[i]);
            _c[0]->rotateParent(0.5*dt, UnitVector<T, 3>(0, 1, 0));

            this->translateParent(UnitVector<T, 3>(dt * xin , 0, dt * yin), true);

            test = pow(-1, i+1);

            if (test2)
                this->rotateParent(0.1*dt, -v[i]);
            else
                this->rotateParent(0.1*dt, v[i]);
        }

        this->setEditDone(true);
        PCurve<T,3>::sample(_sample,0);
        cont ++;




    }


}


template <typename T>
void GMlib::MyBlending_Spline<T>::sample(int m, int d = 0){

    _sample = m;
    PCurve<T,3>::sample(m, d);
}

#endif // MYBLENDINGSPLINE_H
