#!/usr/bin/python3

import sys

def read_graph_from_file(f):
    G = {}
    for l in f:
        l = l.strip().split()
        a = l[0]
        b = l[1]
        if a not in G:
            G[a] = set()
        G[a].add(b)
        if b not in G:
            G[b] = set()
        G[b].add(a)
    return G

if len(sys.argv) < 2:
    G = read_graph_from_file(sys.stdin)
else:
    f = open(sys.argv[1],'r')
    G = read_graph_from_file(f)
    f.close()

print(len(G))
