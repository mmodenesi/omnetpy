from functools import wraps


def no_binding_for_method(method):
    """Use as a decorator for methods explicitly chosen not to be ported to python."""

    @wraps(method)
    def inner(self, *args, **kwargs):
        msg = '{}.{} not ported to python'.format(
                self.__class__.__name__, method.__name__)
        raise NotImplementedError(msg)

    return inner
