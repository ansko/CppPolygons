#!/usr/bin/env python3
#coding=utf-8


import math
import subprocess

REPEATS = 1
CONC_RUNS = 100
STEP = 1000


exf_opts = {'CUBE_EDGE_LENGTH': 100.0,
            'MAX_ATTEMPTS': 1000000.0,
            'VERTICES_NUMBER': 8.0,
            'THICKNESS': 0.1,
            'OUTER_RADIUS': 1.0,
            'SHELL_THICKNESS': 1.0
}


def percolationConcentration():
    h = exf_opts['THICKNESS']
    n = exf_opts['VERTICES_NUMBER']
    r = exf_opts['OUTER_RADIUS'] * math.cos(math.pi / n)
    sh = exf_opts['SHELL_THICKNESS']
    diskVolume = math.pi * r ** 2 * h
    l = exf_opts['CUBE_EDGE_LENGTH']
    f = open('stdout_polygonal.log', 'w')
    log = open('logExec.log', 'w')
    for i in range(1, CONC_RUNS):
        percRates = [0, 0, 0]
        concentration = diskVolume * i * STEP / l**3
        print("%.4f" % concentration, end=" ")
        for r in range(REPEATS):
            ff = open('stdout_int_an.log', 'w')
            fopt = open('options.ini', 'w')
            for key, val in exf_opts.items():
                fopt.write(key + ' ' + str(val) + '\n')
            fopt.write('DISKS_NUM ' + str(i * STEP) + '\n')
            fopt.write('FNAME geofiles/' + str(i) + '_' + str(r) + '.geo\n')
            fopt.close()
            subprocess.call(['./polygonal', ], stdout=f)
            subprocess.call(['./int_an.py', ], stdout=ff)
            ff.close()
            fff = open('stdout_int_an.log', 'r')
            flagx = flagy = flagz = False
            for line in fff:
                if line.split()[0] == 'True':
                    flagx = True
                if line.split()[1] == 'True':
                    flagy = True
                if line.split()[2] == 'True':
                    flagz = True
            if flagx:
                percRates[0] += 1
            if flagy:
                percRates[1] += 1
            if flagz:
                percRates[2] += 1
            log.write(str(i) + '_' + str(r) + ' ')
            log.write(str(flagx) + str(flagy) + str(flagz) + '\n')
        percRates[0] /= REPEATS
        percRates[1] /= REPEATS
        percRates[2] /= REPEATS
        averageRate = (percRates[0] + percRates[1] + percRates[2]) / 3
        print(averageRate)
        

percolationConcentration()
