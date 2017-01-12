      program StrangeAttractor
c     needs a colormap like 1 with lots of contrast
c     page 165
c      parameter(nwidth=512)
      parameter(nwidth=640)
c      parameter(nwidth=720)
c      parameter(nwidth=1024)
c      parameter(nwidth=6000)
      integer maxpx,maxpy
      common np(6001,6001)
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
c
c
c      xxmin=-2.0
c      xxmax=2.0
c      yymin=-2.0
c      yymax=2.0
c      xinc=nwidth/(xxmax-xxmin)
c      yinc=nheight/(yymax-yymin)
c
c  baseline
      centerx=0.0
      centery=0.0
      distx=2.0
c
c
      aspectx=640.0
      aspecty=480.0
      nheight=nwidth*aspecty/aspectx
      disty=distx*aspecty/aspectx
      xxmin=centerx-distx
      xxmax=centerx+distx
      yymin=centery-disty
      yymax=centery+disty
      xinc=nwidth/(xxmax-xxmin)
      yinc=nheight/(yymax-yymin)
c
c
      a=2.24
      b=.43
      c=-.65
      d=-2.43
      e=1.0
c     set p array = 0 here
      x=0
      y=0
      z=0
c
      print*,'dimensions:', nwidth, nheight
      print*,'centers   :', centerx, centery
      print*,'distances :', distx, disty
      print*,'x bounds  :', xxmin, xxmax
      print*,'y bounds  :', yymin, yymax
      print*,'increments:', xinc, yinc
c
      do 10 j=1,iter1
c         print*,j
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
               ix=xxx+1
               iy=yyy+1
c               np(xxx,yyy)=np(xxx,yyy)+1
               np(ix,iy)=np(ix,iy)+1
            endif
 10   continue
c      do 20 i=1,nwidth
c         do 20 j=1,nheight
c 20         np(i,j)=mod(np(i,j),64)-1
c
c
c     write out the Fractal arrays and dimensions
c
      open (unit=10,file="Strange-array1")
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
