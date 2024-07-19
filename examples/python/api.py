import raspa
import numpy as np

ff = raspa.ForceField(fileName="force_field.json")
with open("ff2.txt", "w") as f:
    f.write(str(ff))
box = raspa.SimulationBox(11.8671 * np.ones(3))

itq = raspa.Framework(0, ff, "ITQ-29", fileName="ITQ-29.cif", numberOfUnitCells=[1, 1, 1])

# print(ff)

mcmoves = raspa.MCMoveProbabilitiesParticles(probabilityTranslationMove=1.0, probabilityRotationMove=1.0)
co2 = raspa.Component.exampleCO2(0, ff, particleProbabilities=mcmoves)

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

mc = raspa.MonteCarlo(numberOfCycles=10000, numberOfInitializationCycles=10000, systems=[system])
mc.run()
