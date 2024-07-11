import raspalib
from .base import RaspaBase


class MCMoveProbabilitiesParticles(RaspaBase):
    def __init__(self, probabilityTranslationMove: float = 0.0, probabilityRotationMove: float = 0.0):
        self._probabilityTranslationMove = probabilityTranslationMove
        self._probabilityRotationMove = probabilityRotationMove
        self._cpp_obj = raspalib.MCMoveProbabilitiesParticles(probabilityTranslationMove, probabilityRotationMove)

    @property
    def probabilityTranslationMove(self):
        return self._probabilityTranslationMove

    @probabilityTranslationMove.setter
    def probabilityTranslationMove(self, probabilityTranslationMove: float):
        self._probabilityTranslationMove = probabilityTranslationMove
        self._cpp_obj.probabilityTranslationMove = probabilityTranslationMove

    @property
    def probabilityRotationMove(self):
        return self._probabilityRotationMove

    @probabilityRotationMove.setter
    def probabilityRotationMove(self, probabilityRotationMove: float):
        self._probabilityRotationMove = probabilityRotationMove
        self._cpp_obj.probabilityRotationMove = probabilityRotationMove
