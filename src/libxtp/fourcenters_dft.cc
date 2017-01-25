/* 
 *            Copyright 2009-2016 The VOTCA Development Team
 *                       (http://www.votca.org)
 *
 *      Licensed under the Apache License, Version 2.0 (the "License")
 *
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */



#include <votca/xtp/threecenters.h>



using namespace votca::tools;

namespace votca {
    namespace xtp {
        namespace ub = boost::numeric::ublas;

        /*
         * Cleaning FCMatrix_dft data and free memory
         */
        void FCMatrix_dft::Cleanup() {

            for (unsigned _i = 0; _i < _matrix.size(); _i++) {
                _matrix[ _i ].resize(0, 0, false);
            }
            _matrix.clear();

        } // FCMatrix_dft::Cleanup








        void FCMatrix_dft::Fill_4c_small_molecule(AOBasis& dftbasis) {

          //cout << endl;
          //cout << "fourcenters_dft.cc FCMatrix_dft::Fill_4c_small_molecule" << endl;
          int dftBasisSize = dftbasis._AOBasisSize;
          int vectorSize = (dftBasisSize*(dftBasisSize+1))/2;
          _4c_vector = ub::zero_vector<double>((vectorSize*(vectorSize+1))/2);

          int _ishell = 0;
          #pragma omp parallel for
          for (std::vector< AOShell* >::iterator _it_3 = dftbasis.firstShell(); _it_3 != dftbasis.lastShell(); _it_3++) {
            AOShell* _shell_3 = dftbasis.getShell(_it_3);
            int _start_3 = _shell_3->getStartIndex();
            int NumFunc_3 = _shell_3->getNumFunc();
            _ishell++;
            //cout << "_ishell = " << _ishell << endl;

            for (std::vector< AOShell* >::iterator _it_4 = _it_3; _it_4 != dftbasis.lastShell(); _it_4++) {
              AOShell* _shell_4 = dftbasis.getShell(_it_4);
              int _start_4 = _shell_4->getStartIndex();
              int NumFunc_4 = _shell_4->getNumFunc();                

              for (std::vector< AOShell* >::iterator _it_1 = _it_3; _it_1 != dftbasis.lastShell(); _it_1++) {
                AOShell* _shell_1 = dftbasis.getShell(_it_1);
                int _start_1 = _shell_1->getStartIndex();
                int NumFunc_1 = _shell_1->getNumFunc();

                for (std::vector< AOShell* >::iterator _it_2 = _it_1; _it_2 != dftbasis.lastShell(); _it_2++) {
                  AOShell* _shell_2 = dftbasis.getShell(_it_2);
                  int _start_2 = _shell_2->getStartIndex();
                  int NumFunc_2 = _shell_2->getNumFunc();

                  // get 4-center directly as _subvector
                  ub::matrix<double> _subvector = ub::zero_matrix<double>(NumFunc_1 * NumFunc_2, NumFunc_3 * NumFunc_4);
                  bool nonzero=FillFourCenterRepBlock(_subvector, _shell_1, _shell_2, _shell_3, _shell_4);

                  if (nonzero) {

                    for (int _i_3 = 0; _i_3 < NumFunc_3; _i_3++) {
                      int ind_3 = _start_3 + _i_3;
                      int sum_ind_3 = (ind_3*(ind_3+1))/2;
                      for (int _i_4 = 0; _i_4 < NumFunc_4; _i_4++) {
                        int ind_4 = _start_4 + _i_4;
                        if (ind_3 > ind_4) continue;
                        int _index_subv_34 = NumFunc_3 * _i_4 + _i_3;
                        int _index_34 = dftBasisSize * ind_3 - sum_ind_3 + ind_4;
                        int _index_34_12_a = vectorSize * _index_34 - (_index_34*(_index_34+1))/2;
                        for (int _i_1 = 0; _i_1 < NumFunc_1; _i_1++) {
                          int ind_1 = _start_1 + _i_1;
                          int sum_ind_1 = (ind_1*(ind_1+1))/2;
                          for (int _i_2 = 0; _i_2 < NumFunc_2; _i_2++) {
                            int ind_2 = _start_2 + _i_2;
                            if (ind_1 > ind_2) continue;
                            int _index_12 = dftBasisSize * ind_1 - sum_ind_1 + ind_2;
                            if (_index_34 > _index_12) continue;
                            //int _index_subv_12 = NumFunc_1 * _i_2 + _i_1;

                            _4c_vector(_index_34_12_a + _index_12) = _subvector(NumFunc_1 * _i_2 + _i_1, _index_subv_34);

                          } // _i_2
                        } // _i_1
                      } // _i_4
                    } // _i_3

                  } // end if

                } // DFT shell_2
              } // DFT shell_1
            } // DFT shell_4
          } // DFT shell_3


        } // FCMatrix_dft::Fill_4c_small_molecule
        
        void FCMatrix_dft::Fill_4c_large_molecule(AOBasis& dftbasis) {

          //cout << endl;
          //cout << "fourcenters_dft.cc FCMatrix_dft::Fill" << endl;
          int dftBasisSize = dftbasis._AOBasisSize;
          for (int i = 0; i < (dftBasisSize*(dftBasisSize+1))/2; i++){
            _matrix.push_back(ub::zero_matrix<double>(dftBasisSize, dftBasisSize));        
          }

          int _ishell = 0;
          for (std::vector< AOShell* >::iterator _it_3 = dftbasis.firstShell(); _it_3 != dftbasis.lastShell(); _it_3++) {
            AOShell* _shell_3 = dftbasis.getShell(_it_3);
            int _start_3 = _shell_3->getStartIndex();
            int NumFunc_3 = _shell_3->getNumFunc();
            _ishell++;
            //cout << "_ishell = " << _ishell << endl;

            for (std::vector< AOShell* >::iterator _it_4 = _it_3; _it_4 != dftbasis.lastShell(); _it_4++) {
              AOShell* _shell_4 = dftbasis.getShell(_it_4);
              int _start_4 = _shell_4->getStartIndex();
              int NumFunc_4 = _shell_4->getNumFunc();                

////              for (std::vector< AOShell* >::iterator _it_1 = _it_3; _it_1 != dftbasis.lastShell(); _it_1++) {
              for (std::vector< AOShell* >::iterator _it_1 = dftbasis.firstShell(); _it_1 != dftbasis.lastShell(); _it_1++) {
                AOShell* _shell_1 = dftbasis.getShell(_it_1);
                int _start_1 = _shell_1->getStartIndex();
                int NumFunc_1 = _shell_1->getNumFunc();

                for (std::vector< AOShell* >::iterator _it_2 = _it_1; _it_2 != dftbasis.lastShell(); _it_2++) {
                  AOShell* _shell_2 = dftbasis.getShell(_it_2);
                  int _start_2 = _shell_2->getStartIndex();
                  int NumFunc_2 = _shell_2->getNumFunc();

                  // get 4-center directly as _subvector
                  ub::matrix<double> _subvector = ub::zero_matrix<double>(NumFunc_1 * NumFunc_2, NumFunc_3 * NumFunc_4);
                  bool nonzero=FillFourCenterRepBlock(_subvector, _shell_1, _shell_2, _shell_3, _shell_4);

                  if (nonzero) {

                    for (int _i_3 = 0; _i_3 < NumFunc_3; _i_3++) {
                      int ind_3 = _start_3 + _i_3;
                      int sum_ind_3 = (ind_3*(ind_3+1))/2;
                      for (int _i_4 = 0; _i_4 < NumFunc_4; _i_4++) {
                        int ind_4 = _start_4 + _i_4;
                        if (ind_3 > ind_4) continue;
                        int _index_34 = NumFunc_3 * _i_4 + _i_3;
                        int _first_index_34 = dftBasisSize * ind_3 - sum_ind_3 + ind_4;
                        for (int _i_1 = 0; _i_1 < NumFunc_1; _i_1++) {
                          int ind_1 = _start_1 + _i_1;
//                          int sum_ind_1 = (ind_1*(ind_1+1))/2;
                          for (int _i_2 = 0; _i_2 < NumFunc_2; _i_2++) {
                            int ind_2 = _start_2 + _i_2;
                            if (ind_1 > ind_2) continue;
                            int _index_12 = NumFunc_1 * _i_2 + _i_1;
//                            int _first_index_12 = dftBasisSize * ind_1 - sum_ind_1 + ind_2;
                            double value = _subvector(_index_12, _index_34);

                            if (ind_1 == ind_2) {
                              _matrix[_first_index_34](ind_1, ind_2) = value;
                            } else {
                              _matrix[_first_index_34](ind_1, ind_2) = 2.*value;
                            }
/*
                            if ((_it_1 != _it_3)||(_it_2 != _it_4)) {
                              if (ind_3 == ind_4) {
                                _matrix[_first_index_12](ind_3, ind_4) = value;
                              } else {
                                _matrix[_first_index_12](ind_3, ind_4) = 2.*value;
                              }
                            }
*/

                          } // _i_2
                        } // _i_1
                      } // _i_4
                    } // _i_3

                  } // end if

                } // DFT shell_2
              } // DFT shell_1
            } // DFT shell_4
          } // DFT shell_3


        } // FCMatrix_dft::Fill_4c_large_molecule







 


    }
}
