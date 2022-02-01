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

  if (method_ == "jacobi-sweeps") {
    XTP_LOG(Log::error, log_)
        << TimeStamp() << " Using Jacobi-Sweeps" << std::flush;
    computePML_JS(orbitals);
  } else if (method_ == "unitary-optimizer") {
    XTP_LOG(Log::error, log_)
        << TimeStamp() << " Using Unitary Optimizer" << std::flush;
    computePML_UT(orbitals);
  }
}

void PMLocalization::computePML_UT(Orbitals &orbitals) {

  occupied_orbitals = orbitals.MOs().eigenvectors().leftCols(
      orbitals.getNumberOfAlphaElectrons());

  // initialize a unitary matrix (as Idenity matrix for now)
  const Index n_occs = orbitals.getNumberOfAlphaElectrons();
  Eigen::MatrixXd W = Eigen::MatrixXd::Identity(n_occs, n_occs);

  // prepare Mulliken charges
  // get overlap matrix
  aobasis_ = orbitals.getDftBasis();
  AOOverlap overlap;
  overlap.Fill(aobasis_);
  overlap_ = overlap.Matrix();
  numfuncpatom_ = aobasis_.getFuncPerAtom();
  Index numatoms = Index(numfuncpatom_.size());
  Index numfunctions = overlap_.cols();
  Index start = 0;

  // could be a bit memory expensive
  std::vector<Eigen::MatrixXd> Sat_all;

  for (Index iat = 0; iat < numatoms; iat++) {

    Eigen::MatrixXd Sat = Eigen::MatrixXd::Zero(numfunctions, numfunctions);

    // copy all columns of overlap belingin to this atom to local Sat
    Sat.middleCols(start, numfuncpatom_[iat]) =
        overlap_.middleCols(start, numfuncpatom_[iat]);
    // symmetrize
    Sat = (Sat + Sat.transpose()) / 2.0;
    // multiply with orbitals
    Sat = occupied_orbitals.transpose() * Sat * occupied_orbitals;

    Sat_all.push_back(Sat);
    start += numfuncpatom_[iat];
  }

  // calculate value of cost function
  double Dinv = 0.0;
  double p = 2.0; //standard PM
  for (Index iat = 0; iat < numatoms; iat++) {
    Eigen::MatrixXd qw = Sat_all[iat] * W;
    for (Index i = 0; i < W.cols(); i++) {
      double Qa = W.col(i).transpose() * qw.col(i);
      Dinv += std::pow(Qa, p);
    }
  }

XTP_LOG(Log::error, log_)
        << TimeStamp() << " Calculated cost function" << std::flush;

  // calculate derivative of cost function wrt unitary matrix
  Eigen::MatrixXd Jderiv = Eigen::MatrixXd::Zero(n_occs, n_occs);
  for (Index iat = 0; iat < numatoms; iat++) {
    Eigen::MatrixXd qw = Sat_all[iat] * W;
    for (Index i = 0; i < W.cols(); i++) {
      double qwp = W.col(i).transpose() * qw.col(i);
      double t = p * std::pow(qwp, p - 1);
      for (Index j = 0; j < W.cols(); j++) {

        Jderiv(j, i) += t * qw(j, i);
      }
    }
  }

  XTP_LOG(Log::error, log_)
        << TimeStamp() << " Calculated derivative" << std::flush;

  // calculate Riemannian derivative
  Eigen::MatrixXd G = Jderiv * W.transpose() - W * Jderiv.transpose();

  // calculate search direction using SDSA for now
  Eigen::MatrixXd H = G;

  Index orderW = 4; // for PM
  std::complex<double> ima(0.0,1.0);
  Eigen::ComplexEigenSolver<Eigen::MatrixXcd> es(-ima*H);
XTP_LOG(Log::error, log_)
        << TimeStamp() << " Eigensystem" << std::flush;

  Eigen::VectorXcd Hval = es.eigenvalues();
  double wmax = Hval.cwiseAbs().maxCoeff();
  double Tmu = 2.0*tools::conv::Pi/(orderW*wmax);
XTP_LOG(Log::error, log_)
        << TimeStamp() << " Max step" << Tmu << std::flush;



  // line optimization
  // update unitary matrix
}

void PMLocalization::computePML_JS(Orbitals &orbitals) {
  occupied_orbitals = orbitals.MOs().eigenvectors().leftCols(
      orbitals.getNumberOfAlphaElectrons());
  aobasis_ = orbitals.getDftBasis();
  AOOverlap overlap;
  overlap.Fill(aobasis_);
  overlap_ = overlap.Matrix();

  XTP_LOG(Log::error, log_) << std::flush;
  XTP_LOG(Log::error, log_)
      << TimeStamp() << " Starting localization of orbitals" << std::flush;

  // determine initial penalty_function
  initial_penalty();

  Index iteration = 1;
  Index maxrow, maxcol;
  double max_penalty = PM_penalty_.maxCoeff(&maxrow, &maxcol);

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

    update_penalty(maxrow, maxcol);

    // check for convergence
    max_penalty = PM_penalty_.maxCoeff(&maxrow, &maxcol);

    if (max_penalty < convergence_limit_) not_converged = false;

    XTP_LOG(Log::info, log_)
        << "maximum of penalty function: " << max_penalty << std::flush;

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
      0.25 *
      asin(B_(s, t) / sqrt((A_(s, t) * A_(s, t)) + (B_(s, t) * B_(s, t))));
  Eigen::MatrixX2d rotatedorbitals(maxorbs.rows(), 2);
  rotatedorbitals.col(0) =
      (std::cos(gamma) * maxorbs.col(0)) + (std::sin(gamma) * maxorbs.col(1));
  rotatedorbitals.col(1) = -1 * (std::sin(gamma) * maxorbs.col(0)) +
                           (std::cos(gamma) * maxorbs.col(1));
  XTP_LOG(Log::info, log_) << "Sine of the rotation angle = " << std::sin(gamma)
                           << std::flush;
  return rotatedorbitals;
}

Eigen::VectorXd PMLocalization::pop_per_atom(const Eigen::VectorXd &orbital) {

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
void PMLocalization::initial_penalty() {

  PM_penalty_ =
      Eigen::MatrixXd::Zero(occupied_orbitals.cols(), occupied_orbitals.cols());
  // Variable names A and B are used directly as described in the paper above
  A_ =
      Eigen::MatrixXd::Zero(occupied_orbitals.cols(), occupied_orbitals.cols());
  B_ =
      Eigen::MatrixXd::Zero(occupied_orbitals.cols(), occupied_orbitals.cols());

  numfuncpatom_ = aobasis_.getFuncPerAtom();

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

      Eigen::Vector2d temp = offdiag_penalty_elements(s_overlap, s, t);

      A_(s, t) = temp(0);
      B_(s, t) = temp(1);
      PM_penalty_(s, t) =
          A_(s, t) + sqrt((A_(s, t) * A_(s, t)) + (B_(s, t) * B_(s, t)));
    }
  }
  return;
}

Eigen::Vector2d PMLocalization::offdiag_penalty_elements(
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
void PMLocalization::update_penalty(Index orb1, Index orb2) {

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

        Eigen::Vector2d temp = offdiag_penalty_elements(s_overlap, s, t);
        A_(s, t) = temp(0);
        B_(s, t) = temp(1);
        PM_penalty_(s, t) =
            A_(s, t) + sqrt((A_(s, t) * A_(s, t)) + (B_(s, t) * B_(s, t)));
      }
    }
  }
  return;
}

}  // namespace xtp
}  // namespace votca