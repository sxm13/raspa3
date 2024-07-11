import raspalib
from .base import RaspaBase
from .simulationbox import SimulationBox
from .forcefield import ForceField
from .framework import Framework
from .component import Component


class System(RaspaBase):
    def __init__(
        self,
        systemId: int,
        temperature: float,
        forceField: ForceField,
        components: list[Component],
        initialNumberOfMolecules: list[int],
        numberOfBlocks: int = 5,
        pressure: float = None,
        frameworkComponents: list[Framework] = None,
        simulationBox: SimulationBox = None,
    ):
        super().__init__()

        self.__systemId = systemId
        self.__temperature = temperature
        self.__forceField = forceField
        self.__components = components
        self.__initialNumberOfMolecules = initialNumberOfMolecules
        self.__numberOfBlocks = numberOfBlocks
        self.__pressure = pressure
        self.__frameworkComponents = frameworkComponents
        self.__simulationBox = simulationBox

        self._cpp_obj = raspalib.System(
            systemId,
            simulationBox._cpp_obj if simulationBox is not None else None,
            temperature,
            pressure,
            forceField._cpp_obj,
            [fwC._cpp_obj for fwC in frameworkComponents] if frameworkComponents is not None else None,
            [c._cpp_obj for c in components],
            initialNumberOfMolecules,
            numberOfBlocks,
        )
