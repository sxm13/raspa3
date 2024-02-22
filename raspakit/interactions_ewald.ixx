export module interactions_ewald;

import <span>;
import <optional>;
import <tuple>;
import <complex>;
import <vector>;
             
import double3;
import double3x3;                                                                                                       
import atom;                                                                                                            
import running_energy;                                                                                                  
import energy_status;                                                                                                   
import simulationbox;                                                                                                   
import force_factor;                                                                                                    
import forcefield;                                                                                                      
import component;

export namespace Interactions
{
  double computeEwaldFourierEnergySingleIon(std::vector<std::complex<double>> &eik_x,
                                            std::vector<std::complex<double>> &eik_y,
                                            std::vector<std::complex<double>> &eik_z,
                                            std::vector<std::complex<double>> &eik_xy,
                                            const ForceField &forceField,
                                            const SimulationBox &simulationBox,
                                            double3 position,
                                            double charge);

  void computeEwaldFourierRigidEnergy(std::vector<std::complex<double>> &eik_x,
                                      std::vector<std::complex<double>> &eik_y,
                                      std::vector<std::complex<double>> &eik_z,
                                      std::vector<std::complex<double>> &eik_xy,
                                      std::vector<std::pair<std::complex<double>, std::complex<double>>> &fixedFrameworkStoredEik,
                                      const ForceField &forceField, 
                                      const SimulationBox &simulationBox,
                                      std::span<const Atom> frameworkAtoms, 
                                      RunningEnergy& energyStatus);

  void computeEwaldFourierEnergy(std::vector<std::complex<double>> &eik_x,
                                 std::vector<std::complex<double>> &eik_y,
                                 std::vector<std::complex<double>> &eik_z,
                                 std::vector<std::complex<double>> &eik_xy,
                                 std::vector<std::pair<std::complex<double>, std::complex<double>>> &fixedFrameworkStoredEik,
                                 std::vector<std::pair<std::complex<double>, std::complex<double>>> &storedEik,
                                 const ForceField &forceField, 
                                 const SimulationBox &simulationBox,
                                 const std::vector<Component> &components,
                                 const std::vector<size_t> &numberOfMoleculesPerComponent,
                                 std::span<const Atom> flexibleAtomPositions, 
                                 std::span<const Atom> atoms, 
                                 RunningEnergy &energyStatus);

  RunningEnergy energyDifferenceEwaldFourier(std::vector<std::complex<double>> &eik_x,
                                             std::vector<std::complex<double>> &eik_y,
                                             std::vector<std::complex<double>> &eik_z,
                                             std::vector<std::complex<double>> &eik_xy,
                                             std::vector<std::pair<std::complex<double>, std::complex<double>>> &storedEik,
                                             std::vector<std::pair<std::complex<double>, std::complex<double>>> &totalEik,
                                             const ForceField &forceField, const SimulationBox &simulationBox,
                                             std::span<const Atom> newatoms, std::span<const Atom> oldatoms);

  ForceFactor computeEwaldFourierGradient(std::vector<std::complex<double>> &eik_x,
                                          std::vector<std::complex<double>> &eik_y,
                                          std::vector<std::complex<double>> &eik_z,
                                          std::vector<std::complex<double>> &eik_xy,
                                          const ForceField &forceField, const SimulationBox &simulationBox,
                                          std::span<Atom> atomPositions);

  std::pair<EnergyStatus, double3x3> computeEwaldFourierEnergyStrainDerivative(std::vector<std::complex<double>> &eik_x,
                                             std::vector<std::complex<double>> &eik_y,
                                             std::vector<std::complex<double>> &eik_z,
                                             std::vector<std::complex<double>> &eik_xy,
                                             std::vector<std::pair<std::complex<double>, std::complex<double>>> &fixedFrameworkStoredEik,
                                             std::vector<std::pair<std::complex<double>, std::complex<double>>> &storedEik,
                                             const ForceField &forceField, const SimulationBox &simulationBox,
                                             const std::vector<Component> &components,
                                             const std::vector<size_t> &numberOfMoleculesPerComponent,
                                             std::span<Atom> atomPositions) noexcept;


  void acceptEwaldMove(const ForceField &forceField,
                       std::vector<std::pair<std::complex<double>, std::complex<double>>> &storedEik,
                       std::vector<std::pair<std::complex<double>, std::complex<double>>> &totalEik);
}

export struct Ewald
{
  std::vector<std::complex<double>> eik_xy;
  std::vector<std::complex<double>> eik_x;
  std::vector<std::complex<double>> eik_y;
  std::vector<std::complex<double>> eik_z;
  std::vector<std::pair<std::complex<double>, std::complex<double>>> storedEik;
  std::vector<std::pair<std::complex<double>, std::complex<double>>> fixedFrameworkStoredEik;
  std::vector<std::pair<std::complex<double>, std::complex<double>>> totalEik;
  double CoulombicFourierEnergySingleIon{ 0.0 };


  ForceFactor computeEwaldFourierGradient(const ForceField &forceField, const SimulationBox &simulationBox,
                                               std::span<Atom> atomPositions);


  void acceptEwaldMove(const ForceField &forceField);
};
