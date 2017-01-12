      program ComplexIterationMapGenerator
c     page 104
      real rz,iz,rrz,iiz
      real rmin,rmax,imin,imax
      real exittest, aspectx, aspecty
      integer i,colour
      integer width,height
      complex u,z
      common ncolors(6000,6000)
      aspectx=640.0
      aspecty=480.0
c
c      centerr=0.2415
      centerr=0.2435
c      centeri=1.0037
      centeri=1.0042
      distr=1.5
      width=576
      dwell=20000
      exittest=10000.0
c
c     calculate height parameters by aspect ratio
c
      disti=distr*aspecty/aspectx
      height=width*aspecty/aspectx
c
      rmin=centerr-distr
      rmax=centerr+distr
      imin=centeri-disti
      imax=centeri+disti
      lengthr=sqrt((rmin**2)+(rmax**2))
      lengthi=sqrt((imin**2)+(imax**2))
c
      u=(0.5,0.0)
c
c     zero colors array
c
      do 11 nr=1,width
         do 11 ni=1,height
            ncolors(nr,ni)=0
 11   continue
      print *,'Fractalling ',width,' by ',height
      do 10 nr=1,width
         print*,nr
         rrz=rmin+nr*(rmax-rmin)/(width-1.0)
         do 10 ni=1,height
            iiz=imin+ni*(imax-imin)/(height-1.0)
            z=cmplx(rrz,iiz)
c
c     Inner Loop
            do 20 i=0,dwell
               colour=i
               z=z**2+u
               rz=real(z)
               iz=imag(z)
               if (sqrt(rz**2+iz**2).gt.exittest) goto 30
 20         continue
 30         if (colour.eq.dwell) then
               ncolors(nr,ni)=dwell
             else
               ncolors(nr,ni)=colour
             endif
 10   continue
      print*,'center at (',centerr,',',centeri,')'
      print*,'dist is (',distr,',',disti,')'
      print*,'width ',width,', height ',height,', dwell ',dwell
c
c     write out the Fractal arrays and dimensions
c
      open (unit=10,file="ComplexMap-array1")
      print *,'Writing Fractal array values...'
      write(10,*) width, height
      do 60 nr=1,width
         do 60 ni=1,height
            write(10,*) ncolors(nr,ni)
 60   continue
      close(10)
      stop
      end

