import raspa

ff = raspa.ForceField.exampleMoleculeForceField()

mcmoves = raspa.MCMoveProbabilitiesParticles(
    probabilityTranslationMove=0.5,
    probabilityRotationMove=0.5,
    probabilityReinsertionMove_CBMC=0.5,
    probabilitySwapMove=1.0,
    probabilityWidomMove=1.0,
)
co2 = raspa.Component.exampleCO2(0, ff, particleProbabilities=mcmoves)
cubtc = raspa.Framework(0, ff, "Cu-BTC", "Cu-BTC.cif", numberOfUnitCells=[1, 1, 1])

system = raspa.System(
    systemId=0,
    temperature=323.0,
    forceField=ff,
    components=[co2],
    initialNumberOfMolecules=[1],
    pressure=1e4,
    frameworkComponents=[cubtc],
)

mc = raspa.MonteCarlo(
    numberOfCycles=100000,
    numberOfInitializationCycles=20000,
    numberOfEquilibrationCycles=0,
    printEvery=5000,
    systems=[system],
)

mc.run()
