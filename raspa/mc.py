import raspalib

from .base import RaspaBase
from .system import System
from .utils import popSelf

class RandomSeed(RaspaBase):
    def __init__(self, seed=12):
        super().__init__(**popSelf(locals()))
        self._cpp_obj = raspalib.random(**self.cpp_args())

class MonteCarlo(RaspaBase):
    def __init__(
        self,
        numberOfCycles: int,
        numberOfInitializationCycles: int,
        systems: list[System],
        numberOfEquilibrationCycles: int = 0,
        printEvery: int = 1000,
        writeBinaryRestartEvery: int = 100,
        rescaleWangLandauEvery: int = 100,
        optimizeMCMovesEvery: int = 100,
        randomSeed: RandomSeed = RandomSeed(),
        numberOfBlocks: int = 5,
    ):
        super().__init__(**popSelf(locals()))
        self._cpp_obj = raspalib.MonteCarlo(**self.cpp_args())

    def run(self):
        self._cpp_obj.run()
