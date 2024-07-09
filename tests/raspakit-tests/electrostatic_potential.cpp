#include <gtest/gtest.h>

#include <cstddef>
#include <algorithm>
#include <vector>
#include <span>
#include <complex>

import int3;
import double3;
import double3x3;

import units;
import atom;
import pseudo_atom;
import vdwparameters;
import forcefield;
import framework;
import component;
import system;
import simulationbox;
import energy_factor;
import force_factor;
import running_energy;
import interactions_intermolecular;
import interactions_framework_molecule;
import interactions_ewald;
import energy_status;

TEST(electrostatic_potential, Test_2_CO2_in_ITQ_29_2x2x2_inter)
{
  ForceField forceField = ForceField(
    { PseudoAtom("Si",    28.0855,   2.05,   0.0, 14, false),
      PseudoAtom("O",     15.999,   -1.025,  0.0,  8, false),
      PseudoAtom("CH4",   16.04246,  0.0,    0.0,  6, false),
      PseudoAtom("C_co2", 12.0,      0.6512, 0.2,  6, false),
      PseudoAtom("O_co2", 15.9994,  -0.3256, 0.1,  8, false),
    },
    { VDWParameters(22.0, 2.30),
      VDWParameters(53.0, 3.3),
      VDWParameters(158.5, 3.72),
      VDWParameters(29.933, 2.745),
      VDWParameters(85.671, 3.017)
    },
    ForceField::MixingRule::Lorentz_Berthelot,
    11.8,
    true,
    false);
  Framework f = Framework(0, forceField, "ITQ-29", SimulationBox(11.8671, 11.8671, 11.8671),
    517,
    { // double3 position, double charge, double lambda, uint32_t moleculeId, uint16_t type, uint8_t componentId, uint8_t groupId
      Atom(double3(0.3683, 0.1847, 0),       2.05,  1.0, 0, 0, 0, 0),
      Atom(double3(0.5,    0.2179, 0),      -1.025, 1.0, 0, 1, 0, 0),
      Atom(double3(0.2939, 0.2939, 0),      -1.025, 1.0, 0, 1, 0, 0),
      Atom(double3(0.3429, 0.1098, 0.1098), -1.025, 1.0, 0, 1, 0, 0)
    },
    int3(2, 2, 2));
  Component c = Component(1, forceField, "CO2", 304.1282, 7377300.0, 0.22394,
    { // double3 position, double charge, double lambda, uint32_t moleculeId, uint16_t type, uint8_t componentId, uint8_t groupId
      Atom(double3(0.0, 0.0,  1.149), -0.3256, 1.0, 0, 4, 0, 0),
      Atom(double3(0.0, 0.0,  0.0),    0.6512, 1.0, 0, 3, 0, 0),
      Atom(double3(0.0, 0.0, -1.149), -0.3256, 1.0, 0, 4, 0, 0)
    }, 5, 21);

  System system = System(0, std::nullopt, 300.0, 1e4, forceField, { f }, { c }, { 2 }, 5);
  system.forceField.EwaldAlpha = 0.25;
  system.forceField.numberOfWaveVectors = int3(8, 8, 8);

  std::span<Atom> spanOfMoleculeAtoms = system.spanOfMoleculeAtoms();
  std::span<const Atom> frameworkAtomPositions = system.spanOfFrameworkAtoms();
  std::span<double> moleculeElectricPotential = system.spanOfMoleculeElectricPotential();
  std::span<double3> electricField = system.spanOfMoleculeElectricField();
  spanOfMoleculeAtoms[0].position = double3(5.93355, 7.93355, 5.93355 + 1.149);
  spanOfMoleculeAtoms[1].position = double3(5.93355, 7.93355, 5.93355 + 0.0);
  spanOfMoleculeAtoms[2].position = double3(5.93355, 7.93355, 5.93355 - 1.149);
  spanOfMoleculeAtoms[3].position = double3(5.93355, 3.93355, 5.93355 + 1.149);
  spanOfMoleculeAtoms[4].position = double3(5.93355, 3.93355, 5.93355 + 0.0);
  spanOfMoleculeAtoms[5].position = double3(5.93355, 3.93355, 5.93355 - 1.149);

  RunningEnergy energy = Interactions::computeInterMolecularEnergy(system.forceField, system.simulationBox, spanOfMoleculeAtoms) +
                         Interactions::computeFrameworkMoleculeEnergy(system.forceField,system.simulationBox, frameworkAtomPositions, spanOfMoleculeAtoms);

  EXPECT_NEAR(energy.frameworkMoleculeVDW * Units::EnergyToKelvin, -1932.15586114, 1e-6);
  EXPECT_NEAR(energy.frameworkMoleculeCharge * Units::EnergyToKelvin, 554.41444763, 1e-6);
  EXPECT_NEAR(energy.moleculeMoleculeVDW * Units::EnergyToKelvin, -242.36960932, 1e-6);
  EXPECT_NEAR(energy.moleculeMoleculeCharge * Units::EnergyToKelvin, 162.41877650, 1e-6);

  system.computeTotalElectricPotential();

  system.computeTotalElectricField();

  EXPECT_NEAR(electricField[0].x, 0.0, 1e-6);
  EXPECT_NEAR(electricField[0].y, 1426.420166315602, 1e-6);
  EXPECT_NEAR(electricField[0].z, 757.263103162725, 1e-6);
  EXPECT_NEAR(electricField[1].x, 0.0, 1e-6);
  EXPECT_NEAR(electricField[1].y, 1673.170684602238, 1e-6);
  EXPECT_NEAR(electricField[1].z, 0.0, 1e-6);
  EXPECT_NEAR(electricField[2].x, 0.0, 1e-6);
  EXPECT_NEAR(electricField[2].y, 1426.420166315595, 1e-6);
  EXPECT_NEAR(electricField[2].z, -757.263103162717, 1e-6);

  EXPECT_NEAR(electricField[3].x, 0.0, 1e-6);
  EXPECT_NEAR(electricField[3].y, -1426.420166315596, 1e-6);
  EXPECT_NEAR(electricField[3].z, 757.263103162714, 1e-6);
  EXPECT_NEAR(electricField[4].x, 0.0, 1e-6);
  EXPECT_NEAR(electricField[4].y, -1673.170684602236, 1e-6);
  EXPECT_NEAR(electricField[4].z, 0.0, 1e-6);
  EXPECT_NEAR(electricField[5].x, 0.0, 1e-6);
  EXPECT_NEAR(electricField[5].y, -1426.420166315593, 1e-6);
  EXPECT_NEAR(electricField[5].z, -757.263103162715, 1e-6);


  // U = \sum_i q_i \phi_i
  double potentialEnergy{};
  double polarizationEnergy{};
  for(size_t i = 0; i < 6; ++i)
  {
    potentialEnergy += spanOfMoleculeAtoms[i].charge * moleculeElectricPotential[i];

    size_t type = spanOfMoleculeAtoms[i].type;
    double polarizability = system.forceField.pseudoAtoms[type].polarizability / Units::CoulombicConversionFactor;
    polarizationEnergy -= 0.5 * polarizability * double3::dot(electricField[i], electricField[i]);
  }

  EXPECT_NEAR(polarizationEnergy * Units::EnergyToKelvin, -9.362444, 1e-6);

  EXPECT_NEAR(energy.moleculeMoleculeCharge * Units::EnergyToKelvin, 162.41877650, 1e-6);
  EXPECT_NEAR(energy.frameworkMoleculeCharge * Units::EnergyToKelvin, 554.41444763, 1e-6);

  EXPECT_NEAR(potentialEnergy * Units::EnergyToKelvin, 554.41444763 + 162.41877650, 1e-6);

  // create copy
  std::vector<Atom> atomPositions = std::vector<Atom>(spanOfMoleculeAtoms.begin(), spanOfMoleculeAtoms.end());

  double delta = 1e-5;
  double tolerance = 1e-4;
  double3 gradient;
  for (size_t i = 0; i < atomPositions.size(); ++i)
  {
    double x1, x2, y1, y2, z1, z2;

    // finite difference x
    atomPositions[i].position.x = spanOfMoleculeAtoms[i].position.x + 0.5 * delta;
    std::fill(moleculeElectricPotential.begin(), moleculeElectricPotential.end(), 0.0);
    Interactions::computeFrameworkMoleculeElectricPotential(
      system.forceField, system.simulationBox, moleculeElectricPotential, frameworkAtomPositions, atomPositions);
    Interactions::computeInterMolecularElectricPotential(
      system.forceField, system.simulationBox, moleculeElectricPotential, atomPositions);
    x2 = moleculeElectricPotential[i];

    atomPositions[i].position.x = spanOfMoleculeAtoms[i].position.x - 0.5 * delta;
    std::fill(moleculeElectricPotential.begin(), moleculeElectricPotential.end(), 0.0);
    Interactions::computeFrameworkMoleculeElectricPotential(
      system.forceField, system.simulationBox, moleculeElectricPotential, frameworkAtomPositions, atomPositions);
    Interactions::computeInterMolecularElectricPotential(
      system.forceField, system.simulationBox, moleculeElectricPotential, atomPositions);
    x1 = moleculeElectricPotential[i];
    atomPositions[i].position.x = spanOfMoleculeAtoms[i].position.x;

    // finite difference y
    atomPositions[i].position.y = spanOfMoleculeAtoms[i].position.y + 0.5 * delta;
    std::fill(moleculeElectricPotential.begin(), moleculeElectricPotential.end(), 0.0);
    Interactions::computeFrameworkMoleculeElectricPotential(
      system.forceField, system.simulationBox, moleculeElectricPotential, frameworkAtomPositions, atomPositions);
    Interactions::computeInterMolecularElectricPotential(
      system.forceField, system.simulationBox, moleculeElectricPotential, atomPositions);
    y2 = moleculeElectricPotential[i];

    atomPositions[i].position.y = spanOfMoleculeAtoms[i].position.y - 0.5 * delta;
    std::fill(moleculeElectricPotential.begin(), moleculeElectricPotential.end(), 0.0);
    Interactions::computeFrameworkMoleculeElectricPotential(
      system.forceField, system.simulationBox, moleculeElectricPotential, frameworkAtomPositions, atomPositions);
    Interactions::computeInterMolecularElectricPotential(
      system.forceField, system.simulationBox, moleculeElectricPotential, atomPositions);
    y1 = moleculeElectricPotential[i];
    atomPositions[i].position.y = spanOfMoleculeAtoms[i].position.y;

    // finite difference z
    atomPositions[i].position.z = spanOfMoleculeAtoms[i].position.z + 0.5 * delta;
    std::fill(moleculeElectricPotential.begin(), moleculeElectricPotential.end(), 0.0);
    Interactions::computeFrameworkMoleculeElectricPotential(
      system.forceField, system.simulationBox, moleculeElectricPotential, frameworkAtomPositions, atomPositions);
    Interactions::computeInterMolecularElectricPotential(
      system.forceField, system.simulationBox, moleculeElectricPotential, atomPositions);
    z2 = moleculeElectricPotential[i];

    atomPositions[i].position.z = spanOfMoleculeAtoms[i].position.z - 0.5 * delta;
    std::fill(moleculeElectricPotential.begin(), moleculeElectricPotential.end(), 0.0);
    Interactions::computeFrameworkMoleculeElectricPotential(
      system.forceField, system.simulationBox, moleculeElectricPotential, frameworkAtomPositions, atomPositions);
    Interactions::computeInterMolecularElectricPotential(
      system.forceField, system.simulationBox, moleculeElectricPotential, atomPositions);
    z1 = moleculeElectricPotential[i];
    atomPositions[i].position.z = spanOfMoleculeAtoms[i].position.z;

    gradient.x = -(x2 -x1) / delta;
    gradient.y = -(y2 -y1) / delta;
    gradient.z = -(z2 -z1) / delta;

    EXPECT_NEAR(electricField[i].x, gradient.x, tolerance);
    EXPECT_NEAR(electricField[i].y, gradient.y, tolerance);
    EXPECT_NEAR(electricField[i].z, gradient.z, tolerance);
  }
}

