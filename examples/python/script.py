from raspa import raspalib


atomTypes = [
    raspalib.PseudoAtom("Si", 28.0855, 2.05, 0.0, 14, False, "-"),
    raspalib.PseudoAtom("O", 15.999, -1.025, 0.0, 8, False, "-"),
    raspalib.PseudoAtom("CH4", 16.04246, 0.0, 0.0, 6, False, "-"),
    raspalib.PseudoAtom("C_co2", 12.0, 0.6512, 0.0, 6, False, "-"),
    raspalib.PseudoAtom("O_co2", 15.9994, -0.3256, 0.0, 8, False, "-"),
]
parameters = [
    raspalib.VDWParameters(22.0, 2.30),
    raspalib.VDWParameters(53.0, 3.3),
    raspalib.VDWParameters(158.5, 3.72),
    raspalib.VDWParameters(29.933, 2.745),
    raspalib.VDWParameters(85.671, 3.017),
]
ff = raspalib.ForceField(atomTypes, parameters, raspalib.ForceField.MixingRule.Lorentz_Berthelot, 12.0, True, False)

# ff2 = raspalib.ForceField("force_field.json")


itq = raspalib.Framework(
    0,
    ff,
    "ITQ-29",
    "ITQ-29.cif",
    raspalib.int3(1, 1, 1),
)

co2 = raspalib.Component(
    raspalib.Component.Adsorbate,
    0,
    ff,
    "CO2",
    "CO2.json",
    5,
    21,
    raspalib.MCMoveProbabilitiesParticles(probabilityTranslationMove=1.0, probabilityRotationMove=1.0),
)

sysMC = raspalib.MCMoveProbabilitiesSystem(0.0, 0.0, 0.0)
system = raspalib.System(0, None, 300.0, 1e4, ff, [itq], [co2], [2], 5, sysMC)

system.atomPositions[72].position = raspalib.double3(5.93355, 7.93355, 5.93355 + 1.149)
system.atomPositions[73].position = raspalib.double3(5.93355, 7.93355, 5.93355 + 0.0)
system.atomPositions[74].position = raspalib.double3(5.93355, 7.93355, 5.93355 - 1.149)
system.atomPositions[75].position = raspalib.double3(5.93355, 3.93355, 5.93355 + 1.149)
system.atomPositions[76].position = raspalib.double3(5.93355, 3.93355, 5.93355 + 0.0)
system.atomPositions[77].position = raspalib.double3(5.93355, 3.93355, 5.93355 - 1.149)

energy = system.computeTotalEnergies()
print(1.2027242847 * energy.moleculeMoleculeVDW)

random = raspalib.random(12)
mc = raspalib.MonteCarlo(
    numberOfCycles=10000,
    numberOfInitializationCycles=10000,
    numberOfEquilibrationCycles=0,
    printEvery=1000,
    writeBinaryRestartEvery=100,
    rescaleWangLandauEvery=100,
    optimizeMCMovesEvery=100,
    systems=[system],
    randomSeed=random,
    numberOfBlocks=5,
)

mc.run()
