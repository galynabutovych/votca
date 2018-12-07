/*
 *            Copyright 2009-2018 The VOTCA Development Team
 *                       (http://www.votca.org)
 *
 *      Licensed under the Apache License, Version 2.0 (the "License")
 *
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef _VOTCA_XTP_GW_H
#define _VOTCA_XTP_GW_H

#include <votca/xtp/orbitals.h>
#include <votca/xtp/ppm.h>
#include <votca/xtp/threecenter.h>

namespace votca {
namespace xtp {

class Sigma;

class GW {     
 public:
    GW(ctp::Logger &log, TCMatrix_gwbse& Mmn,const Eigen::MatrixXd& vxc,
       const Eigen::VectorXd& dft_energies):
     _log(log),_Mmn(Mmn),_vxc(vxc),_dft_energies(dft_energies){};
    
    struct options{
        int homo;
        int qpmin;
        int qpmax;
        int rpamin;
        int rpamax;
        double g_sc_limit=1e-5;
        int g_sc_max_iterations=20;
        double gw_sc_limit=1e-5;
        int gw_sc_max_iterations=10;
        double shift=0;
        double ScaHFX=0.0;
        int _reset_3c=5; //how often the 3c integrals in iterate should be rebuild
    };

    void configure(const options& opt){
     _opt=opt;
     _qptotal=_opt.qpmax-_opt.qpmin+1;
  }

    Eigen::MatrixXd getGWAResults()const;
    //Calculates the diagonal elements up to self consistency
    void CalculateGWPerturbation();



    //Calculated offdiagonal elements as well
    void CalculateHQP();

    Eigen::MatrixXd getHQP()const{
        return _dft_energies.segment(_opt.qpmin,_qptotal).asDiagonal()+(1-_opt.ScaHFX)*_Sigma_x+_Sigma_c-_vxc;  
    }
    //Diagonalize QP particle Hamiltonian
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> DiagonalizeQPHamiltonian()const{
        return Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd>(getHQP());
    }

 private:
     int _qptotal;

     Eigen::VectorXd _gwa_energies;

     Eigen::MatrixXd _Sigma_x;
     Eigen::MatrixXd _Sigma_c;

    options _opt;

    ctp::Logger &_log;
    const Eigen::MatrixXd& _vxc;
    const Eigen::VectorXd& _dft_energies;
    TCMatrix_gwbse& _Mmn;

    double CalcShift()const;
 Eigen::VectorXd ScissorShift_DFTlevel(const Eigen::VectorXd& dft_energies)const;
 void PrintQP_Energies()const;
 void PrintGWA_Energies()const;
      

};
}
}

#endif /* _VOTCA_XTP_BSE_H */
