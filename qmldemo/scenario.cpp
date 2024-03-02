
#include <iostream>

#include "scenario.h"
#include "testtorus.h"


// hidmanager
#include "hidmanager/defaulthidmanager.h"

// gmlib
#include <scene/light/gmpointlight.h>
#include <scene/sceneobjects/gmpathtrack.h>
#include <scene/sceneobjects/gmpathtrackarrows.h>
#include "parametrics/curves/gmpline.h"
#include <parametrics/surfaces/gmpplane.h>
#include <parametrics/surfaces/gmpcylinder.h>
#include "parametrics/curves/gmpcircle.h"



// qt
#include <QQuickItem>
#include "D:/Qt Projects/Applied Geometry/qmldemo/spline.h"
#include "D:/Qt Projects/Applied Geometry/qmldemo/ClosedSubdivision.h"
#include "D:/Qt Projects/Applied Geometry/qmldemo/modelcurve.h"
#include "D:/Qt Projects/Applied Geometry/qmldemo/Blending_Spline.h"



template <typename T>
inline
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
  out << v.size() << std::endl;
  for(uint i=0; i<v.size(); i++) out << " " << v[i];
  out << std::endl;
  return out;
}




void Scenario::initializeScenario() {

  // Insert a light
  GMlib::Point<GLfloat,3> init_light_pos( 2.0, 4.0, 10 );
  GMlib::PointLight *light = new GMlib::PointLight(  GMlib::GMcolor::white(), GMlib::GMcolor::white(),
                                                     GMlib::GMcolor::white(), init_light_pos );
  light->setAttenuation(0.8f, 0.002f, 0.0008f);
  this->scene()->insertLight( light, false );

  // Insert Sun
  this->scene()->insertSun();

  // Default camera parameters
  int init_viewport_size = 600;
  GMlib::Point<float,3>  init_cam_pos( 0.0f, 0.0f, 0.0f );
  GMlib::Vector<float,3> init_cam_dir( 0.0f, 1.0f, 0.0f );
  GMlib::Vector<float,3> init_cam_up(  1.0f, 0.0f, 0.0f );

  // Projection cam
  auto proj_rcpair = createRCPair("Projection");
  proj_rcpair.camera->set(init_cam_pos,init_cam_dir,init_cam_up);
  proj_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
  proj_rcpair.camera->rotateGlobal( GMlib::Angle(-45), GMlib::Vector<float,3>( 1.0f, 0.0f, 0.0f ) );
  proj_rcpair.camera->translateGlobal( GMlib::Vector<float,3>( 0.0f, -20.0f, 20.0f ) );
  scene()->insertCamera( proj_rcpair.camera.get() );
  proj_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );


  /***************************************************************************
   *                                                                         *
   * Standar example, including path track and path track arrows             *
   *                                                                         *
   ***************************************************************************/

  GMlib::Material mm(GMlib::GMmaterial::polishedBronze());
  mm.set(45.0);

  /*auto ptom = new TestTorus(1.0f, 0.4f, 0.6f);
  ptom->toggleDefaultVisualizer();
  ptom->sample(60,60,1,1);
  this->scene()->insert(ptom);
  auto ptrack = new GMlib::PathTrack();
  ptrack->setLineWidth(2);
  ptom->insert(ptrack);
  auto ptrack2 = new GMlib::PathTrackArrows();
  ptrack2->setArrowLength(2);
  ptom->insert(ptrack2);
*/

  //Bspline

 DVector<Vector<float,3>> c(7);
  c[0] ={0,0,0};
  c[1] ={1,0,0};
  c[2] ={1,1,0};
  c[3] ={2,0,0};
  c[4] ={2,1,0};
  c[5] ={3,0,0};
  c[6] ={3,1,0};
  auto spl = new spline<float>(c);
  spl->toggleDefaultVisualizer();
  spl->sample(60,0);
 // this->scene()->insert(spl);


//CLosed_Subdivision
DVector<Vector<float,3>> x(6);

x[0] = Vector<float, 3>(0,0,0);
x[1] = Vector<float, 3> (0,2, 0);
x[2] = Vector<float, 3>(1,3, 0);
x[3] = Vector<float, 3>(2,2, 0);
x[4] = Vector<float, 3>(2,0, 0);
x[5] = Vector<float, 3>(1,1, 0);

auto model = new closed_subdivision<float>(x);
model->toggleDefaultVisualizer();
model->sample(3,2);
//this->scene()->insert(model);



  //Model_Curve trough Blending Curve
  auto test2 = new GMlib::Test_Curve_2<float>(5);

  auto myBlending_Spline_curve = new GMlib::MyBlending_Spline<float>(test2, 5);
  myBlending_Spline_curve->toggleDefaultVisualizer();
  myBlending_Spline_curve->sample(80, 0);
  this->scene()->insert(myBlending_Spline_curve);


  //Regular Model Curve
  auto test = new GMlib::Test_Curve_2<float>(7);
  test->toggleDefaultVisualizer();
  test->setColor(GMlib::GMcolor::green());
  test->sample(80, 0);
  //this->scene()->insert(test);


}




void Scenario::cleanupScenario() {

}




void Scenario::callDefferedGL() {

  GMlib::Array< const GMlib::SceneObject*> e_obj;
  this->scene()->getEditedObjects(e_obj);

  for(int i=0; i < e_obj.getSize(); i++)
    if(e_obj(i)->isVisible()) e_obj[i]->replot();
}

