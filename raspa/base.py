

class RaspaBase:
    def __init__(self, **kwargs):
        kwargs.pop("__class__")
        self._settings = kwargs
        self._cpp_obj = None
        
    def cpp_args(self) -> dict:
        cpp_dict = {}
        for key, val in self._settings.items():
            if hasattr(val, "_cpp_obj"):
                cpp_dict[key] = val._cpp_obj
            elif isinstance(val, list) and all(hasattr(v, "_cpp_obj") for v in val):
                cpp_dict[key] = [v._cpp_obj for v in val]
            else:
                cpp_dict[key] = val
        return cpp_dict
    
    def drop_args(self, *args):
        for arg in args:
            self._settings.pop(arg, None)

    def __getattr__(self, name):
        if name in self.__dict__:
            return self.__dict__[name]
        elif hasattr(self._cpp_obj, name):
            return getattr(self._cpp_obj, name)
        elif name in self._settings:
            return self._settings[name]
        

    def __repr__(self):
        if self._cpp_obj is not None and hasattr(self._cpp_obj, "__repr__"):
            return self._cpp_obj.__repr__()
        else:
            raise NotImplementedError("Object has no representation.")
