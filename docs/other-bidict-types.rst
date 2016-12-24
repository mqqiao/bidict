.. _other-bidict-types:

Other ``bidict`` Types
======================

Now that we've covered the basics of
:ref:`bidict.bidict <basic-usage>`,
:ref:`bidict.loosebidict <loosebidict>`,
and other preliminaries,
let's look at the remaining bidict types
and the hierarchy they belong to.

``bidict`` Class Hierarchy
--------------------------

.. hide diagram until it's up-to-date
.. .. image:: _static/class-hierarchy.svg
..     :alt: bidict class hierarchy

At the top of the hierarchy of types that bidict provides is
:class:`bidict.BidirectionalMapping`.
This extends the :class:`collections.abc.Mapping` ABC
with the
:func:`abstractproperty <abc.abstractproperty>`
:attr:`inv <bidict.BidirectionalMapping.inv>`,
as well as a concrete, generic implementation of
:attr:`__inverted__ <bidict.BidirectionalMapping.__inverted__>`.
:class:`BidirectionalMapping <bidict.BidirectionalMapping>` also implements
:attr:`__subclasshook__ <bidict.BidirectionalMapping.__subclasshook__>`,
so that any class providing a conforming API is considered a
:class:`BidirectionalMapping <bidict.BidirectionalMapping>` automatically.

Immediately below :class:`BidirectionalMapping <bidict.BidirectionalMapping>`
is :class:`BidictBase <bidict._common.BidictBase>`,
which contains logic shared by its various subclasses.
Users should typically only use one of the subclasses.

At this point the type hierarchy tree forks into
a mutable branch and an immutable branch.
On the mutable side we have
:class:`bidict.bidict`,
which implements :class:`collections.abc.MutableMapping`,
as well as :class:`bidict.loosebidict`.

.. include:: frozenbidict.rst.inc

.. include:: orderedbidict.rst.inc

.. include:: namedbidict.rst.inc

.. include:: polymorphism.rst.inc

.. include:: extending.rst.inc
