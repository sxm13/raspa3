module;

#ifdef USE_LEGACY_HEADERS
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <optional>
#include <span>
#include <tuple>
#include <type_traits>
#include <vector>
#endif

module cbmc_interactions;

#ifndef USE_LEGACY_HEADERS
import <iomanip>;
import <iostream>;
import <algorithm>;
import <vector>;
import <span>;
import <optional>;
import <cmath>;
import <tuple>;
import <type_traits>;
#endif

import atom;
import molecule;
import energy_factor;
import energy_status;
import energy_status_intra;
import energy_status_inter;
import running_energy;
import framework;
import component;
import double3;
import double3x3;
import forcefield;
import simulationbox;
import units;
import cbmc_interactions_external_field;
import cbmc_interactions_framework_molecule;
import cbmc_interactions_intermolecular;

bool CBMC::insideBlockedPockets(const std::vector<Framework> &frameworkComponents, const Component &component, std::span<const Atom> molecule_atoms)
{
  for(const Framework &framework: frameworkComponents)
  {
    for(size_t i = 0; i != component.blockingPockets.size(); ++i)
    {
      double radius_squared = component.blockingPockets[i].w * component.blockingPockets[i].w;
      double3 pos = framework.simulationBox.cell * double3(component.blockingPockets[i].x, component.blockingPockets[i].y, component.blockingPockets[i].z);
      for(const Atom &atom : molecule_atoms)
      {
        double3 dr = atom.position - pos;

        // compute the periodic boundary conditions with the single unit cell of the framework
        dr = framework.simulationBox.applyPeriodicBoundaryConditions(dr);

        double lambda = atom.scalingVDW;
        if(dr.length_squared() < lambda * radius_squared)
        {
          return true;
        }
      }
    }
  }
  return false;
}

inline std::pair<EnergyStatus, double3x3> pair_acc(const std::pair<EnergyStatus, double3x3> &lhs,
                                                   const std::pair<EnergyStatus, double3x3> &rhs)
{
  return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}

[[nodiscard]] const std::vector<std::pair<Atom, RunningEnergy>> CBMC::computeExternalNonOverlappingEnergies(
    const std::vector<Framework> &frameworkComponents, const Component &component,
    bool hasExternalField, const ForceField &forceField, const SimulationBox &simulationBox,
    std::span<const Atom> frameworkAtoms, std::span<const Atom> moleculeAtoms, [[maybe_unused]] double cutOffVDW,
    [[maybe_unused]] double cutOffCoulomb, [[maybe_unused]] std::vector<Atom> &trialPositions) noexcept
{
  std::vector<std::pair<Atom, RunningEnergy>> energies{};

  // loop over the trial-positions and compute the external energy of each trial position '{it, 1}'
  for (auto it = trialPositions.begin(); it != trialPositions.end(); ++it)
  {
    if(CBMC::insideBlockedPockets(frameworkComponents, component, {it, 1}))
    {
      continue;
    }

    std::optional<RunningEnergy> externalFieldEnergy = CBMC::computeExternalFieldEnergy(
        hasExternalField, forceField, simulationBox, cutOffVDW, cutOffCoulomb, {it, 1});

    // skip trial-positions that have an overlap in external-field energy
    if (!externalFieldEnergy.has_value()) continue;

    std::optional<RunningEnergy> interEnergy =
        CBMC::computeInterMolecularEnergy(forceField, simulationBox, moleculeAtoms, cutOffVDW, cutOffCoulomb, {it, 1});

    // skip trial-positions that have an overlap in inter-molecular energy
    if (!interEnergy.has_value()) continue;

    std::optional<RunningEnergy> frameworkEnergy = CBMC::computeFrameworkMoleculeEnergy(
        forceField, simulationBox, frameworkAtoms, cutOffVDW, cutOffCoulomb, {it, 1});

    // skip trial-positions that have an overlap in framework-molecule energy
    if (!frameworkEnergy.has_value()) continue;

    // store position and energy
    energies.push_back(
        std::make_pair(*it, externalFieldEnergy.value() + interEnergy.value() + frameworkEnergy.value()));
  }
  return energies;
}

const std::vector<std::pair<std::vector<Atom>, RunningEnergy>> CBMC::computeExternalNonOverlappingEnergies(
    const std::vector<Framework> &frameworkComponents, const Component &component,
    bool hasExternalField, const ForceField &forceField, const SimulationBox &simulationBox,
    std::span<const Atom> frameworkAtoms, std::span<const Atom> moleculeAtoms, [[maybe_unused]] double cutOffVDW,
    [[maybe_unused]] double cutOffCoulomb, std::vector<std::vector<Atom>> &trialPositionSets,
    std::make_signed_t<std::size_t> skip) noexcept
{
  std::vector<std::pair<std::vector<Atom>, RunningEnergy>> energies{};

  for (std::vector<Atom> trialPositionSet : trialPositionSets)
  {
    if(CBMC::insideBlockedPockets(frameworkComponents, component, trialPositionSet))
    {
      continue;
    }

    std::optional<RunningEnergy> eternalFieldEnergy = CBMC::computeExternalFieldEnergy(
        hasExternalField, forceField, simulationBox, cutOffVDW, cutOffCoulomb, trialPositionSet);
    if (!eternalFieldEnergy.has_value()) continue;

    std::optional<RunningEnergy> interEnergy = CBMC::computeInterMolecularEnergy(
        forceField, simulationBox, moleculeAtoms, cutOffVDW, cutOffCoulomb, trialPositionSet, skip);
    if (!interEnergy.has_value()) continue;

    std::optional<RunningEnergy> frameworkEnergy = CBMC::computeFrameworkMoleculeEnergy(
        forceField, simulationBox, frameworkAtoms, cutOffVDW, cutOffCoulomb, trialPositionSet, skip);
    if (!frameworkEnergy.has_value()) continue;

    energies.push_back(
        std::make_pair(trialPositionSet, eternalFieldEnergy.value() + interEnergy.value() + frameworkEnergy.value()));
  }
  return energies;
}

const std::vector<std::tuple<Molecule, std::vector<Atom>, RunningEnergy>> CBMC::computeExternalNonOverlappingEnergies(
    const std::vector<Framework> &frameworkComponents, const Component &component,
    bool hasExternalField, const ForceField &forceField, const SimulationBox &simulationBox,
    std::span<const Atom> frameworkAtoms, std::span<const Atom> moleculeAtoms, [[maybe_unused]] double cutOffVDW,
    [[maybe_unused]] double cutOffCoulomb, std::vector<std::pair<Molecule, std::vector<Atom>>> &trialPositionSets,
    std::make_signed_t<std::size_t> skip) noexcept
{
  std::vector<std::tuple<Molecule, std::vector<Atom>, RunningEnergy>> energies{};

  for (auto &[molecule, trialPositionSet] : trialPositionSets)
  {
    if(CBMC::insideBlockedPockets(frameworkComponents, component, trialPositionSet))
    {
      continue;
    }

    std::optional<RunningEnergy> eternalFieldEnergy = CBMC::computeExternalFieldEnergy(
        hasExternalField, forceField, simulationBox, cutOffVDW, cutOffCoulomb, trialPositionSet);
    if (!eternalFieldEnergy.has_value()) continue;

    std::optional<RunningEnergy> interEnergy = CBMC::computeInterMolecularEnergy(
        forceField, simulationBox, moleculeAtoms, cutOffVDW, cutOffCoulomb, trialPositionSet, skip);
    if (!interEnergy.has_value()) continue;

    std::optional<RunningEnergy> frameworkEnergy = CBMC::computeFrameworkMoleculeEnergy(
        forceField, simulationBox, frameworkAtoms, cutOffVDW, cutOffCoulomb, trialPositionSet, skip);
    if (!frameworkEnergy.has_value()) continue;

    energies.push_back(std::make_tuple(molecule, trialPositionSet,
                                       eternalFieldEnergy.value() + interEnergy.value() + frameworkEnergy.value()));
  }
  return energies;
}

const std::optional<RunningEnergy> CBMC::computeExternalNonOverlappingEnergyDualCutOff(
    const std::vector<Framework> &frameworkComponents, const Component &component,
    bool hasExternalField, const ForceField &forceField, const SimulationBox &simulationBox,
    std::span<const Atom> frameworkAtoms, std::span<const Atom> moleculeAtoms, double cutOffVDW, double cutOffCoulomb,
    std::vector<Atom> &trialPositionSet) noexcept
{
  std::pair<std::vector<Atom>, RunningEnergy> energies;

  if(CBMC::insideBlockedPockets(frameworkComponents, component, trialPositionSet))
  {
    return std::nullopt;
  }

  std::optional<RunningEnergy> externalFieldEnergy = CBMC::computeExternalFieldEnergy(
      hasExternalField, forceField, simulationBox, cutOffVDW, cutOffCoulomb, trialPositionSet);
  if (!externalFieldEnergy.has_value()) return std::nullopt;

  std::optional<RunningEnergy> interEnergy = CBMC::computeInterMolecularEnergy(
      forceField, simulationBox, moleculeAtoms, cutOffVDW, cutOffCoulomb, trialPositionSet, -1);
  if (!interEnergy.has_value()) return std::nullopt;

  std::optional<RunningEnergy> frameworkEnergy = CBMC::computeFrameworkMoleculeEnergy(
      forceField, simulationBox, frameworkAtoms, cutOffVDW, cutOffCoulomb, trialPositionSet, -1);
  if (!frameworkEnergy.has_value()) return std::nullopt;

  return externalFieldEnergy.value() + interEnergy.value() + frameworkEnergy.value();
}
