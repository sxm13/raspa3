import raspalib
from .base import RaspaBase
from .forcefield import ForceField
from .atom import Atom
from .mcmoveprobabilities import MCMoveProbabilitiesParticles


class Component(RaspaBase):

    def __init__(
        self,
        componentId: int,
        forceField: ForceField,
        componentName: str,
        criticalTemperature: float,
        criticalPressure: float,
        acentricFactor: float,
        definedAtoms: list[Atom],
        numberOfBlocks: int = 5,
        numberOfLambdaBins: int = 21,
        moveProbabilities: MCMoveProbabilitiesParticles = MCMoveProbabilitiesParticles(),
    ):
        super().__init__()

        self.__componentId = componentId
        self.__forceField = forceField
        self.__componentName = componentName
        self.__criticalTemperature = criticalTemperature
        self.__criticalPressure = criticalPressure
        self.__acentricFactor = acentricFactor
        self.__definedAtoms = definedAtoms
        self.__numberOfBlocks = numberOfBlocks
        self.__numberOfLambdaBins = numberOfLambdaBins
        self.__moveProbabilities = moveProbabilities

        self._cpp_obj = raspalib.Component(
            componentId,
            forceField._cpp_obj,
            componentName,
            criticalTemperature,
            criticalPressure,
            acentricFactor,
            [atom._cpp_obj for atom in definedAtoms],
            numberOfBlocks,
            numberOfLambdaBins,
            moveProbabilities._cpp_obj,
        )
