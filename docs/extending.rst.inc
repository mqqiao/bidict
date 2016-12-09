.. _extending:

Extending ``bidict``
--------------------

Although bidict ships with all the bidict types we just covered,
it's always possible users might need something more than what's provided.
For this reason,
bidict was written with extensibility in mind.

Let's look at some examples.
Suppose you need a bidict that maintains its items in sorted order.
The Python standard library does not include any sorted dict types,
but the excellent
`sortedcontainers <http://www.grantjenks.com/docs/sortedcontainers/>`_ and
`sortedcollections <http://www.grantjenks.com/docs/sortedcollections/>`_
libraries do.
Armed with these along with bidict's
:attr:`_fwd_class <bidict.BidictBase._fwd_class>`
and
:attr:`_inv_class <bidict.BidictBase._inv_class>`
attributes,
creating a sorted bidict type is dead simple::

    >>> import bidict, sortedcontainers
    >>>
    >>> # a sorted bidict whose forward items stay sorted by their keys,
    >>> # and whose inverse items stay sorted by *their* keys (i.e. it and
    >>> # its inverse iterate over their items in different orders):
    >>>
    >>> class sortedbidict1(bidict.OrderedMapping, bidict.bidict):
    ...     _fwd_class = sortedcontainers.SortedDict
    ...     _inv_class = sortedcontainers.SortedDict
    >>>
    >>> b = sortedbidict1({'Tokyo': 'Japan', 'Cairo': 'Egypt', 'Lima': 'Peru'})
    >>>
    >>> list(b.items())  # b stays sorted by its keys
    [('Cairo', 'Egypt'), ('Lima', 'Peru'), ('Tokyo', 'Japan')]
    >>>
    >>> list(b.inv.items())  # b.inv stays sorted by *its* keys (b's values!)
    [('Egypt', 'Cairo'), ('Japan', 'Tokyo'), ('Peru', 'Lima')]
    >>>
    >>> # a sorted bidict whose forward items stay sorted by their keys,
    >>> # and whose inverse items stay sorted by their values (i.e. it and
    >>> # its inverse iterate over their items in the same order):
    >>>
    >>> import sortedcollections
    >>>
    >>> class sortedbidict2(bidict.OrderedMapping, bidict.bidict):
    ...     _fwd_class = sortedcontainers.SortedDict
    ...     _inv_class = sortedcollections.ValueSortedDict
    >>>
    >>> elemByAtomicNum = sortedbidict2({1: 'hydrogen', 2: 'helium', 3: 'lithium'})
    >>>
    >>> # forward items sorted by key:
    >>> list(elemByAtomicNum.items())
    [(1, 'hydrogen'), (2, 'helium'), (3, 'lithium')]
    >>>
    >>> # inverse items sorted by value:
    >>> list(elemByAtomicNum.inv.items())
    [('hydrogen', 1), ('helium', 2), ('lithium', 3)]
    >>>
    >>> elemByAtomicNum.update({4: 'beryllium'})
    >>> list(elemByAtomicNum.inv.items())
    [('hydrogen', 1), ('helium', 2), ('lithium', 3), ('beryllium', 4)]
    >>>
    >>> # order is preserved correctly when passing .inv back into constructor:
    >>> atomicNumByElem = sortedbidict2(elemByAtomicNum.inv)
    >>> list(atomicNumByElem.items()) == list(elemByAtomicNum.inv.items())
    True
    >>> # copies of .inv preserve order correctly too:
    >>> list(elemByAtomicNum.inv.copy().items()) == list(atomicNumByElem.items())
    True
    >>>
    >>> # attrs not defined by bidict are passed through to the _fwd SortedDict:
    >>> elemByAtomicNum.peekitem(index=0)
    (1, 'hydrogen')
    >>> elemByAtomicNum.popitem(last=False)
    (1, 'hydrogen')
    >>> elemByAtomicNum.inv.popitem(last=True)
    ('beryllium', 4)
    >>>
    >>> # bidict provides an OrderedMapping ABC for interoperability.
    >>> # Its __eq__ method is order-sensitive when comparing against another
    >>> # OrderedMapping. If we want our sortedbidicts to have order-sensitive
    >>> # comparison, we can inherit from OrderedMapping before inheriting
    >>> # from bidict, so OrderedMapping's __eq__ comes first in the MRO.
    >>> # Since this is what we did above, the following works:
    >>> elemByAtomicNum == bidict.orderedbidict([(2, 'helium'), (3, 'lithium')])
    True
    >>> elemByAtomicNum != bidict.orderedbidict([(3, 'lithium'), (2, 'helium')])
    True
    >>> # comparing against an unordered Mapping is order-insensitive:
    >>> elemByAtomicNum == {3: 'lithium', 2: 'helium'}
    True
    >>> # Since bidict.OrderedMapping implements __subclasshook__, classes like
    >>> # SortedDict and OrderedDict are detected as subclasses automatically,
    >>> # and so the following works:
    >>> issubclass(sortedcontainers.SortedDict, bidict.OrderedMapping)
    True
    >>> import collections
    >>> issubclass(collections.OrderedDict, bidict.OrderedMapping)
    True
    >>> elemByAtomicNum == collections.OrderedDict([(2, 'helium'), (3, 'lithium')])
    True
    >>> elemByAtomicNum != collections.OrderedDict([(3, 'lithium'), (2, 'helium')])
    True
    >>> # If we wanted our sortedbidicts to be automatically picked up as
    >>> # subclasses of Python 3.6's collections.abc.Reversible, we could
    >>> # add a __reversed__ class attribute that just proxies the method
    >>> # call to the _fwd SortedDict:
    >>> class sortedbidict3(bidict.OrderedMapping, bidict.bidict):
    ...     _fwd_class = _inv_class = sortedcontainers.SortedDict
    ...     __reversed__ = lambda self: reversed(self._fwd)
    >>>
    >>> # Python 3.6:
    >>> issubclass(sortedbidict3, collections.Reversible)  # doctest: +SKIP
    True
    >>> # bidict also provides an OrderedBidirectionalMapping ABC with a
    >>> # __subclasshook__, so with the __reversed__ method implying ordering,
    >>> # the following works automatically too:
    >>> issubclass(sortedbidict3, bidict.OrderedBidirectionalMapping)
    True
