#!/usr/bin/env gnuplot

set key outside below

set yrange [-10:10]
set y2range [-10:10]

set y2tics
set y2label 'Gravitational Acceleration (m/s^{2})'

set ylabel 'Field (Arb. units)'

plot \
    'vector-field.dat' u 1:2 w l t 'v[X]', \
        '' u 1:3 w l t 'v[Y]', \
        '' u 1:4 w l t 'v[Z]', \
    'scalar-field.dat' w l t 'scalar', \
    'gravity-force.dat' u 1:4 w l axes x1y2 t 'g'

pause -1;

plot \
     'timing.dat' u 1:2 w l t 'v[X:Z]', \
     '' u 1:3 w l t 'scalar', \
     '' u 1:4 w l t 'g'

pause -1;
reread

