#!/usr/bin/python
from crypto import *
P = Point(GF(109, 1051), GF(203, 1051), 0, 23);
Q = Point(GF(240, 1051), GF(203, 1051), 0, 23);
S = Point(GF(1, 1051), GF(554, 1051), 0, 23);
print(weil_pairing(5, P, Q, S));
P = Point(GF(5, 883), GF(66, 883), -35, -9);
Q = Point(GF(103, 883), GF(602, 883), -35, -9);
S = Point(GF(1, 883), GF(197, 883), -35, -9);
print(weil_pairing(7, P, Q, S));
P = Point(GF(8, 1009), GF(703, 1009), 37, 0);
Q = Point(GF(49, 1009), GF(20, 1009), 37, 0);
S = Point(GF(0, 1009), GF(0, 1009), 37, 0);
em = weil_pairing(7, P, Q, S);
print(em);
P1 = Point(GF(417, 1009), GF(952, 1009), 37, 0);
Q1 = Point(GF(561, 1009), GF(153, 1009), 37, 0);
S = Point(GF(0, 1009), GF(0, 1009), 37, 0);
em1 = weil_pairing(7, P1, Q1, S);
print(em1);
print("P1 = {0}, 2P = {1}".format(P1, 2*P));
print("Q1 = {0}, 3Q = {1}".format(Q1, 3*Q));
print("em^6 = {0}".format(em**6));
for x in xrange(1, 100):
   print("{0}\t{1}".format( x, x*S));
