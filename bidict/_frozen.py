"""Implements :class:`bidict.frozenbidict`."""

from .compat import viewitems, iteritems
from ._common import BidictBase
from ._ordered import OrderedBidictBase
from functools import reduce


class frozenbidict(BidictBase):
    """Immutable, hashable bidict type."""

    def __hash__(self):
        """Return the hash of this frozenbidict."""
        if hasattr(self, '_hashval'):  # Computed lazily.
            return self._hashval
        # Use the _hash() implementation that our ItemsView provides (via collections.Set).
        self._hashval = hv = viewitems(self)._hash()
        return hv


class frozenorderedbidict(OrderedBidictBase):
    """Immutable, hashable ordered bidict type."""

    def __hash__(self):
        """Return the hash of this frozenorderedbidict."""
        if hasattr(self, '_hashval'):  # Computed lazily.
            return self._hashval
        self._hashval = reduce(lambda h, i: hash((h, i)),
                               iteritems(self),
                               hash(self.__class__))
        return self._hashval
