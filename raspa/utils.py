import os
from importlib.resources import files

RASPA_DIR = files(__package__)
SHARE_DIR = os.path.join(RASPA_DIR, "..", "share", "raspa3")

class RaspaError(Exception):
    """Custom exception class for Raspa errors."""

    def __init__(self, message):
        super().__init__(message)
