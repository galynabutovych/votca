/*
 * Copyright 2009-2018 The VOTCA Development Team (http://www.votca.org)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE bse_test
#include <boost/test/unit_test.hpp>
#include <votca/xtp/linalg.h>
#include <iostream>

using namespace votca::xtp;

BOOST_AUTO_TEST_SUITE(linalg_test)

BOOST_AUTO_TEST_CASE(linalg_mkl_test) {
  
  
 int nmax=10;
  
  Eigen::MatrixXd H=Eigen::MatrixXd::Zero(17,17);
  H<<13.2967782,-1.99797328,0,0,0,-0.26575698,0,0,0,-0.0909339466,-0.147466802,-0.0909339466,-0.147466802,-0.0909339466,-0.147466802,-0.0909339466,-0.147466802,
-1.99797328,-4.04412972,0,0,0,-3.49418055,0,0,0,-0.994581408,-1.89398582,-0.994581408,-1.89398582,-0.994581408,-1.89398582,-0.994581408,-1.89398582,
0,0,-3.93848515,0,0,0,-2.25634153,0,0,-0.780335933,-0.599314142,-0.780335933,-0.599314142,0.780335933,0.599314142,0.780335933,0.599314142,
0,0,0,-3.5,0,0,0,-2.25634153,0,-0.780335933,-0.599314142,0.780335933,0.599314142,0.780335933,0.599314142,-0.780335933,-0.599314142,
0,0,0,0,-3.0,0,0,0,-2.25634153,-0.780335933,-0.599314142,0.780335933,0.599314142,-0.780335933,-0.599314142,0.780335933,0.599314142,
-0.26575698,-3.49418055,0,0,0,-2.9,0,0,0,-1.38139383,-2.47288528,-1.38139383,-2.47288528,-1.38139383,-2.47288528,-1.38139383,-2.47288528,
0,0,-2.25634153,0,0,0,-2.8,0,0,-1.03641022,-0.99951947,-1.03641022,-0.99951947,1.03641022,0.99951947,1.03641022,0.99951947,
0,0,0,-2.25634153,0,0,0,-2.7,0,-1.03641022,-0.99951947,1.03641022,0.99951947,1.03641022,0.99951947,-1.03641022,-0.99951947,
0,0,0,0,-2.25634153,0,0,0,-2.6,-1.03641022,-0.99951947,1.03641022,0.99951947,-1.03641022,-0.99951947,1.03641022,0.99951947,
-0.0909339466,-0.994581408,-0.780335933,-0.780335933,-0.780335933,-1.38139383,-1.03641022,-1.03641022,-1.03641022,-2.5,-2.29509192,-0.0552940511,-0.512094198,-0.0552940511,-0.512094198,-0.0552940511,-0.512094198,
-0.147466802,-1.89398582,-0.599314142,-0.599314142,-0.599314142,-2.47288528,-0.99951947,-0.99951947,-0.99951947,-2.29509192,-2.4,-0.512094198,-1.30279378,-0.512094198,-1.30279378,-0.512094198,-1.30279378,
-0.0909339466,-0.994581408,-0.780335933,0.780335933,0.780335933,-1.38139383,-1.03641022,1.03641022,1.03641022,-0.0552940511,-0.512094198,-2.3,-2.29509192,-0.0552940511,-0.512094198,-0.0552940511,-0.512094198,
-0.147466802,-1.89398582,-0.599314142,0.599314142,0.599314142,-2.47288528,-0.99951947,0.99951947,0.99951947,-0.512094198,-1.30279378,-2.29509192,-2.2,-0.512094198,-1.30279378,-0.512094198,-1.30279378,
-0.0909339466,-0.994581408,0.780335933,0.780335933,-0.780335933,-1.38139383,1.03641022,1.03641022,-1.03641022,-0.0552940511,-0.512094198,-0.0552940511,-0.512094198,-1.9,-2.29509192,-0.0552940511,-0.512094198,
-0.147466802,-1.89398582,0.599314142,0.599314142,-0.599314142,-2.47288528,0.99951947,0.99951947,-0.99951947,-0.512094198,-1.30279378,-0.512094198,-1.30279378,-2.29509192,-1.8,-0.512094198,-1.30279378,
-0.0909339466,-0.994581408,0.780335933,-0.780335933,0.780335933,-1.38139383,1.03641022,-1.03641022,1.03641022,-0.0552940511,-0.512094198,-0.0552940511,-0.512094198,-0.0552940511,-0.512094198,-1.5,-2.29509192,
-0.147466802,-1.89398582,0.599314142,-0.599314142,0.599314142,-2.47288528,0.99951947,-0.99951947,0.99951947,-0.512094198,-1.30279378,-0.512094198,-1.30279378,-0.512094198,-1.30279378,-2.29509192,-1;

Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es(H);
Eigen::MatrixXd V_ref = es.eigenvectors().block(0,0,H.rows(),nmax);
Eigen::VectorXd E_ref = es.eigenvalues().segment(0,nmax);

Eigen::VectorXd E;
Eigen::MatrixXd V;
linalg_eigenvalues(H, E, V , nmax );
bool check_energy=E_ref.isApprox(E,0.0001);

bool check_vector=V.cwiseAbs().isApprox(V_ref.cwiseAbs(),0.0001);


if(!check_vector || !check_energy){
  std::cout<<"E_ref"<<std::endl;
  std::cout<<E_ref<<std::endl;
  std::cout<<"E"<<std::endl;
  std::cout<<E<<std::endl;
  std::cout<<"V_ref"<<std::endl;
  std::cout<<V_ref<<std::endl;
  std::cout<<"V"<<std::endl;
  std::cout<<V<<std::endl;
}
 
 
  

BOOST_CHECK_EQUAL(check_energy, 1);
BOOST_CHECK_EQUAL(check_vector, 1);

}

BOOST_AUTO_TEST_SUITE_END()
