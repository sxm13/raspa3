import raspalib


class SimulationType:
    pass


class MC(SimulationType):
    def __init__(self):
        self.name = "Monte Carlo"

        self._cpp_obj = raspalib.MonteCarlo()
