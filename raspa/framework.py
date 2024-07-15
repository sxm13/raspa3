import raspalib
from .base import RaspaBase
from .forcefield import ForceField
from .simulationbox import SimulationBox
from .atom import Atom
from .utils import RaspaError


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
        super().__init__()
        self.__frameworkId = frameworkId
        self.__forceField = forceField
        self.__componentName = componentName
        self.__fileName = fileName
        self.__simulationBox = simulationBox
        self.__spaceGroupHallNumber = spaceGroupHallNumber
        self.__definedAtoms = definedAtoms
        self.__numberOfUnitCells = numberOfUnitCells

        if fileName is not None:
            if simulationBox is not None or spaceGroupHallNumber is not None or definedAtoms is not None:
                raise RaspaError("Pick either json or definedAtoms to init force field, not both")
            self._cpp_obj = raspalib.Framework(
                frameworkId,
                forceField._cpp_obj,
                componentName,
                fileName,
                numberOfUnitCells,
            )
        elif simulationBox is not None and spaceGroupHallNumber is not None and definedAtoms is not None:
            self._cpp_obj = raspalib.Framework(
                frameworkId,
                forceField._cpp_obj,
                componentName,
                simulationBox._cpp_obj,
                spaceGroupHallNumber,
                [atom._cpp_obj for atom in definedAtoms],
                raspalib.int3(*numberOfUnitCells),
            )
        else:
            raise RaspaError("Pick either json or definedAtoms to init force field.")
