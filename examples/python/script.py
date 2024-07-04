from raspa3 import raspalib

atomTypes = [
    raspalib.PseudoAtom("Si", 28.0855, 2.05, 14, False),
    raspalib.PseudoAtom("O", 15.999, -1.025, 8, False),
    raspalib.PseudoAtom("CH4", 16.04246, 0.0, 6, False),
    raspalib.PseudoAtom("C_co2", 12.0, 0.6512, 6, False),
    raspalib.PseudoAtom("O_co2", 15.9994, -0.3256, 8, False),
]
parameters = [
    raspalib.VDWParameters(22.0 / 1.2027242847, 2.30),
    raspalib.VDWParameters(53.0 / 1.2027242847, 3.3),
    raspalib.VDWParameters(158.5 / 1.2027242847, 3.72),
    raspalib.VDWParameters(29.933 / 1.2027242847, 2.745),
    raspalib.VDWParameters(85.671 / 1.2027242847, 3.017),
]

force_field = raspalib.ForceField(
    atomTypes, parameters, raspalib.ForceField.MixingRule.Lorentz_Berthelot, 12.0, True, False
)

print(force_field)

framework = raspalib.Framework(
    0,
    force_field,
    "ITQ-29",
    raspalib.SimulationBox(11.8671, 11.8671, 11.8671),
    517,
    [
        raspalib.Atom(raspalib.double3(0.3683, 0.1847, 0), 2.05, 1.0, 0, 0, 0, 0),
        raspalib.Atom(raspalib.double3(0.5, 0.2179, 0), -1.025, 1.0, 0, 1, 0, 0),
        raspalib.Atom(raspalib.double3(0.2939, 0.2939, 0), -1.025, 1.0, 0, 1, 0, 0),
        raspalib.Atom(raspalib.double3(0.3429, 0.1098, 0.1098), -1.025, 1.0, 0, 1, 0, 0),
    ],
    raspalib.int3(1, 1, 1),
)

component = raspalib.Component(
    0,
    force_field,
    "CO2",
    304.1282,
    7377300.0,
    0.22394,
    [
        raspalib.Atom(raspalib.double3(0.0, 0.0, 1.149), -0.3256, 1.0, 0, 4, 1, 0),
        raspalib.Atom(raspalib.double3(0.0, 0.0, 0.0), 0.6512, 1.0, 0, 3, 1, 0),
        raspalib.Atom(raspalib.double3(0.0, 0.0, -1.149), -0.3256, 1.0, 0, 4, 1, 0),
    ],
    5,
    21,
    raspalib.MCMoveProbabilitiesParticles(probabilityTranslationMove=1.0, probabilityRotationMove=1.0),
)

system = raspalib.System(0, None, 300.0, 1e4, force_field, [framework], [component], [2], 5)
# mc = MonteCarlo(system,....)

# mc.run(numberOfStep = 1000, )

# print(mc)

system.atomPositions[72].position = raspalib.double3(5.93355, 7.93355, 5.93355 + 1.149)
system.atomPositions[73].position = raspalib.double3(5.93355, 7.93355, 5.93355 + 0.0)
system.atomPositions[74].position = raspalib.double3(5.93355, 7.93355, 5.93355 - 1.149)
system.atomPositions[75].position = raspalib.double3(5.93355, 3.93355, 5.93355 + 1.149)
system.atomPositions[76].position = raspalib.double3(5.93355, 3.93355, 5.93355 + 0.0)
system.atomPositions[77].position = raspalib.double3(5.93355, 3.93355, 5.93355 - 1.149)

energy = system.computeTotalEnergies()
print(1.2027242847 * energy.moleculeMoleculeVDW)
