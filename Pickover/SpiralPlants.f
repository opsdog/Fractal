      program SpiralPlantForms
c     page 203
      real*8 rz,iz,rrz,iiz
      real*8 rmin,rmax,imin,imax
      real*8 exittest
      real aspectx, aspecty
      integer i,colour,dwell
      integer width,height
      double complex u,z,a,b,c,d,e,f,g
      common ncolors(6000,6000)
      aspectx=640.0
      aspecty=480.0
c
c Never Completed:  distr=.05, disti=.05, width=height=250
c                   dwell=10000
      centerr=0.5
      centeri=0.5
      distr=0.5
c
      centerr=0.0
      centeri=0.0
      distr=1.5
c
      width=640
      dwell=25000
      exittest=1000.0
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
      u=(0.35,0.35)
      do 10 nr=1,width
         print*,nr
         rrz=rmin+nr*(rmax-rmin)/(width-1.0)
         do 10 ni=1,height
            iiz=imin+ni*(imax-imin)/(height-1.0)
            z=cmplx(rrz,iiz)
            a=(0,0)
            b=(0,0)
            c=(0,0)
            d=(0,0)
            e=(0,0)
            f=(0,0)
            g=(0,0)
c
c     Inner Loop
c            do 20 i=0,dwell
c do while equiv
            i=0
            rz=real(z)
            iz=imag(z)
          do 20 while ((i.le.dwell).and.(sqrt(rz**2+iz**2).le.exittest))
c
               colour=i
c
               a=z
               b=a**2+u
               e=a**2+u
               c=e**2+u
               b=c**2+u
               f=c**2+u
               d=f**2+u
               d=b**2+u
               e=d**2+u
               a=f**2+u
               z=a
c
               rz=real(z)
               iz=imag(z)
c               if (sqrt(rz**2+iz**2).gt.100.0) goto 30
 20            i=i+1
c
 30         ncolors(nr,ni)=colour
c 30         if (colour.eq.dwell) then
c               ncolors(nr,ni)=dwell
c             else
c               ncolors(nr,ni)=colour
c             endif
c
c
 10   continue
c
c     write out the Fractal arrays and dimensions
c
      open (unit=10,file="SpiralPlants-array1")
      print *,'Writing Fractal array values...'
      write(10,*) width, height
      do 60 nr=1,width
         do 60 ni=1,height
            write(10,*) ncolors(nr,ni)
 60   continue
      close(10)
c
      print*,'center at (',centerr,',',centeri,')'
      print*,'dist is (',distr,',',disti,')'
      print*,'width ',width,', height ',height,', dwell ',dwell
      stop
      end

