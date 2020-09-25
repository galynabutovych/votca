#include "qmsandbox.h"
#include <votca/tools/eigenio_matrixmarket.h>
#include "votca/xtp/orbreorder.h"

namespace votca {
namespace xtp {

void QMSandbox::Initialize(const tools::Property& user_options) {

  tools::Property options =
      LoadDefaultsAndUpdateWithUserOptions("xtp", user_options);

  _job_name = options.ifExistsReturnElseReturnDefault<std::string>("job_name",
                                                                   _job_name);
}

bool QMSandbox::Evaluate() {
  _log.setReportLevel(Log::current_level);
  _log.setMultithreading(true);
  _log.setCommonPreface("\n... ...");

  OPENMP::setMaxThreads(_nThreads);

  QMMolecule atoms("", 0);
  atoms.LoadFromFile(_job_name + ".xyz");

  BasisSet bs;
  bs.Load("def2-svp");
  AOBasis basis;
  basis.Fill(bs, atoms);

  std::vector<libint2::Shell> shells = basis.GenerateLibintBasis();

  for (auto& shell : basis) {
    std::cout << shell << std::endl;
  }

  std::copy(std::begin(shells), std::end(shells),
            std::ostream_iterator<libint2::Shell>(std::cout, "\n"));

  AOOverlap aooverlap;
  AOCoulomb aocoulomb;
  AOKinetic aokinetic;
  AODipole aodipole;

  aooverlap.Fill(basis);
  aocoulomb.Fill(basis);
  aokinetic.Fill(basis);
  aodipole.Fill(basis);

  std::string mod = "_org_";

  XTP_LOG(Log::error, _log)
      << "Using threads: " << OPENMP::getMaxThreads() << std::endl;

  std::array<Index, 49> reorderList={
            0, //s
            0,1,-1, //p
            -2,-1,0,1,2, //d
            0,1,-1,2,-2,3,-3, //f 
            0,1,-1,2,-2,3,-3,4,-4, //g
            0,1,-1,2,-2,3,-3,4,-4,5,-5, //h
            0,1,-1,2,-2,3,-3,4,-4,5,-5,6,-6 //i
            };

  std::array<Index, 49> multipliers;
  multipliers.fill(1);

  OrbReorder reorder(reorderList, multipliers);

  Eigen::MatrixXd overlap = aooverlap.Matrix();

  reorder.reorderOperator(overlap, basis);


/*  XTP_LOG(Log::error, _log) << "\nOverlap  0 Integrals:\n";
  XTP_LOG(Log::error, _log) << overlap << std::endl;  */
  Eigen::MatrixXd overlap_org = votca::tools::EigenIO_MatrixMarket::ReadMatrix(
      _job_name + mod + "overlap.mm");

  votca::tools::EigenIO_MatrixMarket::WriteMatrix(
      _job_name + "_new_" + "overlap.mm", overlap);

  XTP_LOG(Log::error, _log) << "Overlap "
      << overlap.isApprox(overlap_org, 10e-5) <<  std::endl;

/*  XTP_LOG(Log::error, _log) << "\nCoulomb 0 Integrals:\n";
  XTP_LOG(Log::error, _log) << aocoulomb.Matrix() << std::endl;  */
  Eigen::MatrixXd coulomb_org = votca::tools::EigenIO_MatrixMarket::ReadMatrix(
      _job_name + mod + "coulomb.mm");

  XTP_LOG(Log::error, _log) << "Coulomb "
      << aocoulomb.Matrix().isApprox(coulomb_org, 10e-5) <<  std::endl;

/*  XTP_LOG(Log::error, _log) << "\nKinetic 0 Integrals:\n";
  XTP_LOG(Log::error, _log) << aokinetic.Matrix() << std::endl;  */
 Eigen::MatrixXd kinetic_org = votca::tools::EigenIO_MatrixMarket::ReadMatrix(
      _job_name + mod + "kinetic.mm");

  XTP_LOG(Log::error, _log) << "Kinetic "
      << aokinetic.Matrix().isApprox(kinetic_org, 10e-5) <<  std::endl;

/*  XTP_LOG(Log::error, _log) << "\nDipole 0 Integrals:\n";
  XTP_LOG(Log::error, _log) << aodipole.Matrix()[0] << std::endl; */
  Eigen::MatrixXd dip0 = votca::tools::EigenIO_MatrixMarket::ReadMatrix(
      _job_name + mod + "dip0.mm");

       XTP_LOG(Log::error, _log) << "dip0 "
      << aodipole.Matrix()[0].isApprox(dip0, 10e-5) <<  std::endl;

/*  XTP_LOG(Log::error, _log) << "\nDipole 1 Integrals:\n";
  XTP_LOG(Log::error, _log) << aodipole.Matrix()[1] << std::endl; */
  Eigen::MatrixXd dip1 = votca::tools::EigenIO_MatrixMarket::ReadMatrix(
      _job_name + mod + "dip1.mm");

       XTP_LOG(Log::error, _log) << "dip1 "
      << aodipole.Matrix()[1].isApprox(dip1, 10e-5) <<  std::endl;

/*  XTP_LOG(Log::error, _log) << "\nDipole 2 Integrals:\n";
  XTP_LOG(Log::error, _log) << aodipole.Matrix()[2] << std::endl; */
  Eigen::MatrixXd dip2 = votca::tools::EigenIO_MatrixMarket::ReadMatrix(
      _job_name + mod + "dip2.mm");

       XTP_LOG(Log::error, _log) << "dip2  "
      << aodipole.Matrix()[2].isApprox(dip2, 10e-5) <<  std::endl;

  
  return true;
}

}  // namespace xtp
}  // namespace votca