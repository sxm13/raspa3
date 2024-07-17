from typing import Literal

import raspalib
from .base import RaspaBase
from .utils import RASPA_DIR, SHARE_DIR, popSelf
import os
import json

class PseudoAtom(RaspaBase):

    def __init__(
        self,
        name: str = "C",
        mass: float = 1.0,
        charge: float = 0.0,
        polarizability: float = 0.0,
        atomicNumber: int = 8,
        printToPDB: bool = False,
        source: str = "-",
    ):
        super().__init__(**popSelf(locals()))
        self._cpp_obj = raspalib.PseudoAtom(**self.cpp_args())

    @classmethod
    def from_dict(cls, config: dict):
        config.pop("element")
        config.pop("print_as")
        config["printToPDB"] = config.pop("print_to_output")
        return cls(**config)


class VDWParameter(RaspaBase):
    def __init__(self, epsilon: float, sigma: float):
        super().__init__(**popSelf(locals()))
        self._cpp_obj = raspalib.VDWParameters(**self.cpp_args())

    @classmethod
    def from_dict(cls, config: dict):
        return cls(*config["parameters"])


class ForceField(RaspaBase):

    def __init__(
        self,
        pseudoAtoms: list[PseudoAtom],
        parameters: list[VDWParameter],
        mixingRule: Literal["Lorentz_Berthelot"] = "Lorentz_Berthelot",
        cutOff: float = 12.0,
        shifted: bool = False,
        tailCorrections: bool = False,
    ):
        mixingRule = getattr(raspalib.ForceField.MixingRule, mixingRule)
        super().__init__(**popSelf(locals()))
        self._cpp_obj = raspalib.ForceField(**self.cpp_args())

    @classmethod
    def from_json(cls, ff_path: str):
        with open(ff_path) as f:
            config = json.load(f)
        pseudoAtoms = [PseudoAtom.from_dict(psD) for psD in config["PseudoAtoms"]]
        vdwParameters = [VDWParameter.from_dict(vdwD) for vdwD in config["SelfInteractions"]]

        return cls(
            pseudoAtoms,
            vdwParameters,
            mixingRule=config["MixingRule"],
            shifted=config["TruncationMethod"] == "shifted",
            tailCorrections=config["TailCorrections"],
        )

    @classmethod
    def exampleMoleculeForceField(cls):
        return ForceField.from_json(
            os.path.join(SHARE_DIR, "forcefields", "example_molecule_forcefield", "force_field.json")
        )
