class RaspaError(Exception):
    """Custom exception class for Raspa errors."""

    def __init__(self, message):
        super().__init__(message)
