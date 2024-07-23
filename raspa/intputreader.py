import raspalib
from .base import RaspaBase
from .utils import popSelf

class InputReader(RaspaBase):
    def __init__(self, fileName: str = "simulation.json"):
        super().__init__(**popSelf(locals()))
        self._cpp_obj = raspalib.InputReader(**self.cpp_args())
        