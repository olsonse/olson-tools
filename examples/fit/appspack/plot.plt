#!/usr/bin/env gnuplot

set xlabel 'x'
set ylabel 'y'

set hidden3d

splot \
    'func.dat' every 3:3 w l t '', \
    'record' u 1:2:3 w lp ps 3 pt 6 t ''

pause -1;
