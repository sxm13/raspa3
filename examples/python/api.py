import raspa
import numpy as np

ff = raspa.ForceField(fileName="force_field.json")
box = raspa.SimulationBox(11.8671 * np.ones(3))

fw_atoms = [
    raspa.Atom(np.array([0.3683, 0.1847, 0.0]), 2.05, type=2),
    raspa.Atom(np.array([0.5, 0.2179, 0.0]), -1.025, type=0),
    raspa.Atom(np.array([0.2939, 0.2939, 0.0]), -1.025, type=0),
    raspa.Atom(np.array([0.3429, 0.1098, 0.1098]), -1.025, type=0),
]
itq = raspa.Framework(
    0, ff, "ITQ-29", spaceGroupHallNumber=517, definedAtoms=fw_atoms, simulationBox=box, numberOfUnitCells=[1, 1, 1]
)

print(ff)

mcmoves = raspa.MCMoveProbabilitiesParticles()
co2_atoms = [
    raspa.Atom(np.array([0.0, 0.0, 1.149]), -0.3256, type=5),
    raspa.Atom(np.array([0.0, 0.0, 0.0]), 0.6512, type=4),
    raspa.Atom(np.array([0.0, 0.0, -1.149]), -0.3256, type=5),
]
co2 = raspa.Component(
    0,
    ff,
    "CO2",
    criticalTemperature=304.1282,
    criticalPressure=7377300,
    acentricFactor=0.22394,
    definedAtoms=co2_atoms,
    particleProbabilities=mcmoves,
)

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

new = np.array(
    [
        [5.93355, 7.93355, 5.93355 + 1.149],
        [5.93355, 7.93355, 5.93355],
        [5.93355, 7.93355, 5.93355 - 1.149],
        [5.93355, 3.93355, 5.93355 + 1.149],
        [5.93355, 3.93355, 5.93355],
        [5.93355, 3.93355, 5.93355 - 1.149],
    ]
)
system.atomPositions = (np.arange(72, 78), new)
energy = system.computeTotalEnergies()
print(1.2027242847 * energy.moleculeMoleculeVDW)
