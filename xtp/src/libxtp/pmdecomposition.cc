/*
 *            Copyright 2009-2020 The VOTCA Development Team
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
 * Reference- A fast intrinsic localization procedure applicable for ab initio
 * and semiempirical linear combination of atomic orbital wave functions J.
 * Chem. Phys. 90, 4916 (1989); https://doi.org/10.1063/1.456588 János Pipek and
 * Paul G. Mezey
 */

#include "votca/xtp/pmdecomposition.h"
#include "votca/xtp/aomatrix.h"
#include <limits>

namespace votca {
namespace xtp {
void PMDecomposition::computePMD(Orbitals &orbitals) {
  Eigen::MatrixXd occ_orbitals = orbitals.MOs().eigenvectors().leftCols(
      orbitals.getNumberOfAlphaElectrons());
  aobasis = orbitals.SetupDftBasis();
  AOOverlap overlap;
  overlap.Fill(aobasis);
  double convergence_limit = std::numeric_limits<double>::max();
  Index iteration = 1;

  while (convergence_limit > 1e-6 && iteration < 10000) {
    XTP_LOG(Log::error, log_) << "Iteration: " << iteration << std::flush;
    Eigen::MatrixXd orbpair_functionalvalue =
        orbitalselections(occ_orbitals, overlap.Matrix());
    Index maxrow, maxcol;
    convergence_limit = orbpair_functionalvalue.maxCoeff(&maxrow, &maxcol);
    XTP_LOG(Log::error, log_)
        << "Orbitals to be changed: " << maxrow << " " << maxcol << std::flush;
    XTP_LOG(Log::error, log_)
        << "change in the penalty function: " << convergence_limit
        << std::flush;
    Eigen::MatrixX2d max_orbs(occ_orbitals.rows(), 2);
    max_orbs << occ_orbitals.col(maxrow), occ_orbitals.col(maxcol);
    Eigen::MatrixX2d new_orbs = rotateorbitals(max_orbs, maxrow, maxcol);
    occ_orbitals.col(maxrow) = new_orbs.col(0);
    occ_orbitals.col(maxcol) = new_orbs.col(1);
    iteration++;
  }
  orbitals.setPMLocalizedOrbital(occ_orbitals);
  orbitals.WriteToCpt("PML.orb");
}

// Function to rotate the 2 maximum orbitals
Eigen::MatrixX2d PMDecomposition::rotateorbitals(Eigen::MatrixX2d &maxorbs,
                                                 Index s, Index t) {
  double gam =
      0.25 * asin(B(s, t) / sqrt((A(s, t) * A(s, t)) + (B(s, t) * B(s, t))));
  Eigen::MatrixX2d neworbitals(maxorbs.rows(), 2);
  neworbitals.col(0) =
      (std::cos(gam) * maxorbs.col(0)) + (std::sin(gam) * maxorbs.col(1));
  neworbitals.col(1) =
      -1 * (std::sin(gam) * maxorbs.col(0)) + (std::cos(gam) * maxorbs.col(1));
  XTP_LOG(Log::error, log_)
      << "Sine of the rotation angle = " << std::sin(gam) << std::flush;
  return neworbitals;
}

// Function to select n(n-1)/2 orbitals and process Ast and Bst
Eigen::MatrixXd PMDecomposition::orbitalselections(Eigen::MatrixXd &m,
                                                   const Eigen::MatrixXd &S) {
  Eigen::MatrixXd orbital_pairs_functional =
      Eigen::MatrixXd::Zero(m.cols(), m.cols());
  A = Eigen::MatrixXd::Zero(m.cols(), m.cols());
  B = Eigen::MatrixXd::Zero(m.cols(), m.cols());
  for (Index s = 0; s < m.cols(); s++) {
    for (Index t = s + 1; t < m.cols(); t++) {
      Eigen::RowVectorXd sps =
          (m.col(s).asDiagonal() * S * m.col(s).asDiagonal())
              .colwise()
              .sum();  // vec1.S.vec1
      Eigen::MatrixXd spt_split =
          m.col(s).asDiagonal() * S *
          m.col(t).asDiagonal();  // terms of eq 31 referenced above
      Eigen::RowVectorXd tpt =
          (m.col(t).asDiagonal() * S * m.col(t).asDiagonal())
              .colwise()
              .sum();  // vec2.S.vec2
      Eigen::RowVectorXd spt = 0.5 * (spt_split.colwise().sum() +
                                      spt_split.rowwise().sum().transpose());
      std::vector<Index> numfuncpatom = aobasis.getFuncPerAtom();
      Index start = 0;
      double Ast = 0;
      double Bst = 0;
      for (Index atom_id = 0; atom_id < Index(numfuncpatom.size()); atom_id++) {
        double sps_x = sps.segment(start, numfuncpatom[atom_id]).sum();
        double spt_x = spt.segment(start, numfuncpatom[atom_id]).sum();
        double tpt_x = tpt.segment(start, numfuncpatom[atom_id]).sum();
        Ast += spt_x * spt_x - 0.25 * ((sps_x - tpt_x) * (sps_x - tpt_x));
        Bst += spt_x * (sps_x - tpt_x);
        start += numfuncpatom[atom_id];
      }
      A(s, t) = Ast;
      B(s, t) = Bst;
      orbital_pairs_functional(s, t) = Ast + sqrt((Ast * Ast) + (Bst * Bst));
    }
  }
  return orbital_pairs_functional;
}

}  // namespace xtp
}  // namespace votca