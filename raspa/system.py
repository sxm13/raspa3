import raspalib
from .base import RaspaBase
from .utils import popSelf
from .simulationbox import SimulationBox
from .forcefield import ForceField
from .framework import Framework
from .component import Component
from .mcmoveprobabilities import MCMoveProbabilitiesSystem

import numpy as np

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
        frameworkComponents: list[Framework] = [],
        simulationBox: SimulationBox = None,
        systemProbabilities: MCMoveProbabilitiesSystem = MCMoveProbabilitiesSystem(),
    ):
        super().__init__(**popSelf(locals()))
        self._cpp_obj = raspalib.System(**self.cpp_args())

    @property
    def atomPositions(self):
        return np.array([[atom.position.x, atom.position.y, atom.position.z] for atom in self._cpp_obj.atomPositions])

    @atomPositions.setter
    def atomPositions(self, index_position_tuple: tuple[np.ndarray, np.ndarray]):
        indices, position = index_position_tuple
        for i, idx in enumerate(indices):
            self._cpp_obj.atomPositions[idx].position = raspalib.double3(*position[i])

    def computeTotalEnergies(self):
        return self._cpp_obj.computeTotalEnergies()
