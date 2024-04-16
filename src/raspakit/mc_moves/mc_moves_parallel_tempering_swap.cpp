module;

#ifdef USE_LEGACY_HEADERS
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <complex>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <optional>
#include <span>
#include <tuple>
#include <vector>
#endif

module mc_moves_parallel_tempering_swap;

#ifndef USE_LEGACY_HEADERS
import <complex>;
import <vector>;
import <array>;
import <tuple>;
import <optional>;
import <span>;
import <optional>;
import <tuple>;
import <algorithm>;
import <numeric>;
import <chrono>;
import <cmath>;
import <iostream>;
import <iomanip>;
#endif

import component;
import atom;
import double3;
import double3x3;
import simd_quatd;
import simulationbox;
import cbmc;
import randomnumbers;
import system;
import energy_factor;
import energy_status;
import energy_status_inter;
import running_energy;
import property_lambda_probability_histogram;
import property_widom;
import averages;
import forcefield;
import move_statistics;
import mc_moves_probabilities_system;
import interactions_framework_molecule;
import interactions_intermolecular;
import interactions_ewald;
import interactions_external_field;

std::optional<std::pair<RunningEnergy, RunningEnergy>> MC_Moves::ParallelTemperingSwap(RandomNumber &random,
                                                                                       System &systemA, System &systemB)
{
  std::chrono::system_clock::time_point time_begin, time_end;

  systemA.mc_moves_statistics.ParallelTemperingSwap.counts += 1;
  systemA.mc_moves_statistics.ParallelTemperingSwap.totalCounts += 1;
  systemB.mc_moves_statistics.ParallelTemperingSwap.counts += 1;
  systemB.mc_moves_statistics.ParallelTemperingSwap.totalCounts += 1;

  // compute energy of boxA, positionsA in Hamiltonian of B
  // vice versa
  // add constructed to stats
  //

  std::cout << "swapping this bitch\n";
  RunningEnergy systemAHamiltonianB;
  time_begin = std::chrono::system_clock::now();
  Interactions::computeInterMolecularEnergy(systemB.forceField, systemA.simulationBox, systemA.atomPositions,
                                            systemAHamiltonianB);
  time_end = std::chrono::system_clock::now();
  systemA.mc_moves_cputime.ParallelTemperingSwapEnergy += (time_end - time_begin);
  systemA.mc_moves_statistics.ParallelTemperingSwap.constructed += 1;
  systemA.mc_moves_statistics.ParallelTemperingSwap.totalConstructed += 1;

  RunningEnergy systemBHamiltonianA;
  time_begin = std::chrono::system_clock::now();
  Interactions::computeInterMolecularEnergy(systemA.forceField, systemB.simulationBox, systemB.atomPositions,
                                            systemBHamiltonianA);
  time_end = std::chrono::system_clock::now();
  systemB.mc_moves_cputime.ParallelTemperingSwapEnergy += (time_end - time_begin);
  systemB.mc_moves_statistics.ParallelTemperingSwap.constructed += 1;
  systemB.mc_moves_statistics.ParallelTemperingSwap.totalConstructed += 1;

  double deltaU = -systemA.beta * (systemBHamiltonianA.total() - systemA.runningEnergies.total()) -
                  systemB.beta * (systemAHamiltonianB.total() - systemB.runningEnergies.total());

  // apply acceptance/rejection rule
  if (random.uniform() < std::exp(deltaU))
  {
    std::cout << "swapped this bitch\n";
    systemA.mc_moves_statistics.ParallelTemperingSwap.accepted += 1;
    systemA.mc_moves_statistics.ParallelTemperingSwap.totalAccepted += 1;

    systemB.mc_moves_statistics.ParallelTemperingSwap.accepted += 1;
    systemB.mc_moves_statistics.ParallelTemperingSwap.totalAccepted += 1;

    System tmp = systemA;
    double tmp_temp = systemB.temperature;
    double tmp_beta = systemB.beta;

    systemA = systemB;
    systemA.temperature = tmp.temperature;
    systemA.beta = tmp.beta;

    systemB = tmp;
    systemB.temperature = tmp_temp;
    systemB.beta = tmp_beta;

    return std::make_pair(systemAHamiltonianB, systemBHamiltonianA);
  }

  return std::nullopt;
}
