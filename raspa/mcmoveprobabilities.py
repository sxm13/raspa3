import raspalib
from .base import RaspaBase
from .utils import popSelf


class MCMoveProbabilitiesParticles(RaspaBase):

    def __init__(self, probabilityTranslationMove: float = 1.0, probabilityRotationMove: float = 0.0):
        super().__init__(**popSelf(locals()))
        print(self._settings)
        self._cpp_obj = raspalib.MCMoveProbabilitiesParticles(**self.cpp_args())
        print(self._cpp_obj.probabilityTranslationMove, self._cpp_obj.probabilityRotationMove)


class MCMoveProbabilitiesSystem(RaspaBase):

    def __init__(
        self,
        probabilityVolumeMove: float = 0.0,
        probabilityGibbsVolumeMove: float = 0.0,
        probabilityParallelTemperingSwap: float = 0.0,
    ):
        super().__init__(**popSelf(locals()))
        self._cpp_obj = raspalib.MCMoveProbabilitiesSystem(**self.cpp_args())
