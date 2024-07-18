import raspalib
from .base import RaspaBase
from .forcefield import ForceField
from .atom import Atom
from .mcmoveprobabilities import MCMoveProbabilitiesParticles
from .utils import RaspaError, SHARE_DIR, popSelf

import os


class Component(RaspaBase):

    def __init__(
        self,
        componentId: int,
        forceField: ForceField,
        componentName: str,
        fileName: str = None,
        criticalTemperature: float = None,
        criticalPressure: float = None,
        acentricFactor: float = None,
        definedAtoms: list[Atom] = None,
        numberOfBlocks: int = 5,
        numberOfLambdaBins: int = 21,
        particleProbabilities: MCMoveProbabilitiesParticles = MCMoveProbabilitiesParticles(),
    ):
        super().__init__(**popSelf(locals()))

        if self._settings["fileName"] is None:
            self.drop_args("fileName")
        else:
            self._settings["type"] = raspalib.Component.Type.Adsorbate
            self.drop_args("criticalTemperature", "criticalPressure", "acentricFactor", "definedAtoms")

        self._cpp_obj = raspalib.Component(**self.cpp_args())

    @classmethod
    def exampleCO2(
        cls,
        componentId: int,
        forceField: ForceField,
        numberOfBlocks: int = 5,
        numberOfLambdaBins: int = 21,
        particleProbabilities: MCMoveProbabilitiesParticles = MCMoveProbabilitiesParticles(),
    ):
        settings = locals()
        settings.pop("cls")
        return cls(
            componentName="CO2", fileName=os.path.join(SHARE_DIR, "molecules", "example_definitions", "co2"), **settings
        )

    @classmethod
    def exampleCH4(
        cls,
        componentId: int,
        forceField: ForceField,
        numberOfBlocks: int = 5,
        numberOfLambdaBins: int = 21,
        particleProbabilities: MCMoveProbabilitiesParticles = MCMoveProbabilitiesParticles(),
    ):
        settings = locals()
        settings.pop("cls")
        return cls(
            componentName="CH4",
            fileName=os.path.join(SHARE_DIR, "molecules", "example_definitions", "methane"),
            **settings,
        )

    @classmethod
    def exampleN2(
        cls,
        componentId: int,
        forceField: ForceField,
        numberOfBlocks: int = 5,
        numberOfLambdaBins: int = 21,
        particleProbabilities: MCMoveProbabilitiesParticles = MCMoveProbabilitiesParticles(),
    ):
        settings = locals()
        settings.pop("cls")
        return cls(
            componentName="N2", fileName=os.path.join(SHARE_DIR, "molecules", "example_definitions", "co2"), **settings
        )
