import raspa.raspalib as raspalib
from .base import RaspaBase
from .utils import popSelf


class MCMoveProbabilitiesParticles(RaspaBase):
    """
    A class representing all particle move probabilities in RASPA.

    Inherits from RaspaBase.

    Attributes:
        _settings (dict): A dictionary storing the settings for the pseudo atom.
        _cpp_obj: A reference to the associated C++ object.
    """

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
        """
        Initialize a particle mc moves object that holds all probabilities for moves. It will be normalized after init.

        Args:
            probabilityTranslationMove (float, optional): _description_. Defaults to 0.0.
            probabilityRandomTranslationMove (float, optional): _description_. Defaults to 0.0.
            probabilityRotationMove (float, optional): _description_. Defaults to 0.0.
            probabilityRandomRotationMove (float, optional): _description_. Defaults to 0.0.
            probabilityVolumeMove (float, optional): _description_. Defaults to 0.0.
            probabilityReinsertionMove_CBMC (float, optional): _description_. Defaults to 0.0.
            probabilityIdentityChangeMove_CBMC (float, optional): _description_. Defaults to 0.0.
            probabilitySwapMove (float, optional): _description_. Defaults to 0.0.
            probabilitySwapMove_CBMC (float, optional): _description_. Defaults to 0.0.
            probabilitySwapMove_CFCMC (float, optional): _description_. Defaults to 0.0.
            probabilitySwapMove_CFCMC_CBMC (float, optional): _description_. Defaults to 0.0.
            probabilityGibbsVolumeMove (float, optional): _description_. Defaults to 0.0.
            probabilityGibbsSwapMove_CBMC (float, optional): _description_. Defaults to 0.0.
            probabilityGibbsSwapMove_CFCMC (float, optional): _description_. Defaults to 0.0.
            probabilityGibbsSwapMove_CFCMC_CBMC (float, optional): _description_. Defaults to 0.0.
            probabilityWidomMove (float, optional): _description_. Defaults to 0.0.
            probabilityWidomMove_CFCMC (float, optional): _description_. Defaults to 0.0.
            probabilityWidomMove_CFCMC_CBMC (float, optional): _description_. Defaults to 0.0.
            probabilityParallelTemperingSwap (float, optional): _description_. Defaults to 0.0.
        """

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
