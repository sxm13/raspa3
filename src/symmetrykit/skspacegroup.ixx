module;

#ifdef USE_LEGACY_HEADERS
#include <array>
#include <optional>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>
#endif


export module skspacegroup;

#ifndef USE_LEGACY_HEADERS
import <vector>;
import <tuple>;
import <array>;
import <string>;
import <optional>;
import <unordered_set>;
#endif

import int3;
import int3x3;
import double3;
import double3x3;

import skdefinitions;
import skseitzmatrix;
import skpointgroup;
import skpointsymmetryset;
import sksymmetryoperationset;
import skspacegroupsetting;
import skrotationalchangeofbasis;
import skspacegroupdatabase;
import sksymmetrycell;

export class SKSpaceGroup
{
 public:
  struct FoundSpaceGroupInfo
  {
    size_t HallNumber;
    double3 origin;
    SKSymmetryCell cell;
    SKRotationalChangeOfBasis changeOfBasis;
    double3x3 transformationMatrix;
    double3x3 rotationMatrix;
    std::vector<std::tuple<double3, size_t, double>> atoms;
    std::vector<std::tuple<double3, size_t, double>> asymmetricAtoms;
  };

  struct FoundNiggliCellInfo
  {
    size_t HallNumber;
    SKSymmetryCell cell;
    std::vector<std::tuple<double3, size_t, double>> atoms;
  };

  struct FoundPrimitiveCellInfo
  {
    SKSymmetryCell cell;
    std::vector<std::tuple<double3, size_t, double>> atoms;
  };

  SKSpaceGroup(size_t HallNumber);
  std::vector<double3> listOfSymmetricPositions(double3 pos);
  const SKSpaceGroupSetting& spaceGroupSetting() const { return _spaceGroupSetting; }

  static std::vector<std::string> latticeTranslationStrings(size_t HallNumber);
  static std::string inversionCenterString(size_t HallNumber);
  static std::optional<size_t> HallNumberFromHMString(std::string inputString);
  static std::optional<size_t> HallNumberFromSpaceGroupNumber(size_t);
  static std::optional<size_t> HallNumber(std::string inputString);
  static std::optional<FoundPrimitiveCellInfo> SKFindPrimitive(double3x3 unitCell,
                                                               std::vector<std::tuple<double3, size_t, double>> atoms,
                                                               bool allowPartialOccupancies, double symmetryPrecision);
  static std::optional<FoundNiggliCellInfo> findNiggliCell(double3x3 unitCell,
                                                           std::vector<std::tuple<double3, size_t, double>> atoms,
                                                           bool allowPartialOccupancies, double symmetryPrecision);
  static std::optional<SKPointGroup> findPointGroup(double3x3 unitCell,
                                                    std::vector<std::tuple<double3, size_t, double>> atoms,
                                                    bool allowPartialOccupancies, double symmetryPrecision);
  static std::optional<FoundSpaceGroupInfo> findSpaceGroup(double3x3 unitCell,
                                                           std::vector<std::tuple<double3, size_t, double>> atoms,
                                                           bool allowPartialOccupancies, double symmetryPrecision);

  static SKSymmetryOperationSet findSpaceGroupSymmetry(double3x3 unitCell,
                                                       std::vector<std::tuple<double3, size_t, double>> reducedAtoms,
                                                       std::vector<std::tuple<double3, size_t, double>> atoms,
                                                       SKPointSymmetrySet latticeSymmetries,
                                                       bool allowPartialOccupancies, double symmetryPrecision);
  static std::optional<std::pair<double3, SKRotationalChangeOfBasis>> matchSpaceGroup(
      size_t HallNumber, double3x3 lattice, Centring entering, std::vector<SKSeitzMatrix> seitzMatrices,
      double symmetryPrecision);
  static std::optional<double3> getOriginShift(size_t HallNumber, Centring centering,
                                               SKRotationalChangeOfBasis changeOfBasis,
                                               std::vector<SKSeitzMatrix> seitzMatrices, double symmetryPrecision);

 private:
  SKSpaceGroupSetting _spaceGroupSetting = SKSpaceGroupDataBase::spaceGroupData[1];

  static bool matchSpacegroup(std::string spaceSearchGroupString, std::string storedSpaceGroupString);
};
