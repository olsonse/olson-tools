program bob
#if defined(NAG)
     use f90_unix
     TYPE(TMS) :: BUFFER
#  define clock times(buffer)
#else
#  define clock mclock8()
#endif
     double precision a,b, fast_pow
     double precision val, mi, ma, su
     double precision start, finish
     external fast_pow, stdpow

     mi = -100000.0
     ma = 10000.0

     a = 1.001
     b = 3.2
     write (*,*) "fast_pow(", a, ",", b, ") = ", fast_pow(a,b)
     write (*,*) "fast_pow(", 0, ",", 0, ") = ", fast_pow(0d0,0d0)

     su = 0.0
     val = mi
     start = clock
     do while (val .lt. ma)
         su = su + fast_pow(a, val)
         val = val + 0.01
     end do
     finish = clock

     write (*,*) "sum is ", su
     write (*,*) "Elapsed time for fast_pow is ", (finish-start)

     su = 0.0
     val = mi
     start = clock
     do while (val .lt. ma)
         su = su + stdpow(a, val)
         val = val + 0.01
     end do
     finish = clock

     write (*,*) "sum is ", su
     write (*,*) "Elapsed time for stdpow is ", (finish-start)

     su = 0.0
     val = mi
     start = clock
     do while (val .lt. ma)
         su = su + a**val
         val = val + 0.01
     end do
     finish = clock

     write (*,*) "sum is ", su
     write (*,*) "Elapsed time for ** is ", (finish-start)
end program
