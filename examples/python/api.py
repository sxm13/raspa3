import raspa
from raspa import raspalib
import numpy as np

ff = raspa.ForceField.from_json("force_field.json")

box = raspa.SimulationBox(11.8671 * np.ones(3))

fw_atoms = [
    raspa.Atom(np.array([0.3683, 0.1847, 0.0]), 2.05, type_=2),
    raspa.Atom(np.array([0.5, 0.2179, 0.0]), -1.025, type_=0),
    raspa.Atom(np.array([0.2939, 0.2939, 0.0]), -1.025, type_=0),
    raspa.Atom(np.array([0.3429, 0.1098, 0.1098]), -1.025, type_=0),
]
itq = raspa.Framework(
    0, ff, "ITQ-29", spaceGroupHallNumber=517, definedAtoms=fw_atoms, simulationBox=box, numberOfUnitCells=[1, 1, 1]
)

print(ff)

mcmoves = raspa.MCMoveProbabilitiesParticles()

co2_atoms = [
    raspa.Atom(np.array([0.0, 0.0, 1.149]), -0.3256, type_=5),
    raspa.Atom(np.array([0.0, 0.0, 0.0]), 0.6512, type_=4),
    raspa.Atom(np.array([0.0, 0.0, -1.149]), -0.3256, type_=5),
]
co2 = raspa.Component(0, ff, "CO2", 304.1282, 7377300, 0.22394, co2_atoms, 5, 21, mcmoves)

system = raspa.System(
    systemId=0,
    simulationBox=None,
    temperature=300.0,
    pressure=1e4,
    forceField=ff,
    frameworkComponents=[itq],
    components=[co2],
    initialNumberOfMolecules=[2],
    numberOfBlocks=5,
)
# mc = MonteCarlo(system,....)

# mc.run(numberOfStep = 1000, )

# print(mc)

new = np.array(
    [
        [5.93355, 7.93355, 5.93355 + 1.149],
        [5.93355, 7.93355, 5.93355],
        [5.93355, 7.93355, 5.93355 - 1.149],
        [5.93355, 7.93355, 5.93355 + 1.149],
        [5.93355, 7.93355, 5.93355],
        [5.93355, 7.93355, 5.93355 - 1.149],
    ]
)
system.atomPositions = (np.arange(72, 78), new)


system.atomPositions[72].position = raspalib.double3(5.93355, 7.93355, 5.93355 + 1.149)
system.atomPositions[73].position = raspalib.double3(5.93355, 7.93355, 5.93355 + 0.0)
system.atomPositions[74].position = raspalib.double3(5.93355, 7.93355, 5.93355 - 1.149)
system.atomPositions[75].position = raspalib.double3(5.93355, 3.93355, 5.93355 + 1.149)
system.atomPositions[76].position = raspalib.double3(5.93355, 3.93355, 5.93355 + 0.0)
system.atomPositions[77].position = raspalib.double3(5.93355, 3.93355, 5.93355 - 1.149)

energy = system.computeTotalEnergies()
print(1.2027242847 * energy.moleculeMoleculeVDW)
