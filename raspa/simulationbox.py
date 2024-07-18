import numpy as np

import raspalib
from .utils import RaspaError, popSelf
from .base import RaspaBase


class SimulationBox(RaspaBase):
    def __init__(self, box: np.ndarray):
        if box.shape == (3,):
            boxType = raspalib.SimulationBox.Type.Rectangular
        elif box.shape == (6,):
            boxType = raspalib.SimulationBox.Type.Triclinic
        else:
            raise RaspaError(f"Box shape should be either (3,) or (6,) not {self.box.shape}")
        super().__init__(**popSelf(locals()))

        # can not init with kwargs, as "box" is given as either 3 or 6 floats
        self._cpp_obj = raspalib.SimulationBox(*self._settings["box"], self._settings["boxType"])

    @property
    def boxType(self):
        return self._cpp_obj.type.name
