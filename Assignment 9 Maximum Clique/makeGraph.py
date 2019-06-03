#!/usr/bin/env python3

import argparse
import random

def generateGraph(v, e, m, allowreflexive, mustbesymmetric):
    print(v)
    print(e)

    matrix = [0] * (v*v)
    
    i = 0
    while i < e:
        a = random.randint(0, v-1)
        b = random.randint(0, v-1)
        while a == b and allowreflexive == False:
            b = random.randint(0, v-1)

        while matrix[a * v + b] == 1:
            a = random.randint(0, v-1)
            b = random.randint(0, v-1)
            while a == b and allowreflexive == False:
                b = random.randint(0, v-1)

        matrix[a * v + b] = 1
        if mustbesymmetric:
            matrix[b * v + a] = 1

        d = random.randint(1, m)

        if a == b:
            print(a, b, d)
            i += 1
        elif mustbesymmetric == False:
            print(a, b, d)
            i += 1
        else:
            print(a, b, d)
            print(b, a, d)
            i += 2

p = argparse.ArgumentParser(description="Generates the edge list for a randomly generated graph.")

p.add_argument("-v", "--vertices", metavar="15", dest="v", help="Number of verticies.")
p.add_argument("-e", "--edges", metavar="50", dest="e", help="Number of edges.")
p.add_argument("-m", "--maxweight", metavar="15", dest="m", default="1", help="Max weight of edge (default is 1)")
p.add_argument("-r", "--allowreflexive", action='store_true', dest="allowreflexive", help="Allow reflexive edges (default is no reflexive edges)")
p.add_argument("-s", "--mustbesymmetric", action='store_true', dest="mustbesymmetric", help="Resulting graph must be symmetric (default is not symmetric). Using this option may not use the correct number of edges.")

options = p.parse_args()

if options.v is not None:
    v = int(options.v);

    e = random.randint(v, v*(v-1))
    if options.e is not None:
        e = int(options.e)

        if e < 0:
            e = 0

        if e > v*(v-1):
            e = v*(v-1)

    m = int(options.m)
    if m < 1:
        m = 1

    generateGraph(v, e, m, options.allowreflexive, options.mustbesymmetric)
else:
    print("For help, use the -h flag.")
