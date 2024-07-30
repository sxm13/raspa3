module;

#ifdef USE_LEGACY_HEADERS
#include <algorithm>
#include <array>
#include <cmath>
#include <complex>
#include <cstddef>
#include <exception>
#include <format>
#include <fstream>
#include <iostream>
#include <numbers>
#include <print>
#include <source_location>
#include <span>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#endif

module property_msd;

#ifndef USE_LEGACY_HEADERS
import <cstddef>;
import <string>;
import <iostream>;
import <fstream>;
import <sstream>;
import <tuple>;
import <vector>;
import <algorithm>;
import <complex>;
import <format>;
import <numbers>;
import <span>;
import <array>;
import <cmath>;
import <exception>;
import <source_location>;
import <print>;
#endif

import archive;
import double3;
import double4;
import atom;
import simulationbox;
import forcefield;
import component;
import averages;

void PropertyMeanSquaredDisplacement::addSample(size_t currentCycle, 
              const std::vector<Component> &components,
              const std::vector<size_t> &numberOfMoleculesPerComponent, 
              std::vector<Molecule> &moleculePositions)
{
  if (currentCycle % sampleEvery != 0uz) return;

  // determine current number of blocks
  numberOfBlocksMSD = 1;
  size_t k = countMSD / numberOfBlockElementsMSD;
  while(k != 0)
  {
    ++numberOfBlocksMSD;
    k /= numberOfBlockElementsMSD;
  }

  if(numberOfBlocksMSD > maxNumberOfBlocksMSD)
  {
    blockLengthMSD.resize(numberOfBlocksMSD);
    msdOrderNCount.resize(numberOfBlocksMSD, std::vector<std::vector<size_t>>(numberOfComponents, std::vector<size_t>(numberOfBlockElementsMSD, 0uz)));
    blockDataMSD.resize(numberOfBlocksMSD, std::vector<std::vector<double3>>(numberOfParticles, std::vector<double3>(numberOfBlockElementsMSD, double3())));
    msdOrderN.resize(numberOfBlocksMSD, std::vector<std::vector<double4>>(numberOfComponents, std::vector<double4>(numberOfBlockElementsMSD, double4())));
    maxNumberOfBlocksMSD = numberOfBlocksMSD;
  }

  for(size_t currentBlock = 0; currentBlock < numberOfBlocksMSD; currentBlock++)
  {
    // test for blocking operation: CountMSD is a multiple of NumberOfBlockElementsMSDOrderN^CurrentBlock
    if(countMSD % static_cast<size_t>(std::pow(numberOfBlockElementsMSD, currentBlock)) == 0)
    {
      // increase the current block-length
      blockLengthMSD[currentBlock]++;

      // limit length to numberOfBlockElementsMSD
      size_t currentBlocklength = std::min(blockLengthMSD[currentBlock], numberOfBlockElementsMSD);

      size_t index{0};
      for (size_t i = 0; i != components.size(); ++i)
      {
        for (size_t m = 0; m != numberOfMoleculesPerComponent[i]; ++m)
        {
          double3 value = moleculePositions[index].centerOfMassPosition;

          std::shift_right(begin(blockDataMSD[currentBlock][index]), end(blockDataMSD[currentBlock][index]), 1);
          blockDataMSD[currentBlock][index][0] = value;

          for(size_t j = 0; j < currentBlocklength; j++)
          {
            // msd for each component
            ++msdOrderNCount[currentBlock][i][j];

            double msd_x = (blockDataMSD[currentBlock][index][j].x - value.x) * (blockDataMSD[currentBlock][index][j].x - value.x);
            double msd_y = (blockDataMSD[currentBlock][index][j].y - value.y) * (blockDataMSD[currentBlock][index][j].y - value.y);
            double msd_z = (blockDataMSD[currentBlock][index][j].z - value.z) * (blockDataMSD[currentBlock][index][j].z - value.z);

            msdOrderN[currentBlock][i][j].x += msd_x;
            msdOrderN[currentBlock][i][j].y += msd_y;
            msdOrderN[currentBlock][i][j].z += msd_z;
            msdOrderN[currentBlock][i][j].w += msd_x + msd_y + msd_z;
          }

          ++index;
        }
      }
    }
  }

  ++countMSD;
}


void PropertyMeanSquaredDisplacement::writeOutput(size_t systemId, 
                                  const std::vector<Component> &components, double deltaT, size_t currentCycle)
{
  if (currentCycle % writeEvery != 0uz) return;

  std::filesystem::create_directory("msd");

  for (size_t i = 0; i < components.size(); ++i)
  {
    std::ofstream stream_msd_output(std::format("msd/msd_{}.s{}.data", components[i].name, systemId));

    stream_msd_output << std::format("# msd, number of counts: {}\n", countMSD);
    stream_msd_output << "# column 1: time [ps]\n";
    stream_msd_output << "# column 2: msd xyz [A^2]\n";
    stream_msd_output << "# column 3: msd x [A^2]\n";
    stream_msd_output << "# column 4: msd y [A^2]\n";
    stream_msd_output << "# column 5: msd z [A^2]\n";
    stream_msd_output << "# column 6: number of samples [-]\n";

    for(size_t currentBlock = 0; currentBlock < numberOfBlocksMSD; ++currentBlock)
    {
      size_t currentBlocklength = std::min(blockLengthMSD[currentBlock], numberOfBlockElementsMSD);
      double dt = static_cast<double>(sampleEvery) * deltaT * std::pow(numberOfBlockElementsMSD, currentBlock);
      for(size_t j = 1; j < currentBlocklength; ++j)
      {
        if(msdOrderNCount[currentBlock][i][j] > 0)
        {
          double fac = 1.0 / static_cast<double>(msdOrderNCount[currentBlock][i][j]);

          stream_msd_output << std::format("{} {} {} {} {} (count: {})\n",
              static_cast<double>(j) * dt,
              fac * msdOrderN[currentBlock][i][j].w,
              fac * msdOrderN[currentBlock][i][j].x,
              fac * msdOrderN[currentBlock][i][j].y,
              fac * msdOrderN[currentBlock][i][j].z,
              msdOrderNCount[currentBlock][i][j]);
        }
      }
    }
  }
}

Archive<std::ofstream> &operator<<(Archive<std::ofstream> &archive, const PropertyMeanSquaredDisplacement &msd)
{
  archive << msd.versionNumber;

  archive << msd.numberOfBlocks;
  archive << msd.sampleEvery;
  archive << msd.writeEvery;
  archive << msd.countMSD;
  archive << msd.numberOfBlocksMSD;
  archive << msd.maxNumberOfBlocksMSD;
  archive << msd.numberOfBlockElementsMSD;
  archive << msd.blockLengthMSD;
  archive << msd.msdOrderNCount;
  archive << msd.blockDataMSD;
  archive << msd.msdOrderN;

  return archive;
}

Archive<std::ifstream> &operator>>(Archive<std::ifstream> &archive, PropertyMeanSquaredDisplacement &msd)
{
  uint64_t versionNumber;
  archive >> versionNumber;
  if (versionNumber > msd.versionNumber)
  {
    const std::source_location &location = std::source_location::current();
    throw std::runtime_error(
        std::format("Invalid version reading 'PropertyRadialDistributionFunction' at line {} in file {}\n",
                    location.line(), location.file_name()));
  }

  archive >> msd.numberOfBlocks;
  archive >> msd.sampleEvery;
  archive >> msd.writeEvery;
  archive >> msd.countMSD;
  archive >> msd.numberOfBlocksMSD;
  archive >> msd.maxNumberOfBlocksMSD;
  archive >> msd.numberOfBlockElementsMSD;
  archive >> msd.blockLengthMSD;
  archive >> msd.msdOrderNCount;
  archive >> msd.blockDataMSD;
  archive >> msd.msdOrderN;

  return archive;
}
