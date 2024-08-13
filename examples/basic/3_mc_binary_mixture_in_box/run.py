import raspa
import numpy as np

ff = raspa.ForceField.exampleMoleculeForceField(useCharge=True)
mcmoves = raspa.MCMoveProbabilitiesParticles(
    probabilityTranslationMove=1.0, probabilityReinsertionMove_CBMC=1.0, probabilityRotationMove=1.0
)


co2 = raspa.Component.exampleCO2(0, ff, particleProbabilities=mcmoves)
n2 = raspa.Component.exampleN2(1, ff, particleProbabilities=mcmoves)

system0 = raspa.System(
    systemId=0,
    temperature=300.0,
    forceField=ff,
    components=[co2, n2],
    initialNumberOfMolecules=[100, 0],
    simulationBox=raspa.SimulationBox(25.0 * np.ones(3)),
)

system1 = raspa.System(
    systemId=1,
    temperature=500.0,
    forceField=ff,
    components=[co2, n2],
    initialNumberOfMolecules=[0, 100],
    simulationBox=raspa.SimulationBox(30.0 * np.ones(3)),
)

mc = raspa.MonteCarlo(
    numberOfCycles=10000,
    numberOfInitializationCycles=1000,
    systems=[system0, system1],
)
mc.run()
