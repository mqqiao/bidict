``orderedbidict``
-----------------

For those times when your one-to-one mapping must also support
remembering the order in which items were inserted
(à la :class:`collections.OrderedDict`),
:class:`bidict.orderedbidict` and friends have got your back::

    >>> from bidict import orderedbidict
    >>> element_by_symbol = orderedbidict([('H', 'hydrogen'), ('He', 'helium'), ('Li', 'lithium')])
    >>> element_by_symbol.inv
    orderedbidict([('hydrogen', 'H'), ('helium', 'He'), ('lithium', 'Li')])
    >>> first, second, third = element_by_symbol.values()
    >>> first
    'hydrogen'
    >>> second
    'helium'
    >>> third
    'lithium'
    >>> element_by_symbol.inv['beryllium'] = 'Be'
    >>> last = next(reversed(element_by_symbol))
    >>> last
    'Be'

The additional methods of :class:`collections.OrderedDict` are supported too::

    >>> element_by_symbol.popitem(last=True)
    ('Be', 'beryllium')
    >>> element_by_symbol.popitem(last=False)
    ('H', 'hydrogen')
    >>> element_by_symbol['H'] = 'hydrogen'
    >>> element_by_symbol
    orderedbidict([('He', 'helium'), ('Li', 'lithium'), ('H', 'hydrogen')])
    >>> element_by_symbol.move_to_end('Li')  # works on Python < 3.2 too!
    >>> element_by_symbol
    orderedbidict([('He', 'helium'), ('H', 'hydrogen'), ('Li', 'lithium')])
    >>> element_by_symbol.move_to_end('H', last=False)
    >>> element_by_symbol
    orderedbidict([('H', 'hydrogen'), ('He', 'helium'), ('Li', 'lithium')])

As with :class:`collections.OrderedDict`,
equality tests between ordered bidicts are order-sensitive.
Equality tests between ordered bidicts and other
:class:`collections.abc.Mapping` instances
are order-insensitive.

    >>> ob1 = orderedbidict([('one', 1), ('two', 2)])
    >>> ob2 = orderedbidict([('two', 2), ('one', 1)])
    >>> ob1 == ob2
    False
    >>> ob1 == dict(ob2)
    True

Comparing an ordered bidict
with an instance of another ordered mapping type
such as OrderedDict
is order-insensitive::

    >>> from collections import OrderedDict
    >>> ob1 == OrderedDict(ob2)
    True

If you'd like to customize this behavior,
you can override :attr:`bidict.OrderedBidictBase._compare_as_ordered`
in a subclass::

    >>> class MyOrderedBidict(orderedbidict):
    ...    @staticmethod
    ...    def _compare_as_ordered(mapping):
    ...        return hasattr(mapping, '__reversed__')
    >>> m = MyOrderedBidict(ob1)
    >>> m == OrderedDict(ob2)
    False
    >>> m != dict(ob2) if PYPY else m == dict(ob2)  # doctest: +SKIP
    True

Note that with the definition above,
comparing with a dict will be order-sensitive in PyPy but not in CPython,
as on PyPy dicts have a `__reversed__` attribute
while on CPython they don't.

And also as with :class:`collections.OrderedDict`,
updating an existing item preserves its position in the order
(whether changing its key or value)::

    >>> ob1['one'] = 3
    >>> ob1  # order preserved when setting existing key to new value
    orderedbidict([('one', 3), ('two', 2)])
    >>> ob1.forceput('three', 3)
    >>> ob1  # order preserved when setting existing value to new key too
    orderedbidict([('three', 3), ('two', 2)])

When setting an item whose key duplicates that of an existing item
and whose value duplicates that of a different existing item,
the existing item whose value is duplicated will be dropped
and the existing item whose key is duplicated
will have its value overwritten in place::

    >>> o = orderedbidict([(1, 2), (3, 4), (5, 6), (7, 8)])
    >>> o.forceput(3, 8)
    >>> o
    orderedbidict([(1, 2), (3, 8), (5, 6)])
    >>> o = orderedbidict([(1, 2), (3, 4), (5, 6), (7, 8)])
    >>> o.forceput(5, 2)
    >>> o
    orderedbidict([(3, 4), (5, 2), (7, 8)])


:class:`orderedbidict <bidict.orderedbidict>` also comes in
:class:`loose <bidict.looseorderedbidict>` and
:class:`frozen <bidict.frozenorderedbidict>`
flavors.
