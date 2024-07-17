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
      .def(pybind11::init<double, double, double, SimulationBox::Type>(), pybind11::arg("a"), pybind11::arg("b"),
           pybind11::arg("c"), pybind11::arg("type") = SimulationBox::Type::Rectangular)
      .def(pybind11::init<double, double, double, double, double, double, SimulationBox::Type>(), pybind11::arg("a"),
           pybind11::arg("b"), pybind11::arg("c"), pybind11::arg("alpha"), pybind11::arg("beta"),
           pybind11::arg("gamma"), pybind11::arg("type") = SimulationBox::Type::Rectangular)
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
                          bool>(),
           pybind11::arg("pseudoAtoms"), pybind11::arg("parameters"), pybind11::arg("mixingRule"),
           pybind11::arg("cutOff"), pybind11::arg("shifted"), pybind11::arg("tailCorrections"))
      .def(pybind11::init<std::string>(), pybind11::arg("fileName"))
      .def("__repr__", &ForceField::repr)
      .def_readonly("pseudoAtoms", &ForceField::pseudoAtoms)
      .def_readonly("vdwParameters", &ForceField::data);
  m.def("readForceField", &ForceField::readForceField, pybind11::return_value_policy::reference);

  pybind11::enum_<ForceField::MixingRule>(forceField, "MixingRule")
      .value("Lorentz_Berthelot", ForceField::MixingRule::Lorentz_Berthelot)
      .export_values();

  pybind11::class_<Framework>(m, "Framework")
      .def(pybind11::init<size_t, const ForceField &, std::string, SimulationBox, size_t, std::vector<Atom>, int3>(),
           pybind11::arg("currentComponent"), pybind11::arg("forceField"), pybind11::arg("componentName"),
           pybind11::arg("simulationBox"), pybind11::arg("spaceGroupHallNumber"), pybind11::arg("definedAtoms"),
           pybind11::arg("numberOfUnitCells"))
      .def(pybind11::init<size_t, const ForceField &, const std::string &, std::optional<const std::string>, int3>(),
           pybind11::arg("currentComponent"), pybind11::arg("forceField"), pybind11::arg("componentName"),
           pybind11::arg("fileName"), pybind11::arg("numberOfUnitCells"))
      .def_readonly("name", &Framework::name)
      .def("__repr__", &Framework::repr);

  pybind11::class_<MCMoveProbabilitiesParticles>(m, "MCMoveProbabilitiesParticles")
      .def(pybind11::init<double, double>(), pybind11::arg("probabilityTranslationMove") = 0.0,
           pybind11::arg("probabilityRotationMove") = 0.0)
      .def_readwrite("probabilityTranslationMove", &MCMoveProbabilitiesParticles::probabilityTranslationMove)
      .def_readwrite("probabilityRotationMove", &MCMoveProbabilitiesParticles::probabilityRotationMove);

  pybind11::class_<MCMoveProbabilitiesSystem>(m, "MCMoveProbabilitiesSystem")
      .def(pybind11::init<double, double, double>(), pybind11::arg("probabilityVolumeMove"),
           pybind11::arg("probabilityGibbsVolumeMove"), pybind11::arg("probabilityParallelTemperingSwap"))
      .def_readwrite("probabilityVolumeMove", &MCMoveProbabilitiesSystem::probabilityVolumeMove)
      .def_readwrite("probabilityGibbsVolumeMove", &MCMoveProbabilitiesSystem::probabilityGibbsVolumeMove)
      .def_readwrite("probabilityParallelTemperingSwap", &MCMoveProbabilitiesSystem::probabilityParallelTemperingSwap);

  pybind11::class_<Component> component(m, "Component");
  component
      .def(pybind11::init<size_t, const ForceField &, std::string, double, double, double, std::vector<Atom>, size_t,
                          size_t, const MCMoveProbabilitiesParticles &>(),
           pybind11::arg("componentId"), pybind11::arg("forceField"), pybind11::arg("componentName"),
           pybind11::arg("criticalTemperature"), pybind11::arg("criticalPressure"), pybind11::arg("acentricFactor"),
           pybind11::arg("definedAtoms"), pybind11::arg("numberOfBlocks"), pybind11::arg("numberOfLambdaBins"),
           pybind11::arg("particleProbabilities"))
      .def(pybind11::init<Component::Type, size_t, const ForceField &, std::string &, std::string, size_t, size_t,
                          const MCMoveProbabilitiesParticles &>(),
           pybind11::arg("type"), pybind11::arg("componentId"), pybind11::arg("forceField"),
           pybind11::arg("componentName"), pybind11::arg("fileName"), pybind11::arg("numberOfBlocks"),
           pybind11::arg("numberOfLambdaBins"), pybind11::arg("particleProbabilities"))
      .def_readonly("name", &Component::name)
      .def("__repr__", &Component::repr);

  pybind11::enum_<Component::Type>(component, "Type")
      .value("Framework", Component::Type::Framework)
      .value("Adsorbate", Component::Type::Adsorbate)
      .value("Cation", Component::Type::Cation)
      .export_values();

  pybind11::class_<System>(m, "System")
      .def(pybind11::init<size_t, std::optional<SimulationBox>, double, std::optional<double>, ForceField,
                          std::vector<Framework>, std::vector<Component>, std::vector<size_t>, size_t,
                          MCMoveProbabilitiesSystem>(),
           pybind11::arg("id"), pybind11::arg("simulationBox"), pybind11::arg("temperature"), pybind11::arg("pressure"),
           pybind11::arg("forceField"), pybind11::arg("frameworkComponents"), pybind11::arg("components"),
           pybind11::arg("initialNumberOfMolecules"), pybind11::arg("numberOfBlocks"),
           pybind11::arg("systemProbabilities"))
      .def("computeTotalEnergies", &System::computeTotalEnergies)
      .def_readwrite("atomPositions", &System::atomPositions)
      .def("__repr__", &System::repr);

  pybind11::class_<MonteCarlo>(m, "MonteCarlo")
      .def(pybind11::init<size_t, size_t, size_t, size_t, size_t, size_t, size_t, std::vector<System> &, RandomNumber &,
                          size_t>(),
           pybind11::arg("numberOfCycles"), pybind11::arg("numberOfInitializationCycles"),
           pybind11::arg("numberOfEquilibrationCycles"), pybind11::arg("printEvery"),
           pybind11::arg("writeBinaryRestartEvery"), pybind11::arg("rescaleWangLandauEvery"),
           pybind11::arg("optimizeMCMovesEvery"), pybind11::arg("systems"), pybind11::arg("randomSeed"),
           pybind11::arg("numberOfBlocks"))
      .def("run", &MonteCarlo::run);
}
