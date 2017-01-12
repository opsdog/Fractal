      program Merge_XPot_Mandel
c
c  program to merge the fractal values spit out from XPot+Mandel
c
c      parameter(maxfract=6000)
      parameter(maxfract=10000)
      parameter(maxcolors=60000000)
      real*16 PotValue, MandelValue
      real*16 MaxPot, MinPot, MaxMandel, MinMandel
      real maxcolors3, mincolors3
      real*16 scalePot, scaleMandel
      real AdjustColors
      integer*16 colors(maxfract,maxfract,3)
c      integer*16 numcolorsused(2)
      integer*16 maxcolors1, mincolors1
      integer*16 maxcolors2, mincolors2
c      integer colorhist(maxcolors,2)
      integer width, height
      integer nx, ny

      character*256 FileBase
      character*256 FileName
      character*256 InputFile1
      character*256 InputFile2
      character*16 sFrame
      character*16 sAspect
      character*7 sRes
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      maxcolors1 = -1
      maxcolors2 = -1
      maxcolors3 = -1
      mincolors1 = maxcolors
      mincolors2 = maxcolors
      mincolors3 = maxcolors
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c  read the input files and get the width and height
c
      call getarg(1, InputFile1)
      call getarg(2, InputFile2)
      ilen1=index(InputFile1,' ')-1
      ilen2=index(InputFile2,' ')-1
      open (11, file=InputFile1(1:ilen1))
      open (12, file=InputFile2(1:ilen2))
      read(11,*) i1, i2
      read(12,*) i3, i4
      print*,i1, i2, i3, i4
      if ( (i1.ne.i3) .or. (i2.ne.i4) ) then
        print*,'Mismatched input files'
        close(11)
        close(12)
        stop
      endif
      width = i1
      height = i2
c
c  basic sanity check passed 
c  
c  zero out initial values
c
      print*,'Zeroing arrays...'
      do 10 nx=1, width
        do 10 ny=1, height
          colors(nx,ny,1)=0
          colors(nx,ny,2)=0
 10       colors(nx,ny,3)=0
c      do 11 nx=1, maxcolors
c        colorhist(nx,1)=0
c 11     colorhist(nx,2)=0
c
c  read the fractal values and get max/min values
c
      print*,'Finding max and min values...'
      maxPot=0.0
      minPot=10000000000.0
      maxMandel=0.0
      minMandel=10000000000.0
      do 15 nx=1, width
        do 15 ny=1, height
          read(11,*) PotValue
          read(12,*) MandelValue
c          print*,PotValue,MandelValue
          if (PotValue.gt.maxPot) then
            maxPot = PotValue
          endif
          if (PotValue.lt.minPot) then
            minPot = PotValue
          endif
          if (MandelValue.gt.maxMandel) then
            maxMandel = MandelValue
          endif
          if (MandelValue.lt.minMandel) then
            minMandel = MandelValue
          endif
 15   continue
      close(11)
      close(12)
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c  reopen files and scale values
c
      print*,'About to scale colors...'
      open (11, file=InputFile1(1:ilen1))
      open (12, file=InputFile2(1:ilen2))
c
c  discard first line of each file
c
      read(11,*) i1, i2
      read(12,*) i1, i2
c
c  read the rest of the values and scale - add to colors array
c
      scalePot=maxcolors/maxPot
      scaleMandel=maxcolors/maxMandel
      do 20 nx=1, width
        do 20 ny=1, height
          read(11,*) PotValue
          read(12,*) MandelValue
          if ( (PotValue.gt.0.0) .and. (PotValue.lt.1.0) ) then
            colors(nx,ny,1) = PotValue * scalePot
          endif
          colors(nx,ny,2) = MandelValue * scaleMandel
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c  add fractals together with scalePot offset
c
c          if (colors(nx,ny,2) .ne. 0) then
c            colors(nx,ny,3)=colors(nx,ny,1)+colors(nx,ny,2)+
c     +scalePot
c          else
c            colors(nx,ny,3)=colors(nx,ny,1)+colors(nx,ny,2)
c          endif
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c  if potvalue = 0.0 then use mandelvalue no further adjustment
c
          if (PotValue.eq.0.0) then
            colors(nx,ny,3)=colors(nx,ny,2)
          else
            colors(nx,ny,3)=colors(nx,ny,1)
          endif
 20   continue
      close(11)
      close(12)
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c  compute color map stats
      print*,'Computing colormap stats'
      do 30 ny=1,height
        do 30 nx=1,width
          if (colors(nx,ny,1).ge.maxcolors1) then
            maxcolors1 = colors(nx,ny,1)
         endif
         if (colors(nx,ny,2).ge.maxcolors2) then
           maxcolors2 = colors(nx,ny,2)
        endif
        if (colors(nx,ny,3).ge.maxcolors3) then
          maxcolors3 = colors(nx,ny,3)
        endif
        if (colors(nx,ny,1).lt.mincolors1) then
          mincolors1 = colors(nx,ny,1)
        endif
        if (colors(nx,ny,2).lt.mincolors2) then
           mincolors2 = colors(nx,ny,2)
        endif
        if (colors(nx,ny,3).lt.mincolors3) then
           mincolors3 = colors(nx,ny,3)
        endif
c        colorhist(colors(nx,ny,2),2)=colorhist(colors(nx,ny,2),2)+1
c 30     colorhist(colors(nx,ny,1),1)=colorhist(colors(nx,ny,1),1)+1
 30   continue
c      numcolorsused=0
c      do 31 nx=1,maxcolors
c        if (colorhist(nx,1).ne.0) then
c           numcolorsused(1)=numcolorsused(1)+1
c        endif
c        if (colorhist(nx,2).ne.0) then
c           numcolorsused(2)=numcolorsused(2)+1
c        endif
c 31   continue
c
c  scale the colors(3) array to maxcolors
c
      if (maxcolors3.gt.60000000) then
        print*,'  Adjusting to maxcolors'
        AdjustColors=maxcolors/maxcolors3
        do 32 ny=1,height
          do 32 nx=1,width
 32         colors(nx,ny,3)=colors(nx,ny,3)*AdjustColors
      endif

c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      print*,'---------------------------------------------------------'
      print*,'Pot Values:'
      print*,minPot,maxPot
      print*,'Mandel Values:'
      print*,minMandel,maxMandel
      print*,'---------------------------------------------------------'
c      print*,'number of colors =',numcolorsused(1)
      print*,'  ',mincolors1,' to ',maxcolors1
c      print*,'number of colors =',numcolorsused(2)
      print*,'  ',mincolors2,' to ',maxcolors2
      print*,'colors3'
      print*,'  ',mincolors3,' to ',maxcolors3
      print*,'scalePot   =',scalePot
      print*,'scaleMandel=',scaleMandel
      print*,'AdjustColors =',AdjustColors
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c  write out the colors arrays and dimensions
c
      ilen3=index(InputFile1,'.')-2
      FileBase=InputFile1(1:ilen3)
      ilen4=index(FileBase,' ')-1
c
      FileName=FileBase(1:ilen4)//'1.values'
      ilen5=index(FileName,' ')-1
      print*,'Writing '//FileName(1:ilen5)
      open (unit=10,file=FileName(1:ilen5))
      write(10,*) width, height
      do 40 nx=1,width
         do 40 ny=1,height
 40        write(10,*) colors(nx,ny,1)
      close(10)
c
      FileName=FileBase(1:ilen4)//'2.values'
      ilen5=index(FileName,' ')-1
      print*,'Writing '//FileName(1:ilen5)
      open (unit=10,file=FileName(1:ilen5))
      write(10,*) width, height
      do 41 nx=1,width
         do 41 ny=1,height
 41        write(10,*) colors(nx,ny,2)
      close(10)
c
      FileName=FileBase(1:ilen4)//'3.values'
      ilen5=index(FileName,' ')-1
      print*,'Writing '//FileName(1:ilen5)
      open (unit=10,file=FileName(1:ilen5))
      write(10,*) width, height
      do 42 nx=1,width
         do 42 ny=1,height
 42        write(10,*) colors(nx,ny,3)
      close(10)

c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      stop
      end
