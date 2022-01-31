/*
 *            Copyright 2009-2022 The VOTCA Development Team
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

#include "votca/xtp/pmlocalization.h"
#include "votca/xtp/aomatrix.h"
#include <limits>

namespace votca {
namespace xtp {
void PMLocalization::computePML(Orbitals &orbitals) {
  occupied_orbitals = orbitals.MOs().eigenvectors().leftCols(
      orbitals.getNumberOfAlphaElectrons());
  aobasis = orbitals.getDftBasis();
  AOOverlap overlap;
  overlap.Fill(aobasis);
  overlap_ = overlap.Matrix();

  XTP_LOG(Log::error, log_) << std::flush;
  XTP_LOG(Log::error, log_)
      << TimeStamp() << " Starting localization of orbitals" << std::flush;

  // determine initial cost_function
  initial_cost_function();

  Index iteration = 1;
  Index maxrow, maxcol;
  double max_cost = PM_cost_function_.maxCoeff(&maxrow, &maxcol);

  bool not_converged = true;
  while (not_converged && iteration < nrOfIterations_) {

    XTP_LOG(Log::info, log_) << "Iteration: " << iteration << std::flush;

    XTP_LOG(Log::info, log_)
        << "Orbitals to be changed: " << maxrow << " " << maxcol << std::flush;

    Eigen::MatrixX2d max_orbs(occupied_orbitals.rows(), 2);
    max_orbs << occupied_orbitals.col(maxrow), occupied_orbitals.col(maxcol);
    Eigen::MatrixX2d rotated_orbs = rotateorbitals(max_orbs, maxrow, maxcol);
    occupied_orbitals.col(maxrow) = rotated_orbs.col(0);
    occupied_orbitals.col(maxcol) = rotated_orbs.col(1);

    update_cost_function(maxrow, maxcol);

    // check for convergence
    max_cost = PM_cost_function_.maxCoeff(&maxrow, &maxcol);

    if (max_cost < convergence_limit_) not_converged = false;

    XTP_LOG(Log::info, log_)
        << "maximum of penalty function: " << max_cost << std::flush;

    iteration++;
  }
  XTP_LOG(Log::error, log_) << TimeStamp() << " Orbitals localized after "
                            << iteration + 1 << " iterations" << std::flush;
  orbitals.setPMLocalizedOrbital(occupied_orbitals);
}

// Function to rotate the 2 maximum orbitals (s and t)
Eigen::MatrixX2d PMLocalization::rotateorbitals(const Eigen::MatrixX2d &maxorbs,
                                                const Index s, const Index t) {
  const double gamma =
      0.25 * asin(B(s, t) / sqrt((A(s, t) * A(s, t)) + (B(s, t) * B(s, t))));
  Eigen::MatrixX2d rotatedorbitals(maxorbs.rows(), 2);
  rotatedorbitals.col(0) =
      (std::cos(gamma) * maxorbs.col(0)) + (std::sin(gamma) * maxorbs.col(1));
  rotatedorbitals.col(1) = -1 * (std::sin(gamma) * maxorbs.col(0)) +
                           (std::cos(gamma) * maxorbs.col(1));
  XTP_LOG(Log::info, log_) << "Sine of the rotation angle = " << std::sin(gamma)
                           << std::flush;
  return rotatedorbitals;
}

Eigen::VectorXd PMLocalization::pop_per_atom(Eigen::VectorXd orbital) {

  Eigen::RowVectorXd MullikenPop_orb_per_basis =
      (orbital.asDiagonal() * overlap_ * orbital.asDiagonal()).colwise().sum();
  Index start = 0;

  Eigen::VectorXd per_atom = Eigen::VectorXd::Zero(Index(numfuncpatom_.size()));
  for (Index atom_id = 0; atom_id < Index(numfuncpatom_.size()); atom_id++) {
    per_atom(atom_id) =
        MullikenPop_orb_per_basis.segment(start, numfuncpatom_[atom_id]).sum();
    start += numfuncpatom_[atom_id];
  }

  return per_atom;
}

// Determine PM cost function based on Mulliken populations
void PMLocalization::initial_cost_function() {

  PM_cost_function_ =
      Eigen::MatrixXd::Zero(occupied_orbitals.cols(), occupied_orbitals.cols());
  // Variable names A and B are used directly as described in the paper above
  A = Eigen::MatrixXd::Zero(occupied_orbitals.cols(), occupied_orbitals.cols());
  B = Eigen::MatrixXd::Zero(occupied_orbitals.cols(), occupied_orbitals.cols());

  numfuncpatom_ = aobasis.getFuncPerAtom();

  // get the s-s elements first ("diagonal in orbital")
  MullikenPop_orb_per_atom_ = Eigen::MatrixXd::Zero(
      occupied_orbitals.cols(), Index(numfuncpatom_.size()));
#pragma omp parallel for
  for (Index s = 0; s < occupied_orbitals.cols(); s++) {
    MullikenPop_orb_per_atom_.row(s) = pop_per_atom(occupied_orbitals.col(s));
  }

// now we only need to calculate the off-diagonals explicitly
#pragma omp parallel for
  for (Index s = 0; s < occupied_orbitals.cols(); s++) {
    Eigen::MatrixXd s_overlap =
        occupied_orbitals.col(s).asDiagonal() * overlap_;

    for (Index t = s + 1; t < occupied_orbitals.cols(); t++) {

      Eigen::Vector2d temp = offdiag_cost_elements(s_overlap, s, t);

      /*
        Eigen::MatrixXd splitwiseMullikenPop_orb_SandT =
            s_overlap * occupied_orbitals.col(t).asDiagonal();
        Eigen::RowVectorXd MullikenPop_orb_SandT_per_basis =
            0.5 * (splitwiseMullikenPop_orb_SandT.colwise().sum() +
                   splitwiseMullikenPop_orb_SandT.rowwise().sum().transpose());

        Index start =
            0;  // This helps to sum only over the basis functions on an atom
        double Ast = 0;
        double Bst = 0;
        for (Index atom_id = 0; atom_id < Index(numfuncpatom_.size());
             atom_id++) {
          double MullikenPop_orb_SandT_per_atom =
              MullikenPop_orb_SandT_per_basis
                  .segment(start, numfuncpatom_[atom_id])
                  .sum();

          Ast += MullikenPop_orb_SandT_per_atom * MullikenPop_orb_SandT_per_atom
        - 0.25 * ((MullikenPop_orb_per_atom_(s, atom_id) -
                          MullikenPop_orb_per_atom_(t, atom_id)) *
                         (MullikenPop_orb_per_atom_(s, atom_id) -
                          MullikenPop_orb_per_atom_(t, atom_id)));

          Bst += MullikenPop_orb_SandT_per_atom *
                 (MullikenPop_orb_per_atom_(s, atom_id) -
                  MullikenPop_orb_per_atom_(t, atom_id));
          start += numfuncpatom_[atom_id];
        }*/
      A(s, t) = temp(0);  // Ast;
      B(s, t) = temp(1);  // Bst;
      PM_cost_function_(s, t) =
          A(s, t) + sqrt((A(s, t) * A(s, t)) + (B(s, t) * B(s, t)));
    }
  }
  return;
}

Eigen::Vector2d PMLocalization::offdiag_cost_elements(
    const Eigen::MatrixXd &s_overlap, Index s, Index t) {

  Eigen::MatrixXd splitwiseMullikenPop_orb_SandT =
      s_overlap * occupied_orbitals.col(t).asDiagonal();
  Eigen::RowVectorXd MullikenPop_orb_SandT_per_basis =
      0.5 * (splitwiseMullikenPop_orb_SandT.colwise().sum() +
             splitwiseMullikenPop_orb_SandT.rowwise().sum().transpose());

  Index start =
      0;  // This helps to sum only over the basis functions on an atom
  double Ast = 0;
  double Bst = 0;

  for (Index atom_id = 0; atom_id < Index(numfuncpatom_.size()); atom_id++) {
    double MullikenPop_orb_SandT_per_atom =
        MullikenPop_orb_SandT_per_basis.segment(start, numfuncpatom_[atom_id])
            .sum();

    Ast += MullikenPop_orb_SandT_per_atom * MullikenPop_orb_SandT_per_atom -
           0.25 * ((MullikenPop_orb_per_atom_(s, atom_id) -
                    MullikenPop_orb_per_atom_(t, atom_id)) *
                   (MullikenPop_orb_per_atom_(s, atom_id) -
                    MullikenPop_orb_per_atom_(t, atom_id)));

    Bst += MullikenPop_orb_SandT_per_atom *
           (MullikenPop_orb_per_atom_(s, atom_id) -
            MullikenPop_orb_per_atom_(t, atom_id));
    start += numfuncpatom_[atom_id];
  }

  Eigen::Vector2d out(Ast, Bst);

  return out;
}

// Update PM cost function based on Mulliken populations after rotations
void PMLocalization::update_cost_function(Index orb1, Index orb2) {

  // update the get the s-s elements for orb1 and orb2
#pragma omp parallel for
  for (Index s = 0; s < occupied_orbitals.cols(); s++) {
    if (s == orb1 || s == orb2) {

      MullikenPop_orb_per_atom_.row(s) = pop_per_atom(occupied_orbitals.col(s));
    }
  }

// now we only need to calculate the off-diagonals explicitly for all
// pairs involving orb1 or orb2
#pragma omp parallel for
  for (Index s = 0; s < occupied_orbitals.cols(); s++) {
    Eigen::MatrixXd s_overlap =
        occupied_orbitals.col(s).asDiagonal() * overlap_;

    for (Index t = s + 1; t < occupied_orbitals.cols(); t++) {

      // we do this only if any of s or t matches orb1 or orb2
      if (s == orb1 || s == orb2 || t == orb1 || t == orb2) {

        Eigen::Vector2d temp = offdiag_cost_elements(s_overlap, s, t);
        A(s, t) = temp(0);
        B(s, t) = temp(1);
        PM_cost_function_(s, t) =
            A(s, t) + sqrt((A(s, t) * A(s, t)) + (B(s, t) * B(s, t)));
      }
    }
  }
  return;
}

}  // namespace xtp
}  // namespace votca