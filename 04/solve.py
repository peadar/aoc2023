#!/usr/bin/python3
import sys
with open(sys.argv[1]) as f:
    winnerCounts = [ len(frozenset.intersection(*
            (frozenset(int(i) for i in side.split())
                for side in (s.strip() for s in
                [s.strip() for s in l.split(":")][1]
                .split("|")))))
        for l in f ]
print(sum(1 << (isec-1) for isec in winnerCounts if isec != 0))
counts = [ 1 ] * len(winnerCounts)
for i, isec in enumerate(winnerCounts):
    for j in range(i + 1, i+1+isec):
        counts[j]+=counts[i]
print(sum(counts))
