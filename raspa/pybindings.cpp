#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <exception>

#pragma clang diagnostic pop

#ifdef USE_LEGACY_HEADERS
#include <span>
#include <vector>
#endif

#ifndef USE_LEGACY_HEADERS
import <span>;
import <vector>;
#endif

import int3;
import double3;
import double3x3;
import atom;
import simulationbox;
import pseudo_atom;
import vdwparameters;
import forcefield;
import running_energy;
import mc_moves_probabilities_particles;
import mc_moves_probabilities_system;
import framework;
import component;
import system;
import randomnumbers;
import monte_carlo;
import input_reader;

PYBIND11_MODULE(raspalib, m)
{
  pybind11::class_<int3>(m, "int3").def(pybind11::init<int32_t, int32_t, int32_t>(), pybind11::arg("x"),
                                        pybind11::arg("y"), pybind11::arg("z"));

  pybind11::class_<double3>(m, "double3")
      .def(pybind11::init<double, double, double>(), pybind11::arg("x"), pybind11::arg("y"), pybind11::arg("z"))
      .def_readwrite("x", &double3::x)
      .def_readwrite("y", &double3::y)
      .def_readwrite("z", &double3::z);

  pybind11::class_<RandomNumber>(m, "random").def(pybind11::init<size_t>(), pybind11::arg("seed"));

  pybind11::class_<RunningEnergy>(m, "RunningEnergy")
      .def(pybind11::init<>())
      .def_readwrite("moleculeMoleculeVDW", &RunningEnergy::moleculeMoleculeVDW)
      .def_readwrite("frameworkMoleculeVDW", &RunningEnergy::frameworkMoleculeVDW);

  pybind11::class_<Atom>(m, "Atom")
      .def(pybind11::init<double3, double, double, uint32_t, uint16_t, uint8_t, uint8_t>(), pybind11::arg("position"),
           pybind11::arg("charge"), pybind11::arg("lambda"), pybind11::arg("moleculeId"), pybind11::arg("type"),
           pybind11::arg("componentId"), pybind11::arg("groupId"))
      .def_readwrite("position", &Atom::position)
      .def("__repr__", &Atom::repr);

  pybind11::class_<SimulationBox> simulationBox(m, "SimulationBox");
  pybind11::enum_<SimulationBox::Type>(simulationBox, "Type")
      .value("Rectangular", SimulationBox::Type::Rectangular)
      .value("Triclinic", SimulationBox::Type::Triclinic)
      .export_values();
  simulationBox
      .def(pybind11::init<double, double, double>(), pybind11::arg("a"), pybind11::arg("b"), pybind11::arg("c"))
      .def(pybind11::init<double, double, double, double, double, double>(), pybind11::arg("a"),
           pybind11::arg("b"), pybind11::arg("c"), pybind11::arg("alpha"), pybind11::arg("beta"), pybind11::arg("gamma"))
      .def(pybind11::init<double, double, double, double, double, double, SimulationBox::Type>(), pybind11::arg("a"),
           pybind11::arg("b"), pybind11::arg("c"), pybind11::arg("alpha"), pybind11::arg("beta"),
           pybind11::arg("gamma"), pybind11::arg("type"))
      .def(pybind11::init<double3x3, SimulationBox::Type>())
      .def_readwrite("type", &SimulationBox::type)
      .def_readonly("lengthA", &SimulationBox::lengthA)
      .def_readonly("lengthB", &SimulationBox::lengthB)
      .def_readonly("lengthC", &SimulationBox::lengthC);

  pybind11::class_<VDWParameters>(m, "VDWParameters")
      .def(pybind11::init<double, double>(), pybind11::arg("epsilon"), pybind11::arg("sigma"));

  pybind11::class_<PseudoAtom>(m, "PseudoAtom")
      .def(pybind11::init<std::string, double, double, double, size_t, bool, std::string>(), pybind11::arg("name"),
           pybind11::arg("mass"), pybind11::arg("charge"), pybind11::arg("polarizability"),
           pybind11::arg("atomicNumber"), pybind11::arg("printToPDB"), pybind11::arg("source"));

  pybind11::class_<ForceField> forceField(m, "ForceField");
  forceField
      .def(pybind11::init<std::vector<PseudoAtom>, std::vector<VDWParameters>, ForceField::MixingRule, double, bool,
                          bool, bool>(),
           pybind11::arg("pseudoAtoms"), pybind11::arg("parameters"), pybind11::arg("mixingRule"),
           pybind11::arg("cutOff"), pybind11::arg("shifted"), pybind11::arg("tailCorrections"),
           pybind11::arg("useCharge"))
      .def(pybind11::init<std::string>(), pybind11::arg("fileName"))
      .def("__repr__", &ForceField::repr)
      .def_readonly("pseudoAtoms", &ForceField::pseudoAtoms)
      .def_readonly("vdwParameters", &ForceField::data)
      .def_readwrite("useCharge", &ForceField::useCharge);

  pybind11::enum_<ForceField::MixingRule>(forceField, "MixingRule")
      .value("Lorentz_Berthelot", ForceField::MixingRule::Lorentz_Berthelot)
      .export_values();

  pybind11::class_<Framework>(m, "Framework")
      .def(pybind11::init<size_t, const ForceField &, std::string, SimulationBox, size_t, std::vector<Atom>, int3>(),
           pybind11::arg("frameworkId"), pybind11::arg("forceField"), pybind11::arg("componentName"),
           pybind11::arg("simulationBox"), pybind11::arg("spaceGroupHallNumber"), pybind11::arg("definedAtoms"),
           pybind11::arg("numberOfUnitCells"))
      .def(pybind11::init<size_t, const ForceField &, const std::string &, std::optional<const std::string>, int3, bool>(),
           pybind11::arg("frameworkId"), pybind11::arg("forceField"), pybind11::arg("componentName"),
           pybind11::arg("fileName"), pybind11::arg("numberOfUnitCells"), pybind11::arg("useChargesFromCIFFile"))
      .def_readonly("name", &Framework::name)
      .def("__repr__", &Framework::repr);

  pybind11::class_<MCMoveProbabilitiesParticles>(m, "MCMoveProbabilitiesParticles")
      .def(pybind11::init<double, double, double, double, double, double, double, double, double, double, double,
                          double, double, double, double, double, double, double, double>(),
           pybind11::arg("probabilityTranslationMove") = 0.0, pybind11::arg("probabilityRandomTranslationMove") = 0.0,
           pybind11::arg("probabilityRotationMove") = 0.0, pybind11::arg("probabilityRandomRotationMove") = 0.0,
           pybind11::arg("probabilityVolumeMove") = 0.0, pybind11::arg("probabilityReinsertionMove_CBMC") = 0.0,
           pybind11::arg("probabilityIdentityChangeMove_CBMC") = 0.0, pybind11::arg("probabilitySwapMove") = 0.0,
           pybind11::arg("probabilitySwapMove_CBMC") = 0.0, pybind11::arg("probabilitySwapMove_CFCMC") = 0.0,
           pybind11::arg("probabilitySwapMove_CFCMC_CBMC") = 0.0, pybind11::arg("probabilityGibbsVolumeMove") = 0.0,
           pybind11::arg("probabilityGibbsSwapMove_CBMC") = 0.0, pybind11::arg("probabilityGibbsSwapMove_CFCMC") = 0.0,
           pybind11::arg("probabilityGibbsSwapMove_CFCMC_CBMC") = 0.0, pybind11::arg("probabilityWidomMove") = 0.0,
           pybind11::arg("probabilityWidomMove_CFCMC") = 0.0, pybind11::arg("probabilityWidomMove_CFCMC_CBMC") = 0.0,
           pybind11::arg("probabilityParallelTemperingSwap") = 0.0)
      .def_readwrite("probabilityTranslationMove", &MCMoveProbabilitiesParticles::probabilityTranslationMove)
      .def_readwrite("probabilityRandomTranslationMove",
                     &MCMoveProbabilitiesParticles::probabilityRandomTranslationMove)
      .def_readwrite("probabilityRotationMove", &MCMoveProbabilitiesParticles::probabilityRotationMove)
      .def_readwrite("probabilityRandomRotationMove", &MCMoveProbabilitiesParticles::probabilityRandomRotationMove)
      .def_readwrite("probabilityVolumeMove", &MCMoveProbabilitiesParticles::probabilityVolumeMove)
      .def_readwrite("probabilityReinsertionMove_CBMC", &MCMoveProbabilitiesParticles::probabilityReinsertionMove_CBMC)
      .def_readwrite("probabilityIdentityChangeMove_CBMC",
                     &MCMoveProbabilitiesParticles::probabilityIdentityChangeMove_CBMC)
      .def_readwrite("probabilitySwapMove", &MCMoveProbabilitiesParticles::probabilitySwapMove)
      .def_readwrite("probabilitySwapMove_CBMC", &MCMoveProbabilitiesParticles::probabilitySwapMove_CBMC)
      .def_readwrite("probabilitySwapMove_CFCMC", &MCMoveProbabilitiesParticles::probabilitySwapMove_CFCMC)
      .def_readwrite("probabilitySwapMove_CFCMC_CBMC", &MCMoveProbabilitiesParticles::probabilitySwapMove_CFCMC_CBMC)
      .def_readwrite("probabilityGibbsVolumeMove", &MCMoveProbabilitiesParticles::probabilityGibbsVolumeMove)
      .def_readwrite("probabilityGibbsSwapMove_CBMC", &MCMoveProbabilitiesParticles::probabilityGibbsSwapMove_CBMC)
      .def_readwrite("probabilityGibbsSwapMove_CFCMC", &MCMoveProbabilitiesParticles::probabilityGibbsSwapMove_CFCMC)
      .def_readwrite("probabilityGibbsSwapMove_CFCMC_CBMC",
                     &MCMoveProbabilitiesParticles::probabilityGibbsSwapMove_CFCMC_CBMC)
      .def_readwrite("probabilityWidomMove", &MCMoveProbabilitiesParticles::probabilityWidomMove)
      .def_readwrite("probabilityWidomMove_CFCMC", &MCMoveProbabilitiesParticles::probabilityWidomMove_CFCMC)
      .def_readwrite("probabilityParallelTemperingSwap",
                     &MCMoveProbabilitiesParticles::probabilityParallelTemperingSwap);

  pybind11::class_<MCMoveProbabilitiesSystem>(m, "MCMoveProbabilitiesSystem")
      .def(pybind11::init<double, double, double>(), pybind11::arg("probabilityVolumeMove"),
           pybind11::arg("probabilityGibbsVolumeMove"), pybind11::arg("probabilityParallelTemperingSwap"))
      .def_readwrite("probabilityVolumeMove", &MCMoveProbabilitiesSystem::probabilityVolumeMove)
      .def_readwrite("probabilityGibbsVolumeMove", &MCMoveProbabilitiesSystem::probabilityGibbsVolumeMove)
      .def_readwrite("probabilityParallelTemperingSwap", &MCMoveProbabilitiesSystem::probabilityParallelTemperingSwap);

  pybind11::class_<Component> component(m, "Component");
  component
      .def(pybind11::init<size_t, const ForceField &, std::string, double, double, double, std::vector<Atom>, size_t,
                          size_t, const MCMoveProbabilitiesParticles &, std::optional<double>, bool>(),
           pybind11::arg("componentId"), pybind11::arg("forceField"), pybind11::arg("componentName"),
           pybind11::arg("criticalTemperature"), pybind11::arg("criticalPressure"), pybind11::arg("acentricFactor"),
           pybind11::arg("definedAtoms"), pybind11::arg("numberOfBlocks"), pybind11::arg("numberOfLambdaBins"),
           pybind11::arg("particleProbabilities"), pybind11::arg("fugacityCoefficient") = std::nullopt,
           pybind11::arg("thermodynamicIntegration") = false)
      .def(pybind11::init<Component::Type, size_t, const ForceField &, std::string &, std::string, size_t, size_t,
                          const MCMoveProbabilitiesParticles &, std::optional<double>, bool>(),
           pybind11::arg("type"), pybind11::arg("componentId"), pybind11::arg("forceField"),
           pybind11::arg("componentName"), pybind11::arg("fileName"), pybind11::arg("numberOfBlocks"),
           pybind11::arg("numberOfLambdaBins"), pybind11::arg("particleProbabilities"),
           pybind11::arg("fugacityCoefficient") = std::nullopt, pybind11::arg("thermodynamicIntegration") = false)
      .def_readonly("name", &Component::name)
      .def("__repr__", &Component::repr);

  pybind11::enum_<Component::Type>(component, "Type")
      .value("Adsorbate", Component::Type::Adsorbate)
      .value("Cation", Component::Type::Cation)
      .export_values();

  pybind11::class_<System>(m, "System")
      .def(pybind11::init<size_t, ForceField, std::optional<SimulationBox>, double, std::optional<double>, double,
                          std::vector<Framework>, std::vector<Component>, std::vector<size_t>, size_t,
                          MCMoveProbabilitiesSystem, std::optional<size_t>>(),
           pybind11::arg("systemId"), pybind11::arg("forceField"), pybind11::arg("simulationBox"), pybind11::arg("temperature"),
           pybind11::arg("pressure"), pybind11::arg("heliumVoidFraction"), pybind11::arg("frameworkComponents"),
           pybind11::arg("components"), pybind11::arg("initialNumberOfMolecules"), pybind11::arg("numberOfBlocks"),
           pybind11::arg("systemProbabilities"), pybind11::arg("sampleMoviesEvery"))
      .def("computeTotalEnergies", &System::computeTotalEnergies)
      .def_readwrite("atomPositions", &System::atomPositions)
      .def("__repr__", &System::repr);

  pybind11::class_<InputReader> inputReader(m, "InputReader");
  inputReader.def(pybind11::init<const std::string>(), pybind11::arg("fileName"))
      .def_readonly("numberOfBlocks", &InputReader::numberOfBlocks)
      .def_readonly("numberOfCycles", &InputReader::numberOfCycles)
      .def_readonly("numberOfInitializationCycles", &InputReader::numberOfInitializationCycles)
      .def_readonly("numberOfEquilibrationCycles", &InputReader::numberOfEquilibrationCycles)
      .def_readonly("printEvery", &InputReader::printEvery)
      .def_readonly("writeBinaryRestartEvery", &InputReader::writeBinaryRestartEvery)
      .def_readonly("rescaleWangLandauEvery", &InputReader::rescaleWangLandauEvery)
      .def_readonly("optimizeMCMovesEvery", &InputReader::optimizeMCMovesEvery)
      .def_readonly("writeEvery", &InputReader::writeEvery)
      .def_readonly("forceField", &InputReader::forceField)
      .def_readonly("systems", &InputReader::systems);

  pybind11::enum_<InputReader::SimulationType>(inputReader, "SimulationType")
      .value("MonteCarlo", InputReader::SimulationType::MonteCarlo)
      .value("MonteCarloTransitionMatrix", InputReader::SimulationType::MonteCarloTransitionMatrix)
      .value("MolecularDynamics", InputReader::SimulationType::MolecularDynamics)
      .value("Minimization", InputReader::SimulationType::Minimization)
      .value("Test", InputReader::SimulationType::Test)
      .value("Breakthrough", InputReader::SimulationType::Breakthrough)
      .value("MixturePrediction", InputReader::SimulationType::MixturePrediction)
      .value("Fitting", InputReader::SimulationType::Fitting)
      .value("ParallelTempering", InputReader::SimulationType::ParallelTempering)
      .export_values();

  pybind11::class_<MonteCarlo> mc(m, "MonteCarlo");
    mc.def(pybind11::init<size_t, size_t, size_t, size_t, size_t, size_t, size_t, std::vector<System> &, RandomNumber &,
                          size_t>(),
           pybind11::arg("numberOfCycles"), pybind11::arg("numberOfInitializationCycles"),
           pybind11::arg("numberOfEquilibrationCycles"), pybind11::arg("printEvery"),
           pybind11::arg("writeBinaryRestartEvery"), pybind11::arg("rescaleWangLandauEvery"),
           pybind11::arg("optimizeMCMovesEvery"), pybind11::arg("systems"), pybind11::arg("randomSeed"),
           pybind11::arg("numberOfBlocks"))
      .def(pybind11::init<InputReader &>(), pybind11::arg("inputReader"))
      .def("run", &MonteCarlo::run)
      .def("initialize", &MonteCarlo::initialize)
      .def("equilibrate", &MonteCarlo::equilibrate)
      .def("production", &MonteCarlo::production)
      .def("cycle", &MonteCarlo::cycle)
      .def("__repr__", &MonteCarlo::repr)
      .def_readwrite("simulationStage", &MonteCarlo::simulationStage);

  pybind11::enum_<MonteCarlo::SimulationStage>(mc, "SimulationStage")
      .value("Uninitialized", MonteCarlo::SimulationStage::Uninitialized)
      .value("Initialization", MonteCarlo::SimulationStage::Initialization)
      .value("Equilibration", MonteCarlo::SimulationStage::Equilibration)
      .value("Production", MonteCarlo::SimulationStage::Production)
      .export_values();
}
