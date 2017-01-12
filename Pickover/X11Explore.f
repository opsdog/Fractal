      program X11Explore
      integer(16) width,height,dwell
      real(16) centerx,centery,distx,disty,xmin,xmax,ymin,ymax
      real(16) lengthx,lengthy
      real(16) x, y, x2, y2, cx, cy, temp
      real(16) incx, incy
      real(16) aspectx, aspecty
c
c      integer*8 width,height,dwell
c      real*16 centerx,centery,distx,disty,xmin,xmax,ymin,ymax
c      real*16 lengthx,lengthy
c      real*16 x, y, x2, y2, cx, cy, temp
c      real*16 incx, incy
c      real*16 aspectx, aspecty
c
c      integer width,height,dwell
c      real centerx,centery,distx,disty,xmin,xmax,ymin,ymax
c      real lengthx,lengthy
c      real x, y, x2, y2, cx, cy, temp
c      real incx, incy
c      real aspectx, aspecty
c
      common mset(6000,6000)
      aspectx=640.0q+0
      aspecty=480.0q+0
      width=640
c      height=480
      dwell=24*1024
c      centerx= -0.777932399999978
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c  this has a nice symmetry at VGA aspect ratio
c    colormap 13
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
       centerx= -0.7779323999999775q+0
       centery=  0.13505550099806810q+0
         distx=  0.0000000000000005q+0
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c  baseline
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c      centerx= -0.675q+0
c      centery=0.025q+0
c      distx=1.425q+0
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c  keep testing this spiral for max depth (least distx)
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c       centerx= -0.777932399999977506q+0
c       centery=  0.135055500998068092q+0
c         distx=  0.000000000000000005q+0
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c  sequence 08
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c      centerx=0.3584q+0
c      centery=0.644q+0
c      distx=0.006q+0
c
c      print*,'width?'
c      read*,width
c      print*,'height?'
c      read*,height
c      print*,'dwell?'
c      read*,dwell
      height=width*aspecty/aspectx
      disty=distx*aspecty/aspectx
      xmin=centerx-distx
      xmax=centerx+distx
      ymin=centery-disty
      ymax=centery+disty
      lengthx=sqrt((xmin**2)+(xmax**2))
      lengthy=sqrt((ymin**2)+(ymax**2))
      incx=(xmax-xmin)/(width-1.0q+0)
      incy=(ymax-ymin)/(height-1.0q+0)
      print*,width,height,dwell
      do 70 nx=1,width
         do 70 ny=1,height
            mset(nx,ny)=0
 70   continue
      do 10 ny=1,height
         print*,ny
c         cy=ymin+ny*(ymax-ymin)/(height-1.)
         cy=ymin+ny*incy
         do 10 nx=1,width
c            cx=xmin+nx*(xmax-xmin)/(width-1.)
            cx=xmin+nx*incx
c            print*, cx, cy
            x=0.0q+0
            y=0.0q+0
            x2=0.0q+0
            y2=0.0q+0
            niter=0
            do while ((niter.lt.dwell).and.((x2+y2).lt.2500000.0q+0))
               temp=x2-y2+cx
               y=2.0q+0*x*y+cy
               x=temp
               x2=x*x
               y2=y*y
               niter=niter+1
            end do
 60            mset(nx,ny)=niter
c 60         if (niter.eq.dwell) then
c               mset(nx,ny)=dwell
c            else
c               mset(nx,ny)=niter
c            endif
 10   continue
c
c     write out the Fractal arrays and dimensions
c
      open (unit=10,file="X11Explore-array1.values")
      print *,'Writing Fractal array values...'
      write(10,*) width, height
      do 80 nr=1,width
         do 80 ni=1,height
            write(10,*) mset(nr,ni)
 80   continue
      close(10)
c
      print*,'length:'
      print*,lengthx,lengthy
      print*,'center:'
      print*,centerx,',',centery
      print*,'dist:'
      print*,distx,',',disty
      print*,'from:'
      print*,xmin,',',ymin
      print*,'to'
      print*,xmax,',',ymax
      print*,'incs:'
      print*,incx, incy
      print*,'width ',width,', height ',height,', dwell ',dwell
      stop
      end
