#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Implementation of the straight skeleton algorithm as described by Felkel and
Obdržálek in their 1998 conference paper Straight skeleton implementation.

Original author:
	Ármin Scipiades

Original repo:
	https://github.com/Botffy/polyskel

Modified by:
	Celestial Phineas, to make it zero-dependency

This modified version can be retrieved here, under LGPL-3.0 (this file only):
	https://github.com/celestialphineas/polyskel

                   GNU LESSER GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007

 Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.


  This version of the GNU Lesser General Public License incorporates
the terms and conditions of version 3 of the GNU General Public
License, supplemented by the additional permissions listed below.

  0. Additional Definitions.

  As used herein, "this License" refers to version 3 of the GNU Lesser
General Public License, and the "GNU GPL" refers to version 3 of the GNU
General Public License.

  "The Library" refers to a covered work governed by this License,
other than an Application or a Combined Work as defined below.

  An "Application" is any work that makes use of an interface provided
by the Library, but which is not otherwise based on the Library.
Defining a subclass of a class defined by the Library is deemed a mode
of using an interface provided by the Library.

  A "Combined Work" is a work produced by combining or linking an
Application with the Library.  The particular version of the Library
with which the Combined Work was made is also called the "Linked
Version".

  The "Minimal Corresponding Source" for a Combined Work means the
Corresponding Source for the Combined Work, excluding any source code
for portions of the Combined Work that, considered in isolation, are
based on the Application, and not on the Linked Version.

  The "Corresponding Application Code" for a Combined Work means the
object code and/or source code for the Application, including any data
and utility programs needed for reproducing the Combined Work from the
Application, but excluding the System Libraries of the Combined Work.

  1. Exception to Section 3 of the GNU GPL.

  You may convey a covered work under sections 3 and 4 of this License
without being bound by section 3 of the GNU GPL.

  2. Conveying Modified Versions.

  If you modify a copy of the Library, and, in your modifications, a
facility refers to a function or data to be supplied by an Application
that uses the facility (other than as an argument passed when the
facility is invoked), then you may convey a copy of the modified
version:

   a) under this License, provided that you make a good faith effort to
   ensure that, in the event an Application does not supply the
   function or data, the facility still operates, and performs
   whatever part of its purpose remains meaningful, or

   b) under the GNU GPL, with none of the additional permissions of
   this License applicable to that copy.

  3. Object Code Incorporating Material from Library Header Files.

  The object code form of an Application may incorporate material from
a header file that is part of the Library.  You may convey such object
code under terms of your choice, provided that, if the incorporated
material is not limited to numerical parameters, data structure
layouts and accessors, or small macros, inline functions and templates
(ten or fewer lines in length), you do both of the following:

   a) Give prominent notice with each copy of the object code that the
   Library is used in it and that the Library and its use are
   covered by this License.

   b) Accompany the object code with a copy of the GNU GPL and this license
   document.

  4. Combined Works.

  You may convey a Combined Work under terms of your choice that,
taken together, effectively do not restrict modification of the
portions of the Library contained in the Combined Work and reverse
engineering for debugging such modifications, if you also do each of
the following:

   a) Give prominent notice with each copy of the Combined Work that
   the Library is used in it and that the Library and its use are
   covered by this License.

   b) Accompany the Combined Work with a copy of the GNU GPL and this license
   document.

   c) For a Combined Work that displays copyright notices during
   execution, include the copyright notice for the Library among
   these notices, as well as a reference directing the user to the
   copies of the GNU GPL and this license document.

   d) Do one of the following:

       0) Convey the Minimal Corresponding Source under the terms of this
       License, and the Corresponding Application Code in a form
       suitable for, and under terms that permit, the user to
       recombine or relink the Application with a modified version of
       the Linked Version to produce a modified Combined Work, in the
       manner specified by section 6 of the GNU GPL for conveying
       Corresponding Source.

       1) Use a suitable shared library mechanism for linking with the
       Library.  A suitable mechanism is one that (a) uses at run time
       a copy of the Library already present on the user's computer
       system, and (b) will operate properly with a modified version
       of the Library that is interface-compatible with the Linked
       Version.

   e) Provide Installation Information, but only if you would otherwise
   be required to provide such information under section 6 of the
   GNU GPL, and only to the extent that such information is
   necessary to install and execute a modified version of the
   Combined Work produced by recombining or relinking the
   Application with a modified version of the Linked Version. (If
   you use option 4d0, the Installation Information must accompany
   the Minimal Corresponding Source and Corresponding Application
   Code. If you use option 4d1, you must provide the Installation
   Information in the manner specified by section 6 of the GNU GPL
   for conveying Corresponding Source.)

  5. Combined Libraries.

  You may place library facilities that are a work based on the
Library side by side in a single library together with other library
facilities that are not Applications and are not covered by this
License, and convey such a combined library under terms of your
choice, if you do both of the following:

   a) Accompany the combined library with a copy of the same work based
   on the Library, uncombined with any other library facilities,
   conveyed under the terms of this License.

   b) Give prominent notice with the combined library that part of it
   is a work based on the Library, and explaining where to find the
   accompanying uncombined form of the same work.

  6. Revised Versions of the GNU Lesser General Public License.

  The Free Software Foundation may publish revised and/or new versions
of the GNU Lesser General Public License from time to time. Such new
versions will be similar in spirit to the present version, but may
differ in detail to address new problems or concerns.

  Each version is given a distinguishing version number. If the
Library as you received it specifies that a certain numbered version
of the GNU Lesser General Public License "or any later version"
applies to it, you have the option of following the terms and
conditions either of that published version or of any later version
published by the Free Software Foundation. If the Library as you
received it does not specify a version number of the GNU Lesser
General Public License, you may choose any version of the GNU Lesser
General Public License ever published by the Free Software Foundation.

  If the Library as you received it specifies that a proxy can decide
whether future versions of the GNU Lesser General Public License shall
apply, that proxy's public statement of acceptance of any version is
permanent authorization for you to choose that version for the
Library.
"""
import heapq, logging, math
from itertools import *
from collections import namedtuple

log = logging.getLogger("__name__")
# log.setLevel(logging.ERROR)

EPSILON = 0.00001

class Debug:
	def __init__(self, image):
		if image is not None:
			self.im = image[0]
			self.draw = image[1]
			self.do = True
		else:
			self.do = False

	def line(self, *args, **kwargs):
		if self.do:
			self.draw.line(*args, **kwargs)

	def rectangle(self, *args, **kwargs):
		if self.do:
			self.draw.rectangle(*args, **kwargs)

	def show(self):
		if self.do:
			self.im.show()


_debug = Debug(None)


def set_debug(image):
	global _debug
	_debug = Debug(image)


def _window(lst):
	prevs, items, nexts = tee(lst, 3)
	prevs = islice(cycle(prevs), len(lst) - 1, None)
	nexts = islice(cycle(nexts), 1, None)
	return zip(prevs, items, nexts)


def _add(*vecs: tuple[float]) -> tuple[float]:
	return ( sum([ vec[0] for vec in vecs ]), sum([ vec[1] for vec in vecs ]) )

def _sub(v1: tuple[float], v2: tuple[float]) -> tuple[float]:
	return ( v1[0] - v2[0], v1[1] - v2[1] )

def _cross(v1: tuple[float], v2: tuple[float]) -> float:
	return v1[0] * v2[1] - v2[0] * v1[1]

def _dot(v1: tuple[float], v2: tuple[float]) -> float:
	return v1[0] * v2[0] + v1[1] * v2[1]

def _norm(v: tuple[float]) -> float:
	x, y = v
	return math.sqrt(x**2 + y**2)

def _equals(pt1: tuple[float], pt2: tuple[float]) -> bool:
	return pt1[0] == pt2[0] and pt1[1] == pt2[1]

def _approximately_equals(pt1: tuple[float], pt2: tuple[float]) -> bool:
	if _equals(pt1, pt2): return True
	if _norm(_sub(pt1, pt2)) <= max(_norm(pt1), _norm(pt2)) * 0.001:
		return True
	return False

def _normalize(pt: tuple[float]) -> tuple[float]:
	norm = _norm(pt)
	if norm == 0: return pt
	return ( pt[0]/norm, pt[1]/norm )

def _normalize_contour(contour: list[tuple[float]]) -> list[tuple[float]]:
	return [ pt for prev, pt, next in _window(contour)
		if not (_equals(pt, next) or \
			_equals(_normalize(_sub(pt, prev)), _normalize(_sub(next, pt)))) ]

def _vector_of(line: tuple[tuple[float]]) -> tuple[float]:
	pt1, pt2 = line
	return _sub(pt2, pt1)

def _times(v: tuple[float], m: float) -> tuple[float]:
	return ( m*v[0], m*v[1] )

def _intersect(l1: tuple[tuple[float]], l2: tuple[tuple[float]], type1: str = 'l', type2: str = 'l'):
	p1, _ = l1; q1, _ = l2
	r = _vector_of(l1); s = _vector_of(l2)
	r_cross_s = _cross(r, s)
	# Parallel
	if r_cross_s == 0: return None
	q1_p1 = _sub(q1, p1)
	q1_p1_cross_r = _cross(q1_p1, r)
	q1_p1_cross_s = _cross(q1_p1, s)
	t = q1_p1_cross_s/r_cross_s
	u = q1_p1_cross_r/r_cross_s
	x = p1[0] + t*r[0]; y = p1[1] + t*r[1]
	if type1 == 's':
		if t < 0 or t > 1: return None
	elif type1 == 'r':
		if t < 0: return None
	if type2 == 's':
		if u < 0 or u > 1: return None
	elif type2 == 'r':
		if u < 0: return None
	return (x, y)


def _distance_to(line: tuple[tuple[float]], pt: tuple[float]) -> float:
	norm = _norm(_vector_of(line))
	if norm == 0: return _norm(_sub(line[0], pt))
	pt1, pt2 = line
	return abs((pt2[0]-pt1[0])*(pt1[1]-pt[1])-(pt1[0]-pt[0])*(pt2[1]-pt1[1]))/norm

def _distance(pt1: tuple[float], pt2: tuple[float]):
	return _norm(_sub(pt1, pt2))

class _SplitEvent(namedtuple("_SplitEvent", ["distance", "intersection_point", "vertex", "opposite_edge"])):
	__slots__ = ()

	def __lt__(self, other):
		return self.distance < other.distance

	def __str__(self):
		return "{} Split event @ {} from {} to {}".format(self.distance, self.intersection_point, self.vertex, self.opposite_edge)

class _EdgeEvent(namedtuple("_EdgeEvent", ["distance", "intersection_point", "vertex_a", "vertex_b"])):
	__slots__ = ()

	def __lt__(self, other):
		return self.distance < other.distance

	def __str__(self):
		return "{} Edge event @ {} between {} and {}".format(self.distance, self.intersection_point, self.vertex_a, self.vertex_b)


_OriginalEdge = namedtuple("_OriginalEdge", ["edge", "bisector_left", "bisector_right"])

Subtree = namedtuple("Subtree", ["source", "height", "sinks"])


class _LAVertex:
	def __init__(self, point: tuple[float], edge_left: tuple[tuple[float]], edge_right: tuple[tuple[float]], direction_vectors=None):
		self.point = point
		self.edge_left = edge_left
		self.edge_right = edge_right
		self.prev = None
		self.next = None
		self.lav = None
		self._valid = True  # TODO this might be handled better. Maybe membership in lav implies validity?

		creator_vectors = (
			_times(_normalize(_vector_of(edge_left)), -1),
			_normalize(_vector_of(edge_right))
		)
		if direction_vectors is None:
			direction_vectors = creator_vectors

		self._is_reflex = (_cross(*direction_vectors)) < 0
		self._bisector = (self.point, _add(self.point, (_times(_add(*creator_vectors), (-1 if self.is_reflex else 1)))))
		log.info("Created vertex %s", self.__repr__())

	@property
	def bisector(self):
		return self._bisector

	@property
	def is_reflex(self):
		return self._is_reflex

	@property
	def original_edges(self):
		return self.lav._slav._original_edges

	def next_event(self):
		events = []
		if self.is_reflex:
			# a reflex vertex may generate a split event
			# split events happen when a vertex hits an opposite edge, splitting the polygon in two.
			log.debug("looking for split candidates for vertex %s", self)
			for edge in self.original_edges:
				if edge.edge is self.edge_left or edge.edge is self.edge_right:
					continue

				log.debug("\tconsidering EDGE %s", edge)

				# a potential b is at the intersection of between our own bisector and the bisector of the
				# angle between the tested edge and any one of our own edges.

				# we choose the "less parallel" edge (in order to exclude a potentially parallel edge)
				leftdot = abs(_dot(_normalize(_vector_of(self.edge_left)), _normalize(_vector_of(edge.edge))))
				rightdot = abs(_dot(_normalize(_vector_of(self.edge_right)), _normalize(_vector_of(edge.edge))))
				selfedge = self.edge_left if leftdot < rightdot else self.edge_right

				i = _intersect(selfedge, edge.edge, 'l', 's')
				if i is not None and not _approximately_equals(i, self.point):
					# locate candidate b
					linvec = _normalize(_sub(self.point, i))
					edvec =  _normalize(_vector_of(edge.edge))
					if _dot(linvec, edvec) < 0:
						edvec = _sub((0, 0), edvec)

					bisecvec = _add(edvec, linvec)
					if _norm(bisecvec) == 0:
						continue
					bisector = (i, _add(i, bisecvec))
					b = _intersect(bisector, self.bisector, 'r', 'r')

					if b is None: continue

					# check eligibility of b
					# a valid b should lie within the area limited by the edge and the bisectors of its two vertices:
					xleft	= _cross(_normalize(_vector_of(edge.bisector_left)), _normalize(_sub(b, edge.bisector_left[0]))) > -EPSILON
					xright	= _cross(_normalize(_vector_of(edge.bisector_right)), _normalize(_sub(b, edge.bisector_right[0]))) < EPSILON
					xedge	= _cross(_normalize(_vector_of(edge.edge)), _normalize(_sub(b, edge.edge[0]))) < EPSILON

					if not (xleft and xright and xedge):
						log.debug("\t\tDiscarded candidate %s (%s-%s-%s)", b, xleft, xright, xedge)
						continue

					log.debug("\t\tFound valid candidate %s", b)
					events.append(_SplitEvent(_distance_to(edge.edge, b), b, self, edge.edge))

		i_prev = _intersect(self.bisector, self.prev.bisector, 'r', 'r')
		i_next = _intersect(self.bisector, self.next.bisector, 'r', 'r')

		if i_prev is not None:
			events.append(_EdgeEvent(_distance_to(self.edge_left, i_prev), i_prev, self.prev, self))
		if i_next is not None:
			events.append(_EdgeEvent(_distance_to(self.edge_right, i_next), i_next, self, self.next))

		if not events:
			return None

		ev = min(events, key=lambda event: _distance(self.point, event.intersection_point))

		log.info("Generated new event for %s: %s", self, ev)
		return ev

	def invalidate(self):
		if self.lav is not None:
			self.lav.invalidate(self)
		else:
			self._valid = False

	@property
	def is_valid(self):
		return self._valid

	def __str__(self):
		return "Vertex ({:.2f};{:.2f})".format(self.point[0], self.point[1])

	def __repr__(self):
		return "Vertex ({}) ({:.2f};{:.2f}), bisector {}, edges {} {}".format("reflex" if self.is_reflex else "convex",
																			  self.point[0], self.point[1], self.bisector,
																			  self.edge_left, self.edge_right)


class _SLAV:
	def __init__(self, polygons, holes):
		contours = [_normalize_contour(polygons)]
		contours.extend([_normalize_contour(hole) for hole in holes])

		self._lavs = [_LAV.from_polygon(contour, self) for contour in contours]

		# store original polygon edges for calculating split events
		self._original_edges = [
			_OriginalEdge((vertex.prev.point, vertex.point), vertex.prev.bisector, vertex.bisector)
			for vertex in chain.from_iterable(self._lavs)
		]

	def __iter__(self):
		for lav in self._lavs:
			yield lav

	def __len__(self):
		return len(self._lavs)

	def empty(self):
		return len(self._lavs) == 0

	def handle_edge_event(self, event):
		sinks = []
		events = []

		lav = event.vertex_a.lav
		if event.vertex_a.prev is event.vertex_b.next:
			log.info("%.2f Peak event at intersection %s from <%s,%s,%s> in %s", event.distance,
					 event.intersection_point, event.vertex_a, event.vertex_b, event.vertex_a.prev, lav)
			self._lavs.remove(lav)
			for vertex in list(lav):
				sinks.append(vertex.point)
				vertex.invalidate()
		else:
			log.info("%.2f Edge event at intersection %s from <%s,%s> in %s", event.distance, event.intersection_point,
					 event.vertex_a, event.vertex_b, lav)
			new_vertex = lav.unify(event.vertex_a, event.vertex_b, event.intersection_point)
			if lav.head in (event.vertex_a, event.vertex_b):
				lav.head = new_vertex
			sinks.extend((event.vertex_a.point, event.vertex_b.point))
			next_event = new_vertex.next_event()
			if next_event is not None:
				events.append(next_event)

		return (Subtree(event.intersection_point, event.distance, sinks), events)

	def handle_split_event(self, event):
		lav = event.vertex.lav
		log.info("%.2f Split event at intersection %s from vertex %s, for edge %s in %s", event.distance,
				 event.intersection_point, event.vertex, event.opposite_edge, lav)

		sinks = [event.vertex.point]
		vertices = []
		x = None  # right vertex
		y = None  # left vertex
		norm = _normalize(_vector_of(event.opposite_edge))
		for v in chain.from_iterable(self._lavs):
			log.debug("%s in %s", v, v.lav)
			if _equals(norm, _normalize(_vector_of(v.edge_left))) and _equals(event.opposite_edge[0], v.edge_left[0]):
				x = v
				y = x.prev
			elif _equals(norm, _normalize(_vector_of(v.edge_right))) and _equals(event.opposite_edge[0], v.edge_right[0]):
				y = v
				x = y.next

			if x:
				xleft	= _cross(_normalize(_vector_of(y.bisector)), _normalize(_sub(event.intersection_point, y.point))) >= -EPSILON
				xright	= _cross(_normalize(_vector_of(x.bisector)), _normalize(_sub(event.intersection_point, x.point))) <= EPSILON

				if xleft and xright:
					break
				else:
					x = None
					y = None

		if x is None:
			log.info("Failed split event %s (equivalent edge event is expected to follow)", event)
			return (None, [])

		v1 = _LAVertex(event.intersection_point, event.vertex.edge_left, event.opposite_edge)
		v2 = _LAVertex(event.intersection_point, event.opposite_edge, event.vertex.edge_right)

		v1.prev = event.vertex.prev
		v1.next = x
		event.vertex.prev.next = v1
		x.prev = v1

		v2.prev = y
		v2.next = event.vertex.next
		event.vertex.next.prev = v2
		y.next = v2

		new_lavs = None
		self._lavs.remove(lav)
		if lav != x.lav:
			# the split event actually merges two lavs
			self._lavs.remove(x.lav)
			new_lavs = [_LAV.from_chain(v1, self)]
		else:
			new_lavs = [_LAV.from_chain(v1, self), _LAV.from_chain(v2, self)]

		for l in new_lavs:
			log.debug(l)
			if len(l) > 2:
				self._lavs.append(l)
				vertices.append(l.head)
			else:
				log.info("LAV %s has collapsed into the line %s--%s", l, l.head.point, l.head.next.point)
				sinks.append(l.head.next.point)
				for v in list(l):
					v.invalidate()

		events = []
		for vertex in vertices:
			next_event = vertex.next_event()
			if next_event is not None:
				events.append(next_event)

		event.vertex.invalidate()
		return (Subtree(event.intersection_point, event.distance, sinks), events)


class _LAV:
	def __init__(self, slav):
		self.head = None
		self._slav = slav
		self._len = 0
		log.debug("Created LAV %s", self)

	@classmethod
	def from_polygon(cls, polygon, slav):
		lav = cls(slav)
		for prev, point, next in _window(polygon):
			lav._len += 1
			vertex = _LAVertex(point, (prev, point), (point, next))
			vertex.lav = lav
			if lav.head is None:
				lav.head = vertex
				vertex.prev = vertex.next = vertex
			else:
				vertex.next = lav.head
				vertex.prev = lav.head.prev
				vertex.prev.next = vertex
				lav.head.prev = vertex
		return lav

	@classmethod
	def from_chain(cls, head, slav):
		lav = cls(slav)
		lav.head = head
		for vertex in lav:
			lav._len += 1
			vertex.lav = lav
		return lav

	def invalidate(self, vertex):
		assert vertex.lav is self, "Tried to invalidate a vertex that's not mine"
		log.debug("Invalidating %s", vertex)
		vertex._valid = False
		if self.head is vertex:
			self.head = self.head.next
		vertex.lav = None

	def unify(self, vertex_a, vertex_b, point):
		replacement = _LAVertex(point, vertex_a.edge_left, vertex_b.edge_right,
								(_normalize(_vector_of(vertex_b.bisector)), _normalize(_vector_of(vertex_a.bisector))))
		replacement.lav = self

		if self.head in [vertex_a, vertex_b]:
			self.head = replacement

		vertex_a.prev.next = replacement
		vertex_b.next.prev = replacement
		replacement.prev = vertex_a.prev
		replacement.next = vertex_b.next

		vertex_a.invalidate()
		vertex_b.invalidate()

		self._len -= 1
		return replacement

	def __str__(self):
		return "LAV {}".format(id(self))

	def __repr__(self):
		return "{} = {}".format(str(self), [vertex for vertex in self])

	def __len__(self):
		return self._len

	def __iter__(self):
		cur = self.head
		while True:
			yield cur
			cur = cur.next
			if cur is self.head:
				return

	def _show(self):
		cur = self.head
		while True:
			print(cur.__repr__())
			cur = cur.next
			if cur is self.head:
				break


class _EventQueue:
	def __init__(self):
		self.__data = []

	def put(self, item):
		if item is not None:
			heapq.heappush(self.__data, item)

	def put_all(self, iterable):
		for item in iterable:
			heapq.heappush(self.__data, item)

	def get(self):
		return heapq.heappop(self.__data)

	def empty(self):
		return len(self.__data) == 0

	def peek(self):
		return self.__data[0]

	def show(self):
		for item in self.__data:
			print(item)

def _merge_sources(skeleton):
	"""
	In highly symmetrical shapes with reflex vertices multiple sources may share the same
	location. This function merges those sources.
	"""
	sources = {}
	to_remove = []
	for i, p in enumerate(skeleton):
		source = tuple(i for i in p.source)
		if source in sources:
			source_index = sources[source]
			# source exists, merge sinks
			for sink in p.sinks:
				if sink not in skeleton[source_index].sinks:
					skeleton[source_index].sinks.append(sink)
			to_remove.append(i)
		else:
			sources[source] = i
	for i in reversed(to_remove):
		skeleton.pop(i)

def skeletonize(polygons, holes=None):
	"""
	Compute the straight skeleton of a polygon.

	The polygon should be given as a list of vertices in counter-clockwise order.
	Holes is a list of the contours of the holes, the vertices of which should be in clockwise order.

	Please note that the y-axis goes downwards as far as polyskel is concerned, so specify your ordering accordingly.

	Returns the straight skeleton as a list of "subtrees", which are in the form of (source, height, sinks),
	where source is the highest points, height is its height, and sinks are the point connected to the source.
	"""
	slav = _SLAV(polygons, holes)
	output = []
	prioque = _EventQueue()

	for lav in slav:
		for vertex in lav:
			prioque.put(vertex.next_event())

	while not (prioque.empty() or slav.empty()):
		i = prioque.get()
		if isinstance(i, _EdgeEvent):
			if not i.vertex_a.is_valid or not i.vertex_b.is_valid:
				log.info("%.2f Discarded outdated edge event %s", i.distance, i)
				continue

			(arc, events) = slav.handle_edge_event(i)
		elif isinstance(i, _SplitEvent):
			if not i.vertex.is_valid:
				log.info("%.2f Discarded outdated split event %s", i.distance, i)
				continue
			(arc, events) = slav.handle_split_event(i)

		prioque.put_all(events)

		if arc is not None:
			output.append(arc)
			for sink in arc.sinks:
				_debug.line((arc.source[0], arc.source[1], sink[0], sink[1]), fill="red")

			_debug.show()
	_merge_sources(output)
	return output
