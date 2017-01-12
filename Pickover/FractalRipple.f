      program FractalRipple
c     needs a colormap like 1 with lots of contrast
c     page 165
c      parameter(nwidth=1024,nheight=768)
c      parameter(nwidth=640,nheight=480)
c      parameter(nwidth=720,nheight=540)
c      parameter(nwidth=6000,nheight=4500)
      parameter(nwidth=512,nheight=381)
      integer maxpx,maxpy
      common np(6000,6000)
      maxpx=-1
      maxpy=-1
      iter1=1000
      iter2=10000
c 5000x50000 good for 720x540
      iter1=5000
      iter2=50000
c      iter1=75000
c      iter2=1000000
c      print*,'iter1?'
c      read*,iter1
c      print*,'iter2'
c      read*,iter2
      xxmin=-2.0
      xxmax=2.0
      yymin=-2.0
      yymax=2.0
      xinc=nwidth/(xxmax-xxmin)
      yinc=nheight/(yymax-yymin)
      a=2.24
      b=.43
      c=-.65
      d=-2.43
      e=1.0
c     set p array = 0 here
      x=0
      y=0
      z=0
      do 10 j=1,iter1
         print*,j
         do 10 i=1,iter2
            xx=sin(a*y)-z*cos(b*x)
            yy=z*sin(c*x)-cos(d*y)
            zz=e*sin(x)
            x=xx
            y=yy
            z=zz
            if ((x.lt.xxmax).and.(xx.gt.xxmin).and.(yy.lt.yymax).and.
     +           (yy.gt.yymin)) then
               xxx=(xx-xxmin)*xinc
               yyy=(yy-yymin)*yinc
               np(xxx,yyy)=np(xxx,yyy)+1
            endif
 10   continue
c      do 20 i=1,nwidth
c         do 20 j=1,nheight
c 20         np(i,j)=mod(np(i,j),64)-1
c
c
c     write out the Fractal arrays and dimensions
c
      open (unit=10,file="FractalRipple-array1")
      print *,'Writing Fractal array values...'
      write(10,*) nwidth, nheight
      do 60 nr=1,nwidth
         do 60 ni=1,nheight
            write(10,*) np(nr,ni)
 60   continue
      close(10)
c
      stop
      end
