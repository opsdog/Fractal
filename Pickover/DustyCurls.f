      program DustyCurls
c      parameter(nwidth=576,nheight=450)
c      parameter(nwidth=1024)
c     page 119
c      real*16 testrz, testiz
      real*16 rz,rrz,iz,iiz
      real*16 rmin,rmax,imin,imax
      real*16 test
      real*16 testup2
      real*16 distr,disti
      real*16 aspectx, aspecty, centerr,centeri
      real*16 ir
      real*16 maxinternaltest
      real*16 u1,u2
      integer i,colour,maxcolors, nwidth
      integer nr,ni,dwell,maxdwell
      integer Fractal1(6000,6000)
      integer Fractal2(6000,6000)
c      integer Fractal3(6000,6000)
      integer retdisplay,retdraw,retgc,retvals,retwindow
       complex*16 u,z
c      call openwindow(nwidth,nheight,retdisplay,retdraw,retgc,
c     +retvals,retwindow)
c
c     set this for more resolution?
c
      test=256.0
      test=2000.0
c      print*,'test? '
c      read*,test
      testup2=test*test
      dwell=10*1024
c      print*,'dwell? '
c      read*,dwell
c
c     set aspect ratio
c
      aspectx=640.0
      aspecty=480.0
c
c     get fractal parameters
c
      open (11, file="DustyCurls-input")
      read(11,*) nwidth, test, dwell
      read(11,*) centerr, centeri, distr
      read(11,*) u1, u2
      close(11)
      print *,nwidth, test, dwell
      print *,centerr, centeri, distr
      testup2=test*test
      print *,testup2
c      centerr=0.0
c      centeri=0.0
c      distr=1.5
c      disti=1.0
c
c     calculate height parameters by aspect ratio
c
      disti=distr*aspecty/aspectx
      nheight=nwidth*aspecty/aspectx
c
      rmin=centerr-distr
      rmax=centerr+distr
      imin=centeri-disti
      imax=centeri+disti
c
c      print *,'Zeroing Fractal arrays...'
c      do 50 nr=1,nwidth
c         do 50 ni=1,nheight
c            Fractal1(nr,ni)=0
c            Fractal2(nr,ni)=0
c            Fractal3(nr,ni)=0
c 50   continue
      maxdwell=0
      maxcolors=dwell
c     set from file - override below
      print*,u1, u2
      u=cmplx(u1,u2)
c     reference
c      u=(-0.74,0.11)
c     disconnected islands - use this mostly
c      u=(-0.74,0.25)
c     twisty spirals
c      u=(-0.84,0.35)
c     smooth mandelbrot-like effect
c      u=(-0.74,0.0)
c     tapewom heads on overview - very little detail
c      u=(-2.0,2.0)
c     round - very little detail - good stating point perhaps
c      u=(0.0,0.0)
c     looks like a vein structure
c      u=(0.0,1.0)
c     funky hourglass
c      u=(0.0,1.5)
c     explore around this one
c      u=(0.4,0.0)
c     spiral stars
c      u=(0.4,0.4)
c     might be worth exploring
c      u=(1.2,0.8)
c     might be worth exploring
c      u=(1.6,1.6)
      print *,'Fractaling',nwidth,'by',nheight
      do 10 nr=1,nwidth
         print*,nr
         rrz=rmin+nr*(rmax-rmin)/(nwidth-1)
         do 20 ni=1,nheight
            iiz=imin+ni*(imax-imin)/(nheight-1)
            z=cmplx(rrz,iiz)
            do 30,i=1,dwell
c               z=z*z+u
               z=z*z-u
               rz=real(z)
               iz=imag(z)
               if (sqrt(rz*rz+iz*iz).gt.test) goto 40
 30         continue
c            print *,'         inner loop: normal exit'
 40         if (i.eq.(dwell+1)) then
               colour=dwell
c               print *,'dwell+1'
            else
               colour=i
            endif
            if ( maxdwell < i ) maxdwell=i
c
c     convergence test 1
c
c            if ((rz*rz+iz*iz).gt.testup2) Fractal1(nr,ni)=colour
            Fractal1(nr,ni)=colour
c
c     convergence test 2
            if ((abs(rz).lt.test).and.(abs(ri).lt.test))
     +             Fractal2(nr,ni)=colour
c
c     convergence test 3
c            ir=i*1.000
c            if (((rrz**2+iiz**2).gt.testup2).and.(mod(ir,test).eq.0))
c     +             Fractal3(nr,ni)=colour
 20      continue
 10   continue
      print *,'Max dwell used:  ',maxdwell
c
c     write out the Fractal arrays and dimensions
c
      open (unit=10,file="DustyCurls-array1")
      open (unit=20,file="DustyCurls-array2")
      open (unit=30,file="DustyCurls-array3")
      print *,'Writing Fractal array values...'
      write(10,*) nwidth, nheight
      write(20,*) nwidth, nheight
      write(30,*) nwidth, nheight
      do 60 nr=1,nwidth
         do 60 ni=1,nheight
            write(10,*) Fractal1(nr,ni)
            write(20,*) Fractal2(nr,ni)
c            write(30,*) Fractal3(nr,ni)
 60   continue
      close(10)
      close(20)
      close(30)
      stop
      end
