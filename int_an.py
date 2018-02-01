#!/usr/bin/env python3
#coding = utf-8

import pprint
pprint=pprint.PrettyPrinter(indent=4).pprint
def main():
    ints = []
    minmaxes = []

    with open('intersections.log') as f:
        for line in f:
            ints.append([int(line.split()[0]), int(line.split()[1])]);

    with open('coords.log') as f:
        for line in f:
            minmaxes.append(line.split())

    with open('options.ini') as f:
        for line in f:
            if line.startswith('CUBE_EDGE_LENGTH'):
                cubeSize = float(line.split()[1])

    for i in range(len(minmaxes)):
        for j in range(len(minmaxes[i])):
            minmaxes[i][j] = float(minmaxes[i][j])

    chains = []
    for intersection in ints:
        flag1 = False
        flag2 = False
        for chain in chains:
            if intersection[0] in chain:
                chain.update([intersection[1],])
                flag1 = True
            if intersection[1] in chain:
                chain.update([intersection[0],])
                flag2 = True
        if not flag1 and not flag2:
            chains.append(set(intersection))

    chain_minmaxes = [[cubeSize, 0, cubeSize, 0, cubeSize, 0]
                          for i in range(len(chains))]

    for i, chain in enumerate(chains):
        minmax = [cubeSize, 0, cubeSize, 0, cubeSize, 0]
        for pc in chain:
            if minmaxes[pc][0] < minmax[0]:
                minmax[0] = minmaxes[pc][0]
            if minmaxes[pc][1] > minmax[1]:
                minmax[1] = minmaxes[pc][1]
            if minmaxes[pc][2] < minmax[2]:
                minmax[2] = minmaxes[pc][2]
            if minmaxes[pc][3] > minmax[3]:
                minmax[3] = minmaxes[pc][3]
            if minmaxes[pc][4] < minmax[4]:
                minmax[4] = minmaxes[pc][4]
            if minmaxes[pc][5] > minmax[5]:
                minmax[5] = minmaxes[pc][5]
        chain_minmaxes[i] = minmax

    for i, chain in enumerate(chains):
        flagx = flagy = flagz = False
        if chain_minmaxes[i][1] - chain_minmaxes[i][0] > cubeSize:
            flagx = True
        if chain_minmaxes[i][3] - chain_minmaxes[i][2] > cubeSize:
            flagy = True
        if chain_minmaxes[i][5] - chain_minmaxes[i][4] > cubeSize:
            flagz = True
        #if True in [flagx, flagy, flagz]:
        #print(chain, chain_minmaxes[i], flagx, flagy, flagz)
        #print(str(int(flagx)) + str(int(flagy)) + str(int(flagz)))
        print(flagx, flagy, flagz)


main()
