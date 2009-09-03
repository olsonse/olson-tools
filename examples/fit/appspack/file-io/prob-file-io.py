#!/usr/bin/env python

import sys
from math import sqrt, sin
from numpy import arange

def sinc( params, shift=[0] ):
    rho = 0.0

    parm = [] + params
    for i in range(0,len(shift)):
        parm[i] = float(parm[i]) + shift[i]

    for p in parm:
        p = float(p) # make sure it is a number
        rho += p * p

    rho = 3*sqrt(rho)

    if rho == 0:
        return 1.0
    else:
        return sin(rho) / rho

def meritFunction( params ):
    return  - sinc(params,[-2, 2]) \
            - 100.8*sinc(params, [4.0, 3]) \
            - 2.4*sinc(params, [0, -3])

def saveMerit():
    d = arange(-5,5,.1)
    f = open('func.dat','w')
    for x in d:
        for y in d:
            m = meritFunction([x,y])
            f.write( '  '.join([str(x), str(y), str(m), '\n']) )
        f.write('\n')
    f.close()

def usage(prog):
    print 'usage:  ', prog, ' <input-file-name> <output-file-name>'

def main():
    ''' 1.  load the input params,
        2.  call the merit function,
        3.  and write the result to file
    '''
    if len(sys.argv) < 3:
        usage(sys.argv[0])
        return

    # load params
    f = open(sys.argv[1])
    n_params = f.readline()
    params = f.readlines()
    f.close()
    params = ''.join( params ).strip().split()
    if int(n_params) != len(params):
        raise 'Expected number of parameters not found'

    # eval merit function
    merit = meritFunction(params)

    # store params and result
    f = open('record','a')
    f.write( '  '.join( params + [str(merit), '\n'] ) )
    f.close()
    

    # write result
    f = open(sys.argv[2],'w')
    f.write( str(merit) )
    f.close()

if __name__ == '__main__':
    if len(sys.argv) == 2 and sys.argv[1] == 'save':
        saveMerit()
    else:
        main()
