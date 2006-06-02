#include "fmacros.h"

! Copyright 2004 Spencer Eugene Olson --- All Rights Reserved
double precision function getMachineEPS ()
    implicit none
    ! For something of this nature, it is ok to store on a common block.
    double precision M_EPS, tmp, setMachineEPSHelper
    common/EPS/M_EPS
    M_EPS = 1.
  4 M_EPS = M_EPS/2.
    tmp = M_EPS + 1.
    IF (setMachineEPSHelper(tmp) .GT. 1.) GOTO 4
    M_EPS = M_EPS*2
    getMachineEPS = M_EPS
end function

!   This function is to ensure that x gets written to system memory at
!   sometime and is not always in a register.
double precision function setMachineEPSHelper (x)
    implicit none
    double precision x,y
    common/EPS2/y
    y = x
    setMachineEPSHelper = y
end function

subroutine killMe ()
    implicit none
    integer pid, getpid
    integer SIGFPE, SIGINT
    parameter(SIGINT=2, SIGFPE=8)

    ! The purpose of this is to make sure that we first give the trapfe stuff
    ! a chance to trap this signal and thus allow the user to attach with
    ! a debugger.  We follow by a real exit in case the SIGFPE signal is not
    ! caught.
    pid = getpid()
    call kill(pid,SIGFPE)

    call exit;
end subroutine

subroutine rk4step (x, n, t, dt, derivs, derivsArgs)
    implicit none
    integer             n
    double precision    x(1:n)
    double precision    t, dt, dth, th
    ! NOTE that this code does NOT work with f95 (NAGWare) IF you compile with
    ! -C=array or -C=call as this does not allow passing arbitrarily sized
    ! vectors through derivsArgs
    double precision    derivsArgs(1:1)
    double precision    x_tmp(1:n)
    double precision    D1(1:n), D2(1:n), D3(1:n), D4(1:n)
    integer             i
    external            derivs
    ! DO NOT TOUCH derivsArgs contents here because it could result in SEGV

    dth = 0.5 * dt
    th = t + dth

    ! find D1
    call derivs(x, t, D1, derivsArgs)

    ! find D2
    do i=1,n
        x_tmp(i)    = x(i)    + dth * D1(i)
    end do
    call derivs(x_tmp, th, D2, derivsArgs)

    ! find D3
    do i=1,n
        x_tmp(i)    = x(i)    + dth * D2(i)
    end do
    call derivs(x_tmp, th, D3, derivsArgs)

    ! and find D4
    do i=1,n
        x_tmp(i)    = x(i)    + dt * D3(i)
    end do
    call derivs(x_tmp, t+dt, D4, derivsArgs)

    ! now weight the sum
    do i=1,n
        x(i) = x(i) + 0.16666666666666666667d0 * dt * ( D1(i) + 2.0*(D2(i) + D3(i)) + D4(i) )
    end do
end subroutine

subroutine rk2step (x, n, t, dt, derivs, derivsArgs)
    implicit none
    integer             n
    double precision    x(1:n)
    double precision    t, dt
    ! NOTE that this code does NOT work with f95 (NAGWare) IF you compile with
    ! -C=array or -C=call as this does not allow passing arbitrarily sized
    ! vectors through derivsArgs
    double precision    derivsArgs(1:1)
    double precision    x_tmp(1:n)
    double precision    D1(1:n), D2(1:n)
    integer             i
    external            derivs
    ! DO NOT TOUCH derivsArgs contents here because it could result in SEGV

    ! find D1
    call derivs(x, t, D1, derivsArgs)

    ! find D2
    do i=1,n
        x_tmp(i)    = x(i)    + dt * D1(i)
    end do
    call derivs(x_tmp, t+dt, D2, derivsArgs)

    ! now weight the sum
    do i=1,n
        x(i) = x(i) + 0.5d0 * dt * ( D1(i) + D2(i) )
    end do
end subroutine

subroutine rk_adapt_driver(x, n, ti, dt, dt_step, derivs, derivsArgs, ERRMAX)
    IMPLICIT NONE
    integer             n
    double precision    x(1:n), x_cal(1:n), dxdt(1:n)
    double precision    ti, t, tf, dt, dt_step, dt_step_next, dt_step_did
    double precision    ERRMAX
    double precision    dir
    double precision    derivsArgs(1:1)
    integer   i, truncated_step
    double precision    TINY
    parameter (TINY=1.e-30)
    external derivs, rkqs

    double precision M_EPS
    common/EPS/M_EPS

    ! ensure that dt and dt_step have the same sign
    dt_step = sign(dt_step,dt)
    dir = sign(1d0,dt)
    dt_step_next = dt_step
    truncated_step = 0
    t = ti
    tf = ti + dt
    do while ((t*dir) .lt. (tf*dir))
        call derivs(x, t, dxdt, derivsArgs)

        do i=1,n
            ! Scaling used to monitor accuracy. This
            ! general-purpose choice can be modified if need be.
            x_cal(i) = abs(x(i)) + abs( dt_step*dxdt(i) ) + TINY
        end do

        if(((t+dt_step)*dir).gt.(tf*dir)) then
             ! If stepsize can overshoot, decrease.
            dt_step = sign(tf-t,dt)

            truncated_step = 1
        else
            truncated_step = 0
        endif

        ! time is accumulated in this function
        call rkqs(x,n,dxdt,t,dt_step,ERRMAX,x_cal,dt_step_did, derivs, derivsArgs)
        ! write (*,'(X,F15.8,1X,F15.8,1X,F15.8,1X,F15.8)') x(1:3), dt_step_did

        if ( truncated_step .eq. 0 .or. abs(dt_step).lt.abs(dt_step_did)) then
            ! don't increase the stepsize beyond the previous corrected value
            ! if we've truncated this last step.  This will likely occur after
            ! EVERY set of dt time steps.
            dt_step_next = dt_step
        endif

        if((t*dir).ge.(tf*dir))then
            ! We are now finished, so return
            dt_step = dt_step_next
            return ! Normal exit.
        endif

        if(abs(dt_step).le.TINY) then
            ! This code should NEVER be reached for a physical guide!
            ! call log_severe('stepsize (%g) smaller than TINY'//char(0), %val(dt_step),0d0,0d0)
            call log_severe('stepsize (%g) smaller than TINY'//char(0), dt_step, 0d0, 0d0)
            call killMe()
        endif
    end do
    call log_severe('This code (in rk_driver_adapt) is supposed to be unreachable' // char(0),0d0,0d0,0d0)
end subroutine

! Modified routine from Numerical Recipies in Fortran
subroutine rkqs(x, n, dxdt, t, dt_try, eps, xscal, dt_did, derivs, derivsArgs)
    implicit none
    integer           n
    double precision  x(1:n), dxdt(1:n), xscal(1:n), xtemp(1:n), xerr(1:n), errmax
    double precision  t, dt_try, eps, dt_did, dt, dt_temp
    external derivs
    double precision    derivsArgs(1:1)
    !    USES derivs,rkck
    ! Fifth-order Runge-Kutta step with monitoring of local truncation error to ensure accuracy
    ! and adjust stepsize.  
    !          INPUT
    ! x(1:n)[t]		dependent variables (at time x)
    ! n			number of elements we are integrating
    ! dxdt(1:n)[t]	derivatives of x(1:n) (at time x)
    ! t			The current time before integration.
    ! dt_try		stepsize to first try
    ! eps		required accuracy
    ! xscal(1:n)	error scaling for each of the dependent variables
    ! derivs		user-supplied subroutine that computes the right-hand side derivatives 
    !          OUTPUT
    ! t			The current time after the integration.
    ! dt_did		actual stepsize accomplished
    ! dt_try		estimated next stepsize to try
    ! x(1:n)[t+dt_did]

    ! NOTE:  I've simplified a lot of stuff that I didn't need for a dt>0 and
    ! time-independent forces.  If dt < 0 is needed and time-dependent forces
    ! are used, care must be taken to fix this.  Compare against the Numerical
    ! Recipies for Fortran routines.
    integer   i
    double precision    tnew
    double precision    SAFETY,PGROW,PSHRNK,ERRCON,TINY
    parameter (SAFETY=0.9,PGROW=-.2,PSHRNK=-.25,ERRCON=1.89e-4,TINY=1.e-30)
    integer ncomp, nredone
#if defined(USE_SPENCERS_FAST_POW)
    double precision    fast_pow
    external  fast_pow
#endif
    common /NCOMPU/ ncomp, nredone
    ! The value ERRCON equals (5/SAFETY)**(1/PGROW), see use below.
    dt=dt_try    ! Set stepsize to the initial trial value.
1   call rkck(x,n,dxdt,t,dt,xtemp,xerr,derivs,derivsArgs) !   Take a step.
    ncomp = ncomp + 1

    errmax=0.0
    !  Evaluate accuracy.
    do i=1,n
        errmax=max(errmax,abs(xerr(i)/xscal(i)))
    end do
    errmax=errmax/eps    ! Scale relative to required tolerance.

    if ( errmax .gt. 1.0 ) then
        ! Truncation error too large, reduce stepsize.
#if !defined(USE_SPENCERS_FAST_POW)
        dt_temp = SAFETY*dt*(errmax**PSHRNK)
#else
        dt_temp = SAFETY*dt*fast_pow(errmax,PSHRNK)
#endif
        dt = sign(max(abs(dt_temp),0.1*abs(dt)),dt)    ! No more than a factor of 10.
        ! now set the absolute time
        tnew = t + dt
        if (tnew .eq. t) then
            call log_severe("stepsize (%g = %g + %g) underflow in rkqs"//char(0), tnew, t, dt)
            !call log_severe("stepsize (%g = %g + %g) underflow in rkqs"//char(0), %val(tnew), %val(t), %val(dt))
            call killMe()
        endif
        nredone = nredone + 1


        goto 1    ! For another try.
    else    ! Step succeeded. Compute size of next step.
        if(errmax .gt. ERRCON)then
#if !defined(USE_SPENCERS_FAST_POW)
            dt_try = SAFETY*dt*(errmax**PGROW)
#else
            dt_try = SAFETY*dt*fast_pow(errmax,PGROW)
#endif
        else    ! No more than a factor of 5 increase.
            dt_try=5.0*dt
        endif
        dt_did=dt
        ! now set the absolute time
        t = t + dt
        do i=1,n
            x(i)=xtemp(i)
        end do
        return
    endif
end subroutine

! Modified routine from Numerical Recipies in Fortran
subroutine rkck(x,n,D1,t,dt,xout,xerr,derivs, derivsArgs)
    implicit none
    integer          n
    double precision x(1:n), xout(1:n), xerr(1:n), xtemp(1:n)
    double precision t, dt
    external derivs
    double precision derivsArgs(1:1)
    !    USES derivs
    ! Given values for n variables x and their derivatives dxdt known at t, use the fifth-order
    ! Cash-Karp Runge-Kutta method to advance the solution over an interval h and return
    ! the incremented variables as yout. Also return an estimate of the local truncation er-
    ! ror in yout using the embedded fourth-order method. The user supplies the subroutine
    ! derivs(x,y,dxdt), which returns derivatives dxdt at x.

    ! NOTE that this code does NOT work with f95 (NAGWare) IF you compile with
    ! -C=array or -C=call as this does not allow passing arbitrarily sized
    ! vectors through derivsArgs
    integer i
    double precision D1(1:n),D2(1:n),D3(1:n),D4(1:n),D5(1:n),D6(1:n),  &
           a2,a3,a4,a5,a6,                                         &
           b21,b31,b32,b41,b42,b43,b51,b52,b53,b54,b61,b62,b63,b64,b65, &
           c1,c3,c4,c6,                                            &
           dc1,dc3,dc4,dc5,dc6


    ! Cash-Karp parameters for embedded Runge-Kutta
    ! a[i] are sub-time step fractions
    ! b[i,j] are the jth weights for computing the ith derivative (D[i])
    ! c[i] are the weights of D[i] for calculating the x[i](t+dt)
    ! dc[i] = c[i] - c*[i]
    parameter (a2=0.2, a3=0.3, a4=0.6, a5=1.0, a6=0.875,                                             &
               b21=.2,                                                                               &
               b31=3./40.,       b32=9./40.,                                                         &
               b41=.3,           b42=-.9,       b43=1.2,                                             &
               b51=-11./54.,     b52=2.5,       b53=-70./27.,    b54=35./27.,                        &
               b61=1631./55296., b62=175./512., b63=575./13824., b64=44275./110592., b65=253./4096., &
               c1=37./378., c3=250./621., c4=125./594., c6=512./1771.,                               &
               dc1=c1 - 2825./27648.,                                                                &
               dc3=c3 - 18575./48384.,                                                               &
               dc4=c4 - 13525./55296.,                                                               &
               dc5=   - 277./14336.,                                                                 &
               dc6=c6 - 0.25 )

    ! D1 is given as input

    ! get D2
    do i=1,n
        xtemp(i) = x(i) + dt*( b21*D1(i) )
    end do
    call derivs(xtemp, t + a2*dt, D2, derivsArgs)

    ! get D3
    do i=1,n
        xtemp(i) = x(i) + dt*( b31*D1(i) + b32*D2(i) )
    end do
    call derivs(xtemp, t + a3*dt, D3, derivsArgs)

    ! get D4
    do i=1,n
        xtemp(i) = x(i) + dt*( b41*D1(i) + b42*D2(i) + b43*D3(i) )
    end do
    call derivs(xtemp, t + a4*dt, D4, derivsArgs)

    ! get D5
    do i=1,n
        xtemp(i) = x(i) + dt*( b51*D1(i) + b52*D2(i) + b53*D3(i) + b54*D4(i) )
    end do
    call derivs(xtemp, t + a5*dt, D5, derivsArgs)

    ! get D6
    do i=1,n
        xtemp(i) = x(i) + dt*( b61*D1(i) + b62*D2(i) + b63*D3(i) + b64*D4(i) + b65*D5(i))
    end do
    call derivs(xtemp, t + a6*dt, D6, derivsArgs)

    ! Accumulate increments with proper weights.
    do i=1,n
        xout(i)  = x(i) + dt*( c1*D1(i) + c3*D3(i) + c4*D4(i) + c6*D6(i))
    end do

    do i=1,n
        ! Estimate error as difference between 5th and embedded 4th order methods
        xerr(i)  = dt*( dc1*D1(i) + dc3*D3(i) + dc4*D4(i) + dc5*D5(i) + dc6*D6(i) )
    end do
    return
end subroutine

