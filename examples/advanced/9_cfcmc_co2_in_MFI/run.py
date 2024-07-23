import raspa

ff = raspa.ForceField(fileName="force_field.json")

mcmoves = raspa.MCMoveProbabilitiesParticles(
    probabilityTranslationMove=0.5,
    probabilityRotationMove=0.5,
    probabilityReinsertionMove_CBMC=0.5,
    probabilitySwapMove_CBMC=1.0,
    probabilityWidomMove=1.0,
)
co2 = raspa.Component.exampleCO2(0, ff, particleProbabilities=mcmoves)
mfi = raspa.Framework(0, ff, "MFI_SI", "MFI_SI.cif", numberOfUnitCells=[2, 2, 2])

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
    numberOfCycles=10000,
    numberOfInitializationCycles=2000,
    numberOfEquilibrationCycles=0,
    printEvery=500,
    systems=[system],
)

mc.run()
