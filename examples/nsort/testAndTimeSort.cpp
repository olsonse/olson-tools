#include <iostream>
#include <vector>
#include <iterator>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <time.h>
#include <fstream>

#include <olson-tools/nsort/NSort.h>
#include <olson-tools/nsort/map/_3D.h>
#include <olson-tools/nsort/map/w_type.h>
#include "Particle.h"
#include "partition.h"
#include <olson-tools/Timer.h>

//#define DO_TIMING
#ifndef DO_TIMING
#  define DO_SMALL_TESTS
#endif

int main() {
#ifdef DO_TIMING
    const int N_big = 30000000;
    const int N_big_outer = 1;
#endif
#ifdef DO_SMALL_TESTS
    const int N_little = 10;
#endif
    const int n_types = 2;

    using namespace olson_tools::nsort;
    using namespace olson_tools::nsort::map;

    Particle::list                                 pv;
    std::vector<Particle*>                         ptrv;
    NSort<pivot_ctrs<_1D<0> > >                    ps(2);
    NSort<pivot_ctrs<_1D<0> > >                    ptrs(2);
    NSort<pivot_ctrs<w_type<_1D<0> > > >           ps_w_type(2*n_types);
    NSort<pivot_ctrs<w_type<_1D<0> > > >           ptrs_w_type(2*n_types);
    NSort<pivot_ctrs<_2D<0,1> > >                  ps2D(4);
    NSort<pivot_ctrs<_2D<0,1> > >                  ptrs2D(4);
    NSort<pivot_ctrs<w_type<_2D<0,1> > > >         ps2D_w_type(4*n_types);
    NSort<pivot_ctrs<w_type<_2D<0,1> > > >         ptrs2D_w_type(4*n_types);
    NSort<pivot_ctrs<_3D<0,1,2> > >                ps3D(8);
    NSort<pivot_ctrs<_3D<0,1,2> > >                ptrs3D(8);
    NSort<pivot_ctrs<w_type<_3D<0,1,2> > > >       ps3D_w_type(8*n_types);
    NSort<pivot_ctrs<w_type<_3D<0,1,2> > > >       ptrs3D_w_type(8*n_types);
    NSort<map::type>                               pts(n_types);



#ifdef DO_SMALL_TESTS
    int v[10] = {1, 2, 0, 1, 2, 3, 0, 1, 2, 4};
    std::vector<int> sv(v, v+10);

    std::cout << "sv.size() = " << sv.size() << std::endl;
    std::copy(sv.begin(), sv.end(), std::ostream_iterator<int>(std::cout, ", "));
    std::cout << std::endl;

    NSort<> s(10);
    std::sort(v, v+10);
    //s.sort(v, v+10);
    s.sort(sv.begin(),sv.end());
    

    std::copy(v, v+10, std::ostream_iterator<int>(std::cout, ", "));
    std::cout << std::endl;

    std::copy(sv.begin(), sv.end(), std::ostream_iterator<int>(std::cout, ", "));
    std::cout << std::endl;


    /* test for sorting particles. */
    std::copy(pv.begin(), pv.end(), std::ostream_iterator<Particle>(std::cout, "\n"));

    initPVector(pv,N_little, n_types);
    ps.sort(pv.begin(), pv.end());
    std::cout << "\nsorted by NSort:\n";
    std::cout << "Pivot at " << ps.end(0) << " :  " << pv[ps.end(0)] << std::endl;
    std::copy(pv.begin(), pv.end(), std::ostream_iterator<Particle>(std::cout, "\n"));
    std::cout << std::endl;

    initPVector(pv,N_little, n_types);
    std::sort(pv.begin(), pv.end(), position_comp<0>(0.0));
    std::cout << "\nsorted by std::sort:\n";
    std::copy(pv.begin(), pv.end(), std::ostream_iterator<Particle>(std::cout, "\n"));
    std::cout << std::endl;

    initPVector(pv,N_little, n_types);
    int p3 = partition<0>(pv.begin(), pv.end(), 0.0) - pv.begin();
    std::cout << "\nsorted by partition:\n";
    std::cout << "Pivot at " << p3 << " :  " << pv[p3] << std::endl;
    std::copy(pv.begin(), pv.end(), std::ostream_iterator<Particle>(std::cout, "\n"));
    std::cout << std::endl;

    initPVector(pv,N_little, n_types);
    int p4 = std::partition(pv.begin(), pv.end(), partition_position_pred<Particle,0>()) - pv.begin();
    std::cout << "\nsorted by std::partition:\n";
    std::cout << "Pivot at " << p4 << " :  " << pv[p4] << std::endl;
    std::copy(pv.begin(), pv.end(), std::ostream_iterator<Particle>(std::cout, "\n"));
    std::cout << std::endl;


    /* sorting into n-values. */
    initPVector(pv,N_little, n_types);
    std::sort(pv.begin(), pv.end(), type_comp());
    std::cout << "\nsorted by std::sort(type):\n";
    std::copy(pv.begin(), pv.end(), std::ostream_iterator<Particle>(std::cout, "\n"));
    std::cout << std::endl;

    initPVector(pv, N_little, n_types);
    pts.sort(pv.begin(), pv.end());
    std::cout << "\nsorted by NSort(type):\n";
    std::copy(pv.begin(), pv.end(), std::ostream_iterator<Particle>(std::cout, "\n"));
    std::cout << std::endl;


    if (n_types == 2) {
        initPVector(pv, N_little, n_types);
        std::partition(pv.begin(), pv.end(), partition_type_pred());
        std::cout << "\nsorted by std::partition(type):\n";
        std::copy(pv.begin(), pv.end(), std::ostream_iterator<Particle>(std::cout, "\n"));
        std::cout << std::endl;
    }

    /* Testing sorting in 1-dimensions + type. */
    initPVector(pv,N_little, n_types);
    ps_w_type.sort(pv.begin(), pv.end(), pivot_ctrs<w_type<_1D<0> > >(n_types));
    std::cout << "\nsorted by NSort(w/type):\n";
    std::cout << "Pivot at " << ps_w_type.end(0) << " :  " << pv[ps_w_type.end(0)] << std::endl;
    {
        pivot_ctrs<w_type<_1D<0> > > map(n_types);
        for (unsigned int i = 0; i < pv.size(); i++)
            std::cout << pv[i] << ";    map = " << map.super::operator()(pv[i]) << ", " << map(pv[i]) << '\n';
    }
    std::cout << std::endl;


    /* Testing sorting in 2-dimensions. */
    initPVector(pv,N_little, n_types);
    ps2D.sort(pv.begin(), pv.end());
    std::cout << "\nsorted by NSort(2D):\n";
    std::cout << "Pivot at " << ps2D.end(0) << " :  " << pv[ps2D.end(0)] << std::endl;
    {
        pivot_ctrs<_2D<0,1> > map;
        for (unsigned int i = 0; i < pv.size(); i++) {
            std::cout << pv[i] << ";    map = " << map(pv[i]) << '\n';
        }
    }
    std::cout << std::endl;

    initPVector(pv,N_little, n_types);
    quadpartition<0,1>(pv.begin(), pv.end());
    std::cout << "\nsorted by std::partition(2D):\n";
    {
        pivot_ctrs<_2D<0,1> > map;
        for (unsigned int i = 0; i < pv.size(); i++) {
            std::cout << pv[i] << ";    map = " << map(pv[i]) << '\n';
        }
    }
    std::cout << std::endl;

    /* Testing sorting in 2-dimensions + type. */
    initPVector(pv,N_little, n_types);
    ps2D_w_type.sort(pv.begin(), pv.end(), pivot_ctrs<w_type<_2D<0,1> > >(n_types));
    std::cout << "\nsorted by NSort(2D w/type):\n";
    std::cout << "Pivot at " << ps2D_w_type.end(0) << " :  " << pv[ps2D_w_type.end(0)] << std::endl;
    {
        pivot_ctrs<w_type<_2D<0,1> > > map(n_types);
        for (unsigned int i = 0; i < pv.size(); i++)
            std::cout << pv[i] << ";    map = " << map.super::operator()(pv[i]) << ", " << map(pv[i]) << '\n';
    }
    std::cout << std::endl;





    /* Testing sorting in 3-dimensions. */
    initPVector(pv,N_little, n_types);
    ps3D.sort(pv.begin(), pv.end());
    std::cout << "\nsorted by NSort(3D):\n";
    std::cout << "Pivot at " << ps3D.end(0) << " :  " << pv[ps3D.end(0)] << std::endl;
    {
        pivot_ctrs<_3D<0,1,2> > map;
        for (unsigned int i = 0; i < pv.size(); i++)
            std::cout << pv[i] << ";    map = " << map(pv[i]) << '\n';
    }
    std::cout << std::endl;

    initPVector(pv,N_little, n_types);
    octpartition<0,1,2>(pv.begin(), pv.end());
    std::cout << "\nsorted by std::partition(3D):\n";
    {
        pivot_ctrs<_3D<0,1,2> > map;
        for (unsigned int i = 0; i < pv.size(); i++)
            std::cout << pv[i] << ";    map = " << map(pv[i]) << '\n';
    }
    std::cout << std::endl;



    /* Testing sorting in 3-dimensions + type. */
    initPVector(pv,N_little, n_types);
    ps3D_w_type.sort(pv.begin(), pv.end(), pivot_ctrs<w_type<_3D<0,1,2> > >(n_types));
    std::cout << "\nsorted by NSort(3D w/type):\n";
    std::cout << "Pivot at " << ps3D_w_type.end(0) << " :  " << pv[ps3D_w_type.end(0)] << std::endl;
    {
        pivot_ctrs<w_type<_3D<2,1,0> > > map(n_types);
        for (unsigned int i = 0; i < pv.size(); i++)
            std::cout << pv[i] << ";    map = " << map.super::operator()(pv[i]) << ", " << map(pv[i]) << '\n';
    }
    std::cout << std::endl;

    /* with ptr array */
    initPtrVector(pv,ptrv, N_little, n_types);
    ptrs3D_w_type.sort(ptrv.begin(), ptrv.end(), pivot_ctrs<w_type<_3D<0,1,2> > >(n_types));
    std::cout << "\nsorted by NSort(3D w/type) (ptr):\n";
    std::cout << "Pivot at " << ptrs3D_w_type.end(0) << " :  " << *ptrv[ptrs3D_w_type.end(0)] << std::endl;
    {
        pivot_ctrs<w_type<_3D<0,1,2> > > map(n_types);
        for (unsigned int i = 0; i < ptrv.size(); i++)
            std::cout << *ptrv[i] << ";    map = " << map(*ptrv[i]) << '\n';
    }
    std::cout << std::endl;

#endif

#ifdef DO_TIMING
    std::ofstream fout("timings.dat");

    /* do some timing tests */
#define N_TIMERS 18
    olson_tools::Timer timer[N_TIMERS];

    for (int order = 1; order <= 100000; order *= 2) {
        int N_big_i = N_big / order;
        int N_big_outer_i = N_big_outer * order;

        for (int i = 0; i < N_TIMERS; i++) {
            timer[i].function = olson_tools::Timer::CUMMULATIVE;
            timer[i].zero();
        }

        for (int i = 0; i < N_big_outer_i; i++) {

#if 0
            initPVector(pv, N_big_i, n_types);
            timer[0].start();
            ps.sort(pv.begin(), pv.end());
            timer[0].stop();

            initPVector(pv, N_big_i, n_types);
            timer[1].start();
            //std::sort(pv.begin(), pv.end(), position_comp<0>(0.0));
            timer[1].stop();

            initPVector(pv, N_big_i, n_types);
            timer[2].start();
            partition<0>(pv.begin(), pv.end(), 0.0);
            timer[2].stop();

            initPVector(pv, N_big_i, n_types);
            timer[3].start();
            std::partition(pv.begin(), pv.end(), partition_position_pred<Particle,0>());
            timer[3].stop();



            /* Timing sorting into n-values. */
            initPVector(pv, N_big_i, n_types);
            timer[4].start();
            //std::sort(pv.begin(), pv.end(), type_comp());
            timer[4].stop();


            initPVector(pv, N_big_i, n_types);
            timer[5].start();
            pts.sort(pv.begin(), pv.end());
            timer[5].stop();

            if (n_types == 2) {
                initPVector(pv, N_big_i, n_types);
                timer[6].start();
                std::partition(pv.begin(), pv.end(), partition_type_pred());
                timer[6].stop();
            }

#endif

/* ********** THE FOLLOWING ARE THE TESTS I REALLY HAVE USED ****** */

#if 1
            /* 1D sorts. */
            initPVector(pv, N_big_i, n_types);
            timer[0].start();
            ps.sort(pv.begin(), pv.end());
            timer[0].stop();

            initPVector(pv, N_big_i, n_types);
            timer[1].start();
            std::partition(pv.begin(), pv.end(), partition_position_pred<Particle,0>());
            timer[1].stop();

            initPVector(pv, N_big_i, n_types);
            timer[2].start();
            ps_w_type.sort(pv.begin(), pv.end(), pivot_ctrs<w_type<_1D<0> > >(n_types));
            timer[2].stop();
#endif

#if 1
            /* 2D sorts. */
            initPVector(pv, N_big_i, n_types);
            timer[3].start();
            ps2D.sort(pv.begin(), pv.end());
            timer[3].stop();

            initPVector(pv, N_big_i, n_types);
            timer[4].start();
            quadpartition<0,1>(pv.begin(), pv.end());
            timer[4].stop();

            initPVector(pv, N_big_i, n_types);
            timer[5].start();
            ps2D_w_type.sort(pv.begin(), pv.end(), pivot_ctrs<w_type<_2D<0,1> > >(n_types));
            timer[5].stop();
#endif

#if 1
            /* 3D sorts. */
            initPVector(pv, N_big_i, n_types);
            timer[6].start();
            ps3D.sort(pv.begin(), pv.end());
            timer[6].stop();

            initPVector(pv, N_big_i, n_types);
            timer[7].start();
            octpartition<0,1,2>(pv.begin(), pv.end());
            timer[7].stop();

            initPVector(pv, N_big_i, n_types);
            timer[8].start();
            ps3D_w_type.sort(pv.begin(), pv.end(), pivot_ctrs<w_type<_3D<0,1,2> > >(n_types));
            timer[8].stop();
#endif

#if 1
            /* 1D sorts with pointer array. */
            initPtrVector(pv, ptrv, N_big_i, n_types);
            timer[9].start();
            ptrs.sort(ptrv.begin(), ptrv.end());
            timer[9].stop();

            initPtrVector(pv, ptrv, N_big_i, n_types);
            timer[10].start();
            std::partition(ptrv.begin(), ptrv.end(), partition_position_pred<Particle*,0>());
            timer[10].stop();

            initPtrVector(pv, ptrv, N_big_i, n_types);
            timer[11].start();
            ptrs_w_type.sort(ptrv.begin(), ptrv.end(), pivot_ctrs< w_type<_1D<0> > >(n_types));
            timer[11].stop();
#endif

#if 1
            /* 2D sorts with pointer array. */
            initPtrVector(pv, ptrv, N_big_i, n_types);
            timer[12].start();
            ptrs2D.sort(ptrv.begin(), ptrv.end());
            timer[12].stop();

            initPtrVector(pv, ptrv, N_big_i, n_types);
            timer[13].start();
            quadpartition<0,1>(ptrv.begin(), ptrv.end());
            timer[13].stop();

            initPtrVector(pv, ptrv, N_big_i, n_types);
            timer[14].start();
            ptrs2D_w_type.sort(ptrv.begin(), ptrv.end(), pivot_ctrs< w_type<_2D<0,1> > >(n_types));
            timer[14].stop();
#endif


#if 1
            /* 3D sorts with pointer array. */
            initPtrVector(pv, ptrv, N_big_i, n_types);
            timer[15].start();
            ptrs3D.sort(ptrv.begin(), ptrv.end());
            timer[15].stop();

            initPtrVector(pv, ptrv, N_big_i, n_types);
            timer[16].start();
            octpartition<0,1,2>(ptrv.begin(), ptrv.end());
            timer[16].stop();

            initPtrVector(pv, ptrv, N_big_i, n_types);
            timer[17].start();
            ptrs3D_w_type.sort(ptrv.begin(), ptrv.end(), pivot_ctrs< w_type<_3D<0,1,2> > >(n_types));
            timer[17].stop();
#endif

        }

        fout << order << '\t';
        for (int i = 0; i < N_TIMERS; i++) {
            fout << timer[i] << '\t';
        }
        fout << std::endl;

//      std::cout << "Timer results: \n"
//                << "NSort:  "             << timer[0] << "\n"
//                   "std::sort:  "         << timer[1] << "\n"
//                   "partition:  "         << timer[2] << "\n"
//                   "std::partition:  "    << timer[3] << "\n"
//                   "std::sort(type):  "   << timer[4] << "\n"
//                   "NSort(type):  "       << timer[5] << "\n"
//                   "std::partition(type):  "<< timer[6] << (n_types!=2?"disabled":"") << "\n"
//                   "NSort(2D):  "         << timer[7] << "\n"
//                   "std::partition(2D):  "<< timer[8] << "\n"
//                   "NSort(3D):  "         << timer[9] << "\n"
//                   "std::partition(3D):  "<< timer[10] << "\n"
//                   "NSort(3d w/type):  "  << timer[11] << "\n"
//                << std::endl;
    }

#endif

    return 0;
}
