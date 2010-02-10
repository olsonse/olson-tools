
#ifndef USE_SPENCERS_FAST_POW
      program prog
      end
#else

program bob
     double precision a,b, fast_pow
     double precision val, mi, ma, su
     integer start, finish, rate
     double precision stdpow
     external fast_pow, stdpow

     mi = -100000.0
     ma = 10000.0

     a = 1.001
     b = 3.2
     write (*,*) "fast_pow(", a, ",", b, ") = ", fast_pow(a,b)
     write (*,*) "fast_pow(", 0, ",", 0, ") = ", fast_pow(0d0,0d0)

     su = 0.0
     val = mi
     call system_clock(start,rate)
     do while (val .lt. ma)
         su = su + fast_pow(a, val)
         val = val + 0.01
     end do
     call system_clock(finish)

     write (*,*) "sum is ", su
     write (*,*) "Elapsed time for fast_pow is ", (finish-start)*rate

     su = 0.0
     val = mi
     call system_clock(start)
     do while (val .lt. ma)
         su = su + stdpow(a, val)
         val = val + 0.01
     end do
     call system_clock(finish)

     write (*,*) "sum is ", su
     write (*,*) "Elapsed time for stdpow is ", (finish-start)*rate

     su = 0.0
     val = mi
     call system_clock(start)
     do while (val .lt. ma)
         su = su + a**val
         val = val + 0.01
     end do
     call system_clock(finish)

     write (*,*) "sum is ", su
     write (*,*) "Elapsed time for ** is ", (finish-start)*rate
end program

#endif // USE_SPENCERS_FAST_POW
