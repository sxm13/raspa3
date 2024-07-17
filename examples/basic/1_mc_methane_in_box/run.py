import raspa
import numpy as np

ff = raspa.ForceField.exampleMoleculeForceField()
mcmoves = raspa.MCMoveProbabilitiesParticles()
methane = raspa.Component.exampleCH4(0, ff)


box = raspa.SimulationBox(30.0 * np.ones(3))
system = raspa.System(
    systemId=0,
    temperature=300.0,
    forceField=ff,
    components=[methane],
    initialNumberOfMolecules=[100],
    simulationBox=box,
)

mc = raspa.MonteCarlo(
    numberOfCycles=10000,
    numberOfInitializationCycles=1000,
    systems=[system],
)
print(mc)
# mc.run()
