import numpy as np

import raspalib
from .base import RaspaBase
from .utils import popSelf


class Atom(RaspaBase):

    def __init__(
        self,
        position: np.ndarray,
        charge: float,
        lambda_: float = 1.0,
        moleculeId: int = 0,
        type: int = 0,
        componentId: int = 0,
        groupId: int = 0,
    ):
        super().__init__(**popSelf(locals()))

        self._settings["position"] = raspalib.double3(*self._settings["position"])

        # special word "lambda" will crash in python
        self._settings["lambda"] = self._settings.pop("lambda_")
        self._cpp_obj = raspalib.Atom(**self.cpp_args())

    @property
    def position(self):
        self._position = self._cpp_obj.position
        return np.array([self._position.x, self._position.y, self._position.z])

    @position.setter
    def position(self, position: np.ndarray):
        assert position.shape == (3,)
        self._position = raspalib.double3(*position)
        self._cpp_obj.position = self._position
