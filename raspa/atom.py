import numpy as np

import raspalib
from .base import RaspaBase


class Atom(RaspaBase):
    def __init__(
        self,
        position: np.ndarray,
        charge: float,
        lambda_: float = 1.0,
        moleculeId: int = 0,
        type_: int = 0,
        componentId: int = 0,
        groupId: int = 0,
    ):
        super().__init__()

        # lambda and type get underscore to prevent clashes with python lambda and type
        assert position.shape == (3,)
        self._position = raspalib.double3(*position)
        self.__charge = charge
        self.__lambda_ = lambda_
        self.__moleculeId = moleculeId
        self.__type_ = type_
        self.__componentId = componentId
        self.__groupId = groupId

        self._cpp_obj = raspalib.Atom(
            self._position,
            self.__charge,
            self.__lambda_,
            self.__moleculeId,
            self.__type_,
            self.__componentId,
            self.__groupId,
        )

    @property
    def position(self):
        self._position = self._cpp_obj.position
        return np.array([self._position.x, self._position.y, self._position.z])

    @position.setter
    def position(self, position: np.ndarray):
        assert position.shape == (3,)
        self._position = raspalib.double3(*position)
        self._cpp_obj.position = self._position
