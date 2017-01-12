      program XPot_Mandel
c      parameter(maxcolors=500000)
c      parameter(maxcolors=10000000)
c      parameter(maxcolors=50000000)
c      parameter(maxcolors=59990000)
       parameter(maxcolors=60000000)
c      parameter(maxfract=6000)
      parameter(maxfract=10000)
      integer width, height, maxiter
c      integer*16 colors(maxfract,maxfract,3), numcolorsused(2)
c      integer*16 maxcolors1, mincolors1
c      integer*16 maxcolors2, mincolors2
c      integer*16 colorhist(maxcolors,2)
c      real maxcolors3, mincolors3
c      real scalecolor
c      real AdjustColors
      real*16 maxMSet, minMSet
      real*16 centerx, centery, distx, disty
      real*16 xmin, xmax, ymin, ymax
      real*16 incx, incy
      real*16 aspectx, aspecty
      real*16 maxPot, minPot
      real*16 Fractal(maxfract,maxfract,2)
      character*256 FileBase
      character*256 FileName
      character*16 sFrame
      character*16 sAspect
      character*7 sRes
      maxcolors1 = -1
      maxcolors2 = -1
      maxcolors3 = -1
      mincolors1 = maxcolors
      mincolors2 = maxcolors
      mincolors3 = maxcolors
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c  set the aspect ratio
c
      print*,'Setting aspect ratio'
c
c    VGA
c
      aspectx=640.0
      aspecty=480.0
      sAspect='-VGA'
c
c  35mm
c
c      aspectx=3.0
c      aspecty=2.0
c      sAspect='-35mm'
c
c  16:9
c
c      aspectx=16.0
c      aspecty=9.0
c      sAspect='-16x9'
c
c  NTSC
c
c      aspectx=4.0
c      aspecty=3.0
c      sAspect='-NTSC'
c
c  square
c
c      aspectx=1.0
c      aspecty=1.0
c      sAspect='-Square'
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      print*,'Setting fractal parameters'
c
c  wide overview
c      centerx=0.0
c      centery=0.0
c      distx=2.0
c      sFrame='WideOverview'
c  baseline
c    cmap 2, 4, 9, 11, 13
      centerx= -0.675
      centery=0.025
      distx=1.425
      sFrame='Baseline'
c  ColorTest01 - more than MAXCOLORS
c    cmap 2, 4, 9, 11, 13
c      centerx= 0.025
c      centery= 0.725
c      distx= 0.8
c      sFrame='ColorTest01'
c      distx= 1.0
c      distx= 1.425
c  sequence 13, step 11 - good test 
c    cmap 5, 11, 2, 20
c      centerx= -0.780
c      centery= 0.180
c      distx=0.250
c      sFrame='Seq13-11'
c  sequence 6, step 8 - nice
c    cmap 2, 
c      centerx= -1.1225
c      centery= 0.205
c      distx=0.275
c      sFrame='Seq06-08'
c  sequence 6, step 11 - not bad
c      centerx= -0.780
c      centery= 0.180
c      distx=0.250
c      sFrame='Seq06-11'
c  sequence 8, step 1 - not bad
c      centerx=0.3584
c      centery=0.644
c      distx=0.006
c      sFrame='Seq08-01'
c  sequence 11, step 1 - spiky radio wave
c      centerx= -1.4578
c      centery=0.0
c      distx=0.04
c      sFrame='Seq11-01'
c  sequence 9, step 1 - 1 color potential method
c      centerx= -1.48
c      centery=0.0
c      distx=0.001
c      sFrame='Seq09-01'
c  sequence 5, step 3 - kinda cool
c      centerx= -0.7788324
c      centery=0.135055500998
c      distx=0.007
c      sFrame='Seq05-03'
c  still 01 - needs more precision for full depth
c      centerx= -0.777932529
c      centery=0.13505703
c      distx=0.0000175
c      sFrame='Still01'
c  still 02 - nevermind
c      centerx= -0.7499365
c      centery= 0.009840
c      distx=0.010
c      sFrame='Still02'
c  still 03 - nevermind
c      centerx= -0.7499365
c      centery= 0.009838
c      distx=0.010
c      sFrame='Still03'
c  sequence 10, step 3 - not bad
c      centerx=0.36720359945
c      centery=0.5922317790
c      distx=0.25
c      sFrame='Seq10-03'
c  sequence 10, step 6 - more pot colors than normal ?
c      centerx=0.36720359945
c      centery=0.5922317790
c      distx=0.0005
c      sFrame='Seq10-06'
c  sequence 12, step 2 - more pot colors than normal ?
c      centerx= -1.8323762603103880
c      centery=0.0014286451041698
c      distx=0.00002
c      sFrame='Seq12-02'
c  sequence 13, step 2 - very nice
      centerx= -1.6523698334092280
      centery=0.0014357434457388
      distx=0.5
      sFrame='Seq13-02'
c  sequence 13, step 3 - nevermind
c      centerx= -1.8323698334092280
c      centery=0.0014357434457388
c      distx=0.02
c      sFrame='Seq13-03'
c  sequence 4, step 2 - not bad
c      centerx= -1.2334552407264710
c      centery= 0.3167717270553112
c      distx=0.1
c      sFrame='Seq04-02'
c  sequence 4, step 3 - has potential
c      centerx= -1.2334552407264710
c      centery= 0.3167717270553112
c      distx=0.0008
c      sFrame='Seq04-03'
c  sequence 15, step 2 - no detail in mandelbrot
c      centerx= -1.99638
c      centery=0.0
c      distx=0.05
c      sFrame='Seq15-02'
c  sequence 2, step 2 - has potential
c      centerx=0.2914061419
c      centery=0.0149788696
c      distx=0.00140
c      sFrame='Seq02-02'
c  still04 - new
c      centerx=0.2914061419
c      centery=0.0149788696
c      distx=0.00095
c      sFrame='Still04'
c  still05 (mandel) - new
c      centerx=0.2917761419
c      centery=0.0148588696
c        distx=0.00055
c      sFrame='Still05'
c  still06 (mandel) - new
c      centerx=0.2916361419
c      centery=0.0149588696
c        distx=0.00040
c      sFrame='Still06'
c  still07 - new
c      centerx=0.2909561419
c      centery=0.0155088696
c        distx=0.00055
c      sFrame='Still07'
c  still08 - new
c      centerx=0.2908061419
c      centery=0.0155088696
c        distx=0.00055
c      sFrame='Still08'
c  still09 - new
c      centerx=0.2909061419
c      centery=0.0155688696
c        distx=0.00020
c      sFrame='Still09'
c  still10 - new
c      centerx=0.2908961419
c      centery=0.0155818696
c        distx=0.00014
c      sFrame='Still10'
c  still11 - new
c      centerx=0.2906255419
c      centery=0.0151058696
c        distx=0.00010
c      sFrame='Still11'
c  still12 - new
c      centerx=0.2906247419
c      centery=0.0151058696
c        distx=0.00008
c      sFrame='Still12'
c  still13 - new
c      centerx=0.2906224419
c      centery=0.0151058696
c        distx=0.00003
c      sFrame='Still13'
c  still14 - new
c      centerx=0.2916855419
c      centery=0.0145658696
c        distx=0.00110
c      sFrame='Still14'
c  still15 - SQUARE
c      centerx=0.2916105419
c      centery=0.0146158696
c        distx=0.00010
c      sFrame='Still15'
c  still16 - SQUARE
c      centerx=0.2916105419
c      centery=0.0146158696
c      distx=0.00015
c      sFrame='Still16'
c  still 17
c      centerx= -1.3306
c      centery= -0.084
c      distx=0.1
c      sFrame='Still17'
c  sequence 1, step 3 - nevermind
c      centerx= -0.7499225
c      centery=0.0169325
c      distx=0.1056
c      sFrame='Seq01-03'
c  sequence 14, step 7 - nevermind
c      centerx= -0.74996934950
c      centery=0.0169623455
c      distx=0.01056
c      sFrame='Seq14-07'
c  sequence 16, step 2 - has potential
c      centerx= -0.117824
c      centery=0.838990
c      distx=0.160
c      sFrame='Seq16-02'
c  sequence 3, step 2 - needs more precision - quick compute
c      centerx= -1.7116387198
c      centery=0.0004497826
c      distx=0.00014124
c      sFrame='Seq03-02'
c      distx=0.000014124
c  sequence 11, step 4
c    huge scalecolors makes for 14558733518940143616 color3's
c      centerx= -1.4578
c      centery=0.0
c      distx=0.0002
c      sFrame='Seq11-04'
c  sequence 7, step 14 - no potential colors
c      centerx= -0.7499406
c      centery= 0.009840
c      distx=0.000015
c      sFrame='Seq07-14'
c  sequence 13, step 14 - no potential colors
c      centerx= -0.7499600
c      centery= 0.009846
c      distx=0.000040
c      sFrame='Seq13-14'
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      print*,'Setting image parameters'
c
c  set up image parameters
c
c      width=204
c      maxiter=24*1024
c      sRes='-204'
c
c      width=575
c      maxiter=48*1024
c
c      width=640
c      maxiter=48*1024
c      sRes='-640'
c  NTSC
c      width=720
c      maxiter=64*1024
c      sRes='-720'
c
c      width=1024
c      maxiter=84*1024
c      sRes='-1024'
c
c      width=2048
c      maxiter=96*1024
c      sRes='-2048'
c
c      width=4096
c      maxiter=100*1024
c      sRes='-4096'
c
      width=6000
      maxiter=128*1024
      sRes='-6000'
c
c      width=10000
c      maxiter=256*1024
c      sRes='-10000'
c  16:9 HDTV
c      width=1920
c      maxiter=96*1024
c      sRes='-1920'
c
      height=width*aspecty/aspectx
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c  compute fractal parameters
c
      disty=distx*aspecty/aspectx
      xmin=centerx-distx
      xmax=centerx+distx
      ymin=centery-disty
      ymax=centery+disty
      incx=(xmax-xmin)/(width-1.0)
      incy=(ymax-ymin)/(height-1.0)
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      print*,'Zeroing arrays'
c
c  zero out stuff
c
      do 70 nx=1,width
         do 70 ny=1,height
c            colors(nx,ny,1)=0
c            colors(nx,ny,2)=0
            Fractal(nx,ny,1)=0.0
 70         Fractal(nx,ny,2)=0.0
c
c      do 90 nx=1,maxcolors
c         colorhist(nx,1)=0
c 90      colorhist(nx,2)=0
c
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      print*,'Preparing to fractal',width,'X',height
c
c  compute potential method
c  if result is 0.0
c    compute traditional method
c
      maxPot=0.0
      minPot=10000000000.0
      do 10 ny=1,height
         print*,ny
         cy=ymin+ny*incy
         do 10 nx=1,width
            cx=xmin+nx*incx
            Fractal(nx,ny,1)=XPot(cx,cy,maxiter)
            if (Fractal(nx,ny,1).gt.maxPot) then
               maxPot=Fractal(nx,ny,1)
            endif
            if (Fractal(nx,ny,1).lt.minPot) then
               minPot=Fractal(nx,ny,1)
            endif
 10   continue
c
c  for Fracatl(nx,ny) = 0, compute traditional mandelbrot values
c
c      goto 1000
      do 101 ny=1,height
         print*,ny
         cy=ymin+ny*incy
         do 101 nx=1,width
            cx=xmin+nx*incx
            if (Fractal(nx,ny,1).eq.0.0) then
               Fractal(nx,ny,2)=XMSet(cx,cy,maxiter)
               if (Fractal(nx,ny,2).gt.maxMSet) then
                  maxMSet=Fractal(nx,ny,2)
               endif
               if (Fractal(nx,ny,2).lt.minMSet) then
                  minMSet=Fractal(nx,ny,2)
               endif
            endif
 101  continue
 1000 continue
c
c  scale the colors from the potential method
c
c      print*,'about to scale colors...'
c      scalecolor=maxcolors/maxPot
c
c      do 20 nx=1,width
c        do 20 ny=1,height
c          if ((Fractal(nx,ny,1).gt.0.0).and.(Fractal(nx,ny,1).lt.1.0)) 
c     +    then
c            colors(nx,ny,1)=Fractal(nx,ny,1)*scalecolor
c          endif
c          colors(nx,ny,2)=Fractal(nx,ny,2)
c          if (colors(nx,ny,2).ne.0) then
c            colors(nx,ny,3)=colors(nx,ny,1)+colors(nx,ny,2)+
c     +scalecolor
c          else
c            colors(nx,ny,3)=colors(nx,ny,1)+colors(nx,ny,2)
c          endif
c 20    continue
c
c  compute colormap stats
c
c      print*,'computing colormap stats'
c      do 102 nx=1,width
c         do 102 ny=1,height
c            if (colors(nx,ny,1).ge.maxcolors1) then
c               maxcolors1 = colors(nx,ny,1)
c            endif
c            if (colors(nx,ny,2).ge.maxcolors2) then
c               maxcolors2 = colors(nx,ny,2)
c            endif
c            if (colors(nx,ny,3).ge.maxcolors3) then
c               maxcolors3 = colors(nx,ny,3)
c            endif
c            if (colors(nx,ny,1).lt.mincolors1) then
c               mincolors1 = colors(nx,ny,1)
c            endif
c            if (colors(nx,ny,2).lt.mincolors2) then
c               mincolors2 = colors(nx,ny,2)
c            endif
c            if (colors(nx,ny,3).lt.mincolors3) then
c               mincolors3 = colors(nx,ny,3)
c            endif
c            colorhist(colors(nx,ny,2),2)=colorhist(colors(nx,ny,2),2)+1
c 102        colorhist(colors(nx,ny,1),1)=colorhist(colors(nx,ny,1),1)+1
c      numcolorsused=0
c      do 30 nx=1,maxcolors
c         if (colorhist(nx,1).ne.0) then
c            numcolorsused(1)=numcolorsused(1)+1
c         endif
c         if (colorhist(nx,2).ne.0) then
c            numcolorsused(2)=numcolorsused(2)+1
c         endif
c 30   continue
c
c  scale the colors(3) array to maxcolors
c
c      if (maxcolors3.gt.60000000) then
c        print*,'adjusting to maxcolors'
c        AdjustColors=maxcolors/maxcolors3
c        do 113 nx=1,width
c          do 113 ny=1,height
c 113        colors(nx,ny,3)=colors(nx,ny,3)*AdjustColors
c      endif
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c  print out parameters
c
c      print*,'length:'
c      print*,lengthx,lengthy
      print*,'width ',width,', height ',height,', dwell ',maxiter
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
      print*,'===================================================='
      print*,'potential values'
      print*,minPot
      print*,'  to'
      print*,maxPot
      print*,'MSet values'
      print*,minMSet
      print*,'  to'
      print*,maxMSet
c      print*,'===================================================='
c      print*,'number of colors =',numcolorsused(1)
c      print*,'  ',mincolors1,' to ',maxcolors1
c      print*,'number of colors =',numcolorsused(2)
c      print*,'  ',mincolors2,' to ',maxcolors2
c      print*,'colors3'
c      print*,'  ',mincolors3,' to ',maxcolors3
c      print*,'scalecolor   =',scalecolor
c      print*,'AdjustColors =',AdjustColors
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c  write out the fractal arrays and dimensions
c

      i1=index(sFrame,' ')-1
      i2=index(sRes,' ')-1
      i3=index(sAspect,' ')-1
      FileBase='XPot+Mandel-'//sFrame(1:i1)//sRes(1:i2)//sAspect(1:i3)
      i3=index(FileBase,' ')-1
c
c      FileName=FileBase(1:i3)//'-Colors-1.values'
c      i4=index(FileName,' ')-1
c      print*,'Writing '//FileName(1:i4)
c      open (unit=10,file=FileName(1:i4))
c      write(10,*) width, height
c      do 100 nx=1,width
c         do 100 ny=1,height
c 100        write(10,*) colors(nx,ny,1)
c      close(10)
c
c      FileName=FileBase(1:i3)//'-Colors-2.values'
c      i4=index(FileName,' ')-1
c      print*,'Writing '//FileName(1:i4)
c      open (unit=10,file=FileName(1:i4))
c      write(10,*) width, height
c      do 110 nx=1,width
c         do 110 ny=1,height
c 110        write(10,*) colors(nx,ny,2)
c      close(10)
c
c      FileName=FileBase(1:i3)//'-Colors-3.values'
c      i4=index(FileName,' ')-1
c      print*,'Writing '//FileName(1:i4)
c      open (unit=10,file=FileName(1:i4))
c      write(10,*) width, height
c      do 112 nx=1,width
c        do 112 ny=1,height
c 112      write(10,*) colors(nx,ny,3)
c      close(10)
c
      FileName=FileBase(1:i3)//'-Fractal-1.fractal'
      i4=index(FileName,' ')-1
      print*,'Writing '//FileName(1:i4)
      open (unit=10,file=FileName(1:i4))
      write(10,*) width, height
      do 114 nx=1,width
        do 114 ny=1,height
 114      write(10,*) Fractal(nx,ny,1)
      close(10)
c
      FileName=FileBase(1:i3)//'-Fractal-2.fractal'
      i4=index(FileName,' ')-1
      print*,'Writing '//FileName(1:i4)
      open (unit=10,file=FileName(1:i4))
      write(10,*) width, height
      do 115 nx=1,width
        do 115 ny=1,height
 115      write(10,*) Fractal(nx,ny,2)
      close(10)
c
      stop
      end
