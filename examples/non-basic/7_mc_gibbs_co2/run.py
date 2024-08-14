import raspa
import numpy as np

ff = raspa.ForceField.exampleMoleculeForceField()

mcmoves = raspa.MCMoveProbabilitiesParticles(
    probabilityTranslationMove=0.5,
    probabilityRotationMove=0.5,
    probabilityReinsertionMove_CBMC=0.5,
    probabilityGibbsSwapMove_CBMC=1.0,
)
co2 = raspa.Component.exampleCO2(0, ff, particleProbabilities=mcmoves)

sysmoves = raspa.MCMoveProbabilitiesSystem(probabilityGibbsVolumeMove=0.01)
systems = [
    raspa.System(
        systemId=i,
        temperature=240.0,
        systemProbabilities=sysmoves,
        components=[co2],
        forceField=ff,
        simulationBox=raspa.SimulationBox(30.0 * np.ones(3)),
        initialNumberOfMolecules=[256],
    )
    for i in range(2)
]

mc = raspa.MonteCarlo(numberOfCycles=25000, numberOfInitializationCycles=10000, systems=systems)

mc.run()
