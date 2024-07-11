class RaspaBase:
    def __init__(self):
        self._cpp_obj = None

    def __getattr__(self, name):
        privateName = f"_{self.__class__.__name__}__{name}"
        if name in self.__dict__:
            return self.__dict__[name]
        elif privateName in self.__dict__:
            return self.__dict__[privateName]

    def __repr__(self):
        if self._cpp_obj is not None and hasattr(self._cpp_obj, "__repr__"):
            return self._cpp_obj.__repr__()
        else:
            raise NotImplementedError("Object has no representation.")
