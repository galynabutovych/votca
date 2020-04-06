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
 *
 */

#include "gencube.h"
#include <boost/format.hpp>
#include <stdio.h>
#include <votca/tools/constants.h>
#include <votca/tools/elements.h>
#include <votca/xtp/aobasis.h>
#include <votca/xtp/cubefile_writer.h>
#include <votca/xtp/orbitals.h>

namespace votca {
namespace xtp {

using namespace std;

void GenCube::Initialize(tools::Property& opt) {

  // get pre-defined default options from VOTCASHARE/xtp/xml/apdft.xml
  LoadDefaults("xtp");
  // update options with user specified input
  UpdateWithUserOptions(opt);

  _orbfile =
      _options.ifExistsReturnElseThrowRuntimeError<std::string>(".input");
  _output_file =
      _options.ifExistsReturnElseThrowRuntimeError<std::string>(".output");

  // padding
  _padding = _options.ifExistsReturnElseThrowRuntimeError<double>(".padding");

  // steps
  _steps.y() = _options.get(".ysteps").as<Index>();
  _steps.x() = _options.get(".xsteps").as<Index>();
  _steps.z() = _options.get(".zsteps").as<Index>();

  std::string statestring =
      _options.ifExistsReturnElseThrowRuntimeError<std::string>(".state");
  _state.FromString(statestring);
  _dostateonly =
      _options.ifExistsReturnElseReturnDefault<bool>(".diff2gs", false);

  _mode = _options.get(".mode").as<std::string>();
  if (_mode == "subtract") {
    _infile1 = _options.get(".infile1").as<std::string>();
    _infile2 = _options.get(".infile2").as<std::string>();
  }
}

void GenCube::calculateCube() {

  XTP_LOG(Log::error, _log)
      << "Reading serialized QM data from " << _orbfile << flush;

  Orbitals orbitals;
  orbitals.ReadFromCpt(_orbfile);

  CubeFile_Writer writer(_steps, _padding, _log);
  XTP_LOG(Log::error, _log) << "Created cube grid" << flush;
  writer.WriteFile(_output_file, orbitals, _state, _dostateonly);
  XTP_LOG(Log::error, _log) << "Wrote cube data to " << _output_file << flush;
  return;
}

void GenCube::subtractCubes() {

  // open infiles for reading
  ifstream in1;
  XTP_LOG(Log::error, _log) << " Reading first cube from " << _infile1 << flush;
  in1.open(_infile1, ios::in);
  ifstream in2;
  XTP_LOG(Log::error, _log)
      << " Reading second cube from " << _infile2 << flush;
  in2.open(_infile2, ios::in);
  string s;

  std::ofstream out(_output_file);

  // first two lines of header are garbage
  getline(in1, s);
  out << s << "\n";
  getline(in1, s);
  out << s << " substraction\n";
  getline(in2, s);
  getline(in2, s);

  // read rest from header
  Index natoms;
  double xstart;
  double ystart;
  double zstart;
  // first line
  in1 >> natoms;
  bool do_amplitude = false;
  if (natoms < 0) {
    do_amplitude = true;
  }
  in1 >> xstart;
  in1 >> ystart;
  in1 >> zstart;
  // check from second file
  Index tempint;
  double tempdouble;
  in2 >> tempint;
  if (tempint != natoms) {
    throw std::runtime_error("Atom numbers do not match");
  }
  in2 >> tempdouble;
  if (tempdouble != xstart) {
    throw std::runtime_error("Xstart does not match");
  }
  in2 >> tempdouble;
  if (tempdouble != ystart) {
    throw std::runtime_error("Ystart does not match");
  }
  in2 >> tempdouble;
  if (tempdouble != zstart) {
    throw std::runtime_error("Zstart does not match");
  }

  out << boost::format("%1$lu %2$f %3$f %4$f \n") % natoms % xstart % ystart %
             zstart;

  // grid information from first cube
  double xincr;
  double yincr;
  double zincr;
  Index xsteps;
  Index ysteps;
  Index zsteps;
  in1 >> xsteps;
  in1 >> xincr;
  in1 >> tempdouble;
  in1 >> tempdouble;
  in1 >> ysteps;
  in1 >> tempdouble;
  in1 >> yincr;
  in1 >> tempdouble;
  in1 >> zsteps;
  in1 >> tempdouble;
  in1 >> tempdouble;
  in1 >> zincr;

  // check second cube
  in2 >> tempint;
  if (tempint != xsteps) {
    throw std::runtime_error("xsteps does not match");
  }
  in2 >> tempdouble;
  if (tempdouble != xincr) {
    throw std::runtime_error("xincr does not match");
  }
  in2 >> tempdouble;
  in2 >> tempdouble;
  in2 >> tempint;
  if (tempint != ysteps) {
    throw std::runtime_error("ysteps does not match");
  }
  in2 >> tempdouble;
  in2 >> tempdouble;
  if (tempdouble != yincr) {
    throw std::runtime_error("yincr does not match");
  }
  in2 >> tempdouble;
  in2 >> tempint;
  if (tempint != zsteps) {
    throw std::runtime_error("zsteps does not match");
  }
  in2 >> tempdouble;
  in2 >> tempdouble;
  in2 >> tempdouble;
  if (tempdouble != zincr) {
    throw std::runtime_error("zincr does not match");
  }

  out << boost::format("%1$d %2$f 0.0 0.0 \n") % xsteps % xincr;
  out << boost::format("%1$d 0.0 %2$f 0.0 \n") % ysteps % yincr;
  out << boost::format("%1$d 0.0 0.0 %2$f \n") % zsteps % zincr;

  // atom information

  for (Index iatom = 0; iatom < std::abs(natoms); iatom++) {
    // get center coordinates in Bohr
    double x;
    double y;
    double z;
    Index atnum;
    double crg;

    // get from first cube
    in1 >> atnum;
    in1 >> crg;
    in1 >> x;
    in1 >> y;
    in1 >> z;

    // check second cube
    in2 >> tempint;
    if (tempint != atnum) {
      throw std::runtime_error("atnum does not match");
    }
    in2 >> tempdouble;
    if (tempdouble != crg) {
      throw std::runtime_error("crg does not match");
    }
    in2 >> tempdouble;
    if (tempdouble != x) {
      throw std::runtime_error("x does not match");
    }
    in2 >> tempdouble;
    if (tempdouble != y) {
      throw std::runtime_error("y does not match");
    }
    in2 >> tempdouble;
    if (tempdouble != z) {
      throw std::runtime_error("z does not match");
    }
    out << boost::format("%1$d %2$f %3$f %4$f %5$f\n") % atnum % crg % x % y %
               z;
  }

  if (do_amplitude) {
    Index ntotal;
    Index nis;
    in1 >> ntotal;
    in1 >> nis;

    in2 >> tempint;
    if (tempint != ntotal) {
      throw std::runtime_error("ntotal does not match");
    }
    in2 >> tempint;
    if (tempint != nis) {
      throw std::runtime_error("nis does not match");
    }
    out << boost::format("  1 %1$d \n") % nis;
  }
  // now read data
  double val1;
  double val2;
  for (Index ix = 0; ix < xsteps; ix++) {
    for (Index iy = 0; iy < ysteps; iy++) {
      Index Nrecord = 0;
      for (Index iz = 0; iz < zsteps; iz++) {
        Nrecord++;
        in1 >> val1;
        in2 >> val2;
        if (Nrecord == 6 || iz == zsteps - 1) {
          out << boost::format("%1$E \n") % (val1 - val2);
          Nrecord = 0;
        } else {
          out << boost::format("%1$E ") % (val1 - val2);
        }
      }
    }
  }

  out.close();
  XTP_LOG(Log::error, _log)
      << "Wrote subtracted cube data to " << _output_file << flush;
}

bool GenCube::Evaluate() {
  OPENMP::setMaxThreads(_nThreads);
  _log.setReportLevel(Log::current_level);
  _log.setMultithreading(true);

  _log.setCommonPreface("\n... ...");

  // calculate new cube
  if (_mode == "new") {
    calculateCube();
  } else if (_mode == "subtract") {
    subtractCubes();
  }

  return true;
}
}  // namespace xtp
}  // namespace votca
