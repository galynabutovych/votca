/*
 *            Copyright 2009-2012 The VOTCA Development Team
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

// Overload of uBLAS prod function with MKL/GSL implementations
#include <votca/ctp/votca_ctp_config.h>

#include <votca/ctp/gwbse.h>

#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <votca/ctp/aomatrix.h>
#include <votca/ctp/threecenters.h>
// #include <votca/ctp/logger.h>
#include <votca/ctp/qmpackagefactory.h>
#include <boost/math/constants/constants.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include <votca/tools/linalg.h>

using boost::format;
using namespace boost::filesystem;

namespace votca {
    namespace ctp {
        namespace ub = boost::numeric::ublas;

        // +++++++++++++++++++++++++++++ //
        // MBPT MEMBER FUNCTIONS         //
        // +++++++++++++++++++++++++++++ //

        
        

        void GWBSE::FullQPHamiltonian(){
            
            // constructing full QP Hamiltonian, storage in vxc
            _vxc = -_vxc + _sigma_x + _sigma_c;
            // diagonal elements are given by _qp_energies
            for (int _m = 0; _m < _vxc.size1(); _m++ ){
              _vxc( _m,_m ) = _qp_energies( _m + _qpmin );
            }

            
            // sigma matrices can be freed
            _sigma_x.resize(0,0);
            _sigma_c.resize(0,0);
            
            
            if ( _do_qp_diag ){
            /* diagonalize, since _vxc will be needed in BSE, and GSL
             * destroys the input array, we need to make a local copy first
             */
            
            // get eigenvalues and eigenvectors of this matrix
            ub::matrix<double> _temp = _vxc;
            _qp_diag_energies.resize(_temp.size1());
            _qp_diag_coefficients.resize(_temp.size1(), _temp.size1());
            linalg_eigenvalues(_temp, _qp_diag_energies, _qp_diag_coefficients);

            // TODO storage -> orbitals

            }
            
        }
        
        void GWBSE::sigma_c_setup(const TCMatrix& _Mmn, const ub::vector<double>& _edft) {

            // iterative refinement of qp energies
            int _max_iter = 5;
            int _levelsum = _Mmn[0].size2(); // total number of bands
            int _gwsize = _Mmn[0].size1(); // size of the GW basis
            const double pi = boost::math::constants::pi<double>();


            // initial _qp_energies are dft energies
            _qp_energies = _edft; // RANGES!
            double _DFTgap = _qp_energies(_homo + 1) - _qp_energies(_homo);

            _sigma_c = ub::zero_matrix<double>(_qptotal,_qptotal);
            // only diagonal elements except for in final iteration
            for (int _i_iter = 0; _i_iter < _max_iter - 1; _i_iter++) {

                // initialize sigma_c to zero at the beginning of each iteration
                

	      // loop over all GW levels
              #pragma omp parallel for
	      for (int _gw_level = 0; _gw_level < _qptotal ; _gw_level++ ){
                double qp_energy=_qp_energies( _gw_level + _qpmin  );
                double sigma_c=0.0;
                
                const ub::matrix<double>& Mmn = _Mmn[ _gw_level + _qpmin ];
              
		// loop over all functions in GW basis
		for ( int _i_gw = 0; _i_gw < _gwsize ; _i_gw++ ){
                  double ppm_freq=_ppm_freq( _i_gw );
                  double ppm_weight=_ppm_weight( _i_gw );
                  
                    
		  // loop over all bands
		  for ( int _i = 0; _i < _homo+1 ; _i++ ){
                                                    
		    // energy denominator
		    double _denom = qp_energy - _qp_energies( _i ) + ppm_freq;
                    double _stab=1.0;
		    if ( std::abs(_denom) < 0.5 ) {
		      _stab = 0.5 * ( 1.0 - cos(2.0 * pi * std::abs(_denom) ) );
		    }
		    double _factor = ppm_weight *ppm_freq * _stab/_denom; // contains conversion factor 2
		    // sigma_c diagonal elements
		    sigma_c+= _factor * Mmn(_i_gw, _i) *  Mmn(_i_gw, _i);  
                            
		  }// bands
                  
                   for ( int _i = _homo+1; _i < _levelsum ; _i++ ){
                               
		    // energy denominator
		    double _denom = qp_energy - _qp_energies( _i ) -ppm_freq;
                    double _stab=1.0;
		    if ( std::abs(_denom) < 0.5 ) {
		      _stab = 0.5 * ( 1.0 - cos(2.0 * pi * std::abs(_denom) ) );
		    } 
		    double _factor = ppm_weight *ppm_freq * _stab/_denom; // contains conversion factor 2
		    // sigma_c diagonal elements
		    sigma_c+= _factor * Mmn(_i_gw, _i) *  Mmn(_i_gw, _i);  
                            
		  }// bands
                        
		}// GW functions
		_sigma_c( _gw_level , _gw_level )=sigma_c;
                    // update _qp_energies
                    _qp_energies(_gw_level + _qpmin) = _edft(_gw_level + _qpmin) + _sigma_x(_gw_level, _gw_level) + _sigma_c(_gw_level, _gw_level) - _vxc(_gw_level, _gw_level);

                }// all bands

            } // iterations

            // check HOMO-LUMO gap shift
            // cout << " QP HOMO : " << _qp_energies( _homo  ) << endl;
            // cout << " QP LUMO : " << _qp_energies( _homo +1 ) << endl;
            double _QPgap = _qp_energies(_homo + 1) - _qp_energies(_homo);
            // cout << " QP Gap  : " << _QPgap << endl;
            double _shift_new = _QPgap - _DFTgap;
            LOG(logDEBUG, *_pLog) << TimeStamp() << " New shift [Ryd]" << _shift_new << flush;
            //cout << " shift new " << _shift_new << endl;
            if (std::abs((_shift_new - _shift)*13.605698066) > 0.01) {
                _shift = _shift_new;
            } else {
                _shift_converged = true;
            }

            if (!_iterate_shift) _shift_converged = true;

            // only if _shift is converged
            if (_shift_converged) {
                // in final step, also calc offdiagonal elements
                // initialize sigma_c to zero at the beginning
                //_sigma_c = ub::zero_matrix<double>(_qptotal,_qptotal);

                // loop over col  GW levels
#pragma omp parallel for
            if ( _shift_converged ){
	    // in final step, also calc offdiagonal elements
	    // initialize sigma_c to zero at the beginning
	    //_sigma_c = ub::zero_matrix<double>(_qptotal,_qptotal);
            
            // loop over col  GW levels
            #pragma omp parallel for
                for (int _gw_col = 0; _gw_col < _qptotal; _gw_col++) {
                    double sigma_c = 0.0;
                    const double qp_energy = _qp_energies(_gw_col + _qpmin);

                    const ub::matrix<double>& Mmn_col = _Mmn[ _gw_col + _qpmin ];
                    ub::matrix<double>Mmn_colxrow = ub::zero_matrix<double>(Mmn_col.size1(), Mmn_col.size2());


                    for (int _gw_row = 0; _gw_row < _qptotal; _gw_row++) {
                        sigma_c = 0.0;
                        for (int _i = 0; _i < _gwsize; _i++) {
                            for (int _j=0;_j<_levelsum;_j++){
                                Mmn_colxrow(_i, _j) = Mmn_col(_i, _j) * _Mmn[ _gw_row + _qpmin ](_i, _j);
                            }
                        }


                        // loop over all functions in GW basis
                        for (int _i_gw = 0; _i_gw < _gwsize; _i_gw++) {
                            const double ppm_freq = _ppm_freq(_i_gw);
                            const double ppm_freqweight = _ppm_weight(_i_gw) * ppm_freq;

                            // loop over occupied screening levels                           
                            for (int _i = 0; _i < _homo + 1; _i++) {
                                // energy denominator
                                double _denom = qp_energy - _qp_energies(_i) + ppm_freq;
                                double _stab = 1.0;
                                if (std::abs(_denom) < 0.5) {
                                    _stab = 0.5 * (1.0 - cos(2.0 * pi * std::abs(_denom)));
                                }
                                sigma_c += ppm_freqweight * _stab / _denom * Mmn_colxrow(_i_gw, _i);
                                //sigma_c += ppm_freqweight * _stab / _denom * Mmn_col(_i_gw, _i) * Mmn_row(_i_gw, _i); //_submat(_i_gw,_i);            
                            }// occupied screening levels
                            // loop over unoccupied screening levels  
                            for (int _i = _homo + 1; _i < _levelsum; _i++) {
                                double _denom = qp_energy - _qp_energies(_i) - ppm_freq;
                                double _stab = 1.0;
                                if (std::abs(_denom) < 0.5) {
                                    _stab = 0.5 * (1.0 - cos(2.0 * pi * std::abs(_denom)));
                                }
                                sigma_c += ppm_freqweight * _stab / _denom * Mmn_colxrow(_i_gw, _i);
                                //sigma_c += ppm_freqweight * _stab / _denom * Mmn_col(_i_gw, _i)* Mmn_row(_i_gw, _i); //_submat(_i_gw,_i);
                            } // unoccupied screening levels
                        }// GW functions
                        _sigma_c(_gw_row, _gw_col) = sigma_c;
                    }// GW row 
                    _qp_energies(_gw_col + _qpmin) = _edft(_gw_col + _qpmin) + _sigma_x(_gw_col, _gw_col) + _sigma_c(_gw_col, _gw_col) - _vxc(_gw_col, _gw_col);
                } // GW col
            }
        } // sigma_c_setup

        void GWBSE::sigma_x_setup(const TCMatrix& _Mmn){
        
            // initialize sigma_x
            _sigma_x = ub::zero_matrix<double>(_qptotal,_qptotal);
            int _size  = _Mmn[0].size1();

            // band 1 loop over all GW levels
            #pragma omp parallel for
            for ( int _m1 = 0 ; _m1 < _qptotal ; _m1++ ){
                
                const ub::matrix<double>& M1mn =  _Mmn[ _m1 + _qpmin ];
                
                // band 2 loop over all GW levels
                //for ( int _m2 = _qpmin ; _m2 <= _qpmax ; _m2++ ){
                for ( int _m2 = 0 ; _m2 < _qptotal ; _m2++ ){
                    
                    const ub::matrix<double>& M2mn =  _Mmn[ _m2 + _qpmin ];
                    
                    // loop over all basis functions
                    for ( int _i_gw = 0 ; _i_gw < _size ; _i_gw++ ){
                        // loop over all occupied bands used in screening
                        for ( int _i_occ = 0 ; _i_occ <= _homo ; _i_occ++ ){
                            _sigma_x( _m1, _m2 ) -= 2.0 * M1mn( _i_gw , _i_occ ) * M2mn( _i_gw , _i_occ );
                        } // occupied bands
                    } // gwbasis functions
                } // level 2
            } // level 1
        
	    // factor for hybrid DFT
	    _sigma_x = ( 1.0 - _ScaHFX ) * _sigma_x;

        }



        

        void GWBSE::sigma_prepare_threecenters(TCMatrix& _Mmn){
            #pragma omp parallel for
            for ( int _m_level = 0 ; _m_level < _Mmn.get_mtot(); _m_level++ ){
                // get Mmn for this _m_level
                // ub::matrix<double> _temp = ub::trans(  _Mmn.matrix()( _m_level )   );
                // and multiply with _ppm_phi = eigenvectors of epsilon
                // POTENTIAL BUG
	      // casting _Mmn to double for efficint prod() overload
	      ub::matrix<double> _Mmn_double = _Mmn[_m_level];

                //ub::matrix<float> _temp = ub::prod(  _ppm_phi , _Mmn[ _m_level ] );
              ub::matrix<float> _temp = ub::prod(  _ppm_phi , _Mmn_double );
                _Mmn[ _m_level ] = _temp;
            }
        }        
        


    }
    
 
};
