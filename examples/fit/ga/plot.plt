#!/usr/bin/env gnuplot

set hidden3d

splot \
    'func.dat' every 3:2:10 w l t '', \
    'record' u 1:2:3 w lp ps 3 pt 6 t ''

pause -1;
