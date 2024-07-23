import raspalib

from .base import RaspaBase
from .system import System
from .intputreader import InputReader
from .utils import popSelf


class RandomSeed(RaspaBase):
    def __init__(self, seed=12):
        super().__init__(**popSelf(locals()))
        self._cpp_obj = raspalib.random(**self.cpp_args())


class MonteCarlo(RaspaBase):
    def __init__(
        self,
        numberOfCycles: int = 0,
        numberOfInitializationCycles: int = 0,
        systems: list[System] = None,
        numberOfEquilibrationCycles: int = 0,
        printEvery: int = 1000,
        writeBinaryRestartEvery: int = 100,
        rescaleWangLandauEvery: int = 100,
        optimizeMCMovesEvery: int = 100,
        randomSeed: RandomSeed = RandomSeed(),
        numberOfBlocks: int = 5,
        inputReader: InputReader = None,
    ):
        super().__init__(**popSelf(locals()))

        if self._settings["inputReader"] is None:
            self.drop_args("inputReader")
        else:
            self.drop_args(
                "numberOfCycles",
                "numberOfInitializationCycles",
                "systems",
                "numberOfEquilibrationCycles",
                "printEvery",
                "writeBinaryRestartEvery",
                "rescaleWangLandauEvery",
                "optimizeMCMovesEvery",
                "randomSeed",
                "numberOfBlocks",
            )

        self._cpp_obj = raspalib.MonteCarlo(**self.cpp_args())

    def run(self):
        self._cpp_obj.run()

    def initialize(self):
        self._cpp_obj.initialize()

    def equilibrate(self):
        self._cpp_obj.equilibrate()

    def production(self):
        self._cpp_obj.production()
