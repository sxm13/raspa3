import numpy as np

import raspalib
from .utils import RaspaError
from .base import RaspaBase


class SimulationBox(RaspaBase):
    def __init__(self, box: np.ndarray):
        super().__init__()

        self._box = box
        try:
            self.__boxType = {
                (3,): raspalib.SimulationBox.Type.Rectangular,
                (6,): raspalib.SimulationBox.Type.Triclinic,
            }[self.box.shape]
        except KeyError:
            raise RaspaError(f"Box shape should be either (3,) or (6,) not {self.box.shape}")

        self._cpp_obj = raspalib.SimulationBox(*self.box, self.__boxType)

    @property
    def box(self):
        return self._box

    @property
    def boxType(self):
        if self._cpp_obj.type == raspalib.SimulationBox.Type.Rectangular:
            return "Rectangular"
        elif self._cpp_obj.type == raspalib.SimulationBox.Type.Triclinic:
            return "Triclinic"
