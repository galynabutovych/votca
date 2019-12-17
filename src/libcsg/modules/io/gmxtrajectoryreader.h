/*
 * Copyright 2009-2019 The VOTCA Development Team (http://www.votca.org)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
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

#ifndef _VOTCA_CSG_GMXTRAJECTORYREADER_H
#define _VOTCA_CSG_GMXTRAJECTORYREADER_H

#ifndef HAVE_NO_CONFIG
#include <votca_config.h>
#endif

#include <string>
#include <votca/csg/trajectoryreader.h>
#include <votca/tools/unitconverter.h>

#include <gromacs/fileio/oenv.h>
#include <gromacs/fileio/trxio.h>
#include <gromacs/trajectory/trajectoryframe.h>
// this one is needed because of bool is defined in one of the headers included
// by gmx
#undef bool

namespace votca {
namespace csg {

/**
    \brief class for reading gromacs trajectory files

    This class provides the TrajectoryReader interface and encapsulates the
   trajectory reading function of gromacs

*/

class GMXTrajectoryReader : public TrajectoryReader {
 public:
  GMXTrajectoryReader() = default;

  const tools::DistanceUnit distance_unit = tools::DistanceUnit::nanometers;
  const tools::MassUnit mass_unit = tools::MassUnit::atomic_mass_units;
  const tools::TimeUnit time_unit = tools::TimeUnit::picoseconds;
  const tools::ChargeUnit charge_unit = tools::ChargeUnit::e;
  const tools::MolarEnergyUnit energy_unit =
      tools::EnergyUnit::kilojoules_per_mole;
  const tools::VelocityUnit velocity_unit =
      tools::VelocityUnit::nanometers_per_picosecond;
  const tools::MolarForceUnit force_unit =
      tools::ForceUnit::kilojoules_per_mole_nanometer;

  /// open a trejectory file
  bool Open(const std::string &file) override;
  /// read in the first frame
  bool FirstFrame(Topology &top) override;
  /// read in the next frame
  bool NextFrame(Topology &top) override;

  void Close() override;

 private:
  std::string _filename;

  // gmx status used in read_first_frame and _read_next_frame;
  t_trxstatus *_gmx_status;
  /// gmx frame
  t_trxframe _gmx_frame;
};

}  // namespace csg
}  // namespace votca

#endif /* _VOTCA_CSG_GMXTRAJECTORYREADER_H */
