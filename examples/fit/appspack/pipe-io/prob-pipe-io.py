#!/usr/bin/env python

import sys
from math import sqrt, sin
from numpy import arange

def sinc( params, shift=[0] ):
    rho = 0.0

    parm = [] + params
    for i in range(0,len(parm)):
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
    print 'usage:  ', prog, ' [tag]'
    print ' on stdin :  <size>\n<param0>\n<param1>\n...'
    print ' on stdout:  <size>\n<fval0>\n<fval1>\n...'

def main():
    ''' 1.  load the input params,
        2.  call the merit function,
        3.  and write the result to file
    '''
    if len(sys.argv) > 2:
        usage(sys.argv[0])
        return

    # load params
    n_params = int(sys.stdin.readline())
    params = []
    for i in range(0,n_params):
        params.append( sys.stdin.readline() )
    params = ''.join( params ).strip().split()
    if n_params != len(params):
        sys.stderr.write('Expected number of parameters not found\n')
        return 1;

    # eval merit function
    merit = meritFunction(params)

    # store params and result
    f = open('record','a')
    f.write( '  '.join( params + [str(merit), '\n'] ) )
    f.close()
    

    # write result
    sys.stdout.write( str(merit) + '\n' )

    return 0;

if __name__ == '__main__':
    if len(sys.argv) == 2 and sys.argv[1] == 'save':
        saveMerit()
    else:
        sys.exit( main() )
