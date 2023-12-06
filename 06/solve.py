#!/usr/bin/python3
import math, sys, functools
def solve(time, distance):
    # -b +/- sqrt(b^2 - 4ac)           (t +/- sqrt(t^2 - 4 * d )) / 2
    # ----------------------   => 
    #          2a
    #
    discriminant = math.sqrt(time * time - 4 * distance)
    low = (time - discriminant) / 2
    high = (time + discriminant) / 2
    return int(math.floor(high) - math.ceil(low) + 1)

def part1():
    with open(sys.argv[1] if len(sys.argv) > 1 else "input.txt") as f:
        times = [ int(i) for i in next(f).split()[1:] ]
        records = [ int(i) for i in next(f).split()[1:] ]
    print(functools.reduce(lambda x, y: x * y,  (solve(time, record) for time, record in zip(times, records))))

def part2():
    with open(sys.argv[1] if len(sys.argv) > 1 else "input.txt") as f:
        time = int("".join(next(f).split()[1:]))
        record = int("".join(next(f).split()[1:]))
    print(solve(time, record))

part1()
part2()
