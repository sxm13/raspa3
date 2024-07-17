import raspalib
from .base import RaspaBase
from .forcefield import ForceField
from .simulationbox import SimulationBox
from .atom import Atom
from .utils import RaspaError,popSelf


class Framework(RaspaBase):
    def __init__(
        self,
        frameworkId: int,
        forceField: ForceField,
        componentName: str,
        fileName: str = None,
        simulationBox: SimulationBox = None,
        spaceGroupHallNumber: int = None,
        definedAtoms: list[Atom] = None,
        numberOfUnitCells: list[int] = [1, 1, 1],
    ):

        super().__init__(**popSelf(locals()))
        
        # pick either file or manual init
        if self._settings[fileName] is None:
            self.drop_args("fileName")
        else:
            self.drop_args("simulationBox", "spaceGroupHallNumber", "definedAtoms")

        self._cpp_obj = raspalib.Framework(**self.cpp_args())
