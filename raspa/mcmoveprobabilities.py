import raspalib
from .base import RaspaBase
from .utils import popSelf


class MCMoveProbabilitiesParticles(RaspaBase):

    def __init__(
        self,
        probabilityTranslationMove: float = 0.0,
        probabilityRandomTranslationMove: float = 0.0,
        probabilityRotationMove: float = 0.0,
        probabilityRandomRotationMove: float = 0.0,
        probabilityVolumeMove: float = 0.0,
        probabilityReinsertionMove_CBMC: float = 0.0,
        probabilityIdentityChangeMove_CBMC: float = 0.0,
        probabilitySwapMove: float = 0.0,
        probabilitySwapMove_CBMC: float = 0.0,
        probabilitySwapMove_CFCMC: float = 0.0,
        probabilitySwapMove_CFCMC_CBMC: float = 0.0,
        probabilityGibbsVolumeMove: float = 0.0,
        probabilityGibbsSwapMove_CBMC: float = 0.0,
        probabilityGibbsSwapMove_CFCMC: float = 0.0,
        probabilityGibbsSwapMove_CFCMC_CBMC: float = 0.0,
        probabilityWidomMove: float = 0.0,
        probabilityWidomMove_CFCMC: float = 0.0,
        probabilityWidomMove_CFCMC_CBMC: float = 0.0,
        probabilityParallelTemperingSwap: float = 0.0,
    ):

        super().__init__(**popSelf(locals()))
        self._cpp_obj = raspalib.MCMoveProbabilitiesParticles(**self.cpp_args())


class MCMoveProbabilitiesSystem(RaspaBase):

    def __init__(
        self,
        probabilityVolumeMove: float = 0.0,
        probabilityGibbsVolumeMove: float = 0.0,
        probabilityParallelTemperingSwap: float = 0.0,
    ):
        super().__init__(**popSelf(locals()))
        self._cpp_obj = raspalib.MCMoveProbabilitiesSystem(**self.cpp_args())
