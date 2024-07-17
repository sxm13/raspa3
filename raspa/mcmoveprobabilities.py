import raspalib
from .base import RaspaBase
from .utils import popSelf


class MCMoveProbabilitiesParticles(RaspaBase):

    def __init__(self, probabilityTranslationMove: float = 1.0, probabilityRotationMove: float = 0.0):
        super().__init__(**popSelf(locals()))
        self._cpp_obj = raspalib.MCMoveProbabilitiesParticles(**self.cpp_args())

    @property
    def probabilityTranslationMove(self):
        return self._settings["probabilityTranslationMove"]

    @probabilityTranslationMove.setter
    def probabilityTranslationMove(self, probabilityTranslationMove: float):
        self._settings["probabilityTranslationMove"] = probabilityTranslationMove
        self._cpp_obj.probabilityTranslationMove = probabilityTranslationMove

    @property
    def probabilityRotationMove(self):
        return self._settings["probabilityRotationMove"]

    @probabilityRotationMove.setter
    def probabilityRotationMove(self, probabilityRotationMove: float):
        self._settings["probabilityRotationMove"] = probabilityRotationMove
        self._cpp_obj.probabilityRotationMove = probabilityRotationMove


class MCMoveProbabilitiesSystem(RaspaBase):

    def __init__(
        self,
        probabilityVolumeMove: float = 0.0,
        probabilityGibbsVolumeMove: float = 0.0,
        probabilityParallelTemperingSwap: float = 0.0,
    ):  
        super().__init__(**popSelf(locals()))
        self._cpp_obj = raspalib.MCMoveProbabilitiesSystem(**self.cpp_args())

    @property
    def probabilityVolumeMove(self):
        return self._settings["probabilityVolumeMovea"]

    @probabilityVolumeMove.setter
    def probabilityVolumeMove(self, probabilityVolumeMove: float):
        self._settings["probabilityVolumeMove"] = probabilityVolumeMove
        self._cpp_obj.probabilityVolumeMove = probabilityVolumeMove

    @property
    def probabilityGibbsVolumeMove(self):
        return self._settings["probabilityGibbsVolumeMove"]

    @probabilityGibbsVolumeMove.setter
    def probabilityGibbsVolumeMove(self, probabilityGibbsVolumeMove: float):
        self._settings["probabilityGibbsVolumeMove"] = probabilityGibbsVolumeMove
        self._cpp_obj.probabilityGibbsVolumeMove = probabilityGibbsVolumeMove

    @property
    def probabilityParallelTemperingSwap(self):
        return self._settings["probabilityParallelTemperingSwap"]

    @probabilityParallelTemperingSwap.setter
    def probabilityParallelTemperingSwap(self, probabilityParallelTemperingSwap: float):
        self._settings["probabilityParallelTemperingSwap"] = probabilityParallelTemperingSwap
        self._cpp_obj.probabilityParallelTemperingSwap = probabilityParallelTemperingSwap
