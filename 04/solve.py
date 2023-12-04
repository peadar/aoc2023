#!/usr/bin/python3

import sys

with open(sys.argv[1]) as f:
    def parseLine(l):
        _, vals = [s.strip() for s in l.split(":")]
        sets = [ frozenset(int(i) for i in side.split())
                for side in (s.strip() for s in vals.split("|")) ]
        return len( frozenset.intersection( *sets ) )
    cards = [ parseLine(datum) for datum in f ]

print(sum(1 << (isec-1) for isec in cards if isec != 0))

counts = [ 1 ] * len(cards)
for i, isec in enumerate(cards):
    for j in range(i + 1, i+1+isec):
        counts[j]+=counts[i]
print(sum(counts))
