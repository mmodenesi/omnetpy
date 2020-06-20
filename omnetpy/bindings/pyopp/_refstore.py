"""Store references of object to prevent python from deleting them."""
class _RefStore:
    """This class keeps track of all messages created on the python side."""
    _store = set()

    @classmethod
    def save(cls, thing):
        cls._store.add(thing)
        # print('saved', thing, '({} things in store)'.format(len(cls._store)))

    @classmethod
    def delete(cls, thing):
        try:
            cls._store.remove(thing)
        except KeyError:
            pass
        else:
            pass
            # print('deleted', thing, '({} things in store)'.format(len(cls._store)))
