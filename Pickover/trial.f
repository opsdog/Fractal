      parameter (nwidth=512,nheight=381,dwell=1500)
c      integer nwidth,nheight
      integer retdisplay,retdraw,retgc,retvals,retwindow
c      nwidth=512
c      nheight=381
      call openwindow(nwidth,nheight,retdisplay,retdraw,retgc,retvals,
     +retwindow)
      call tester(retdisplay,retdraw,retgc,retvals)
      nx=0
      ny=0
      do 5 nnx=0,50
         nx=nnx
         ny=0
      do 10 i=0,64
         call setcolour(i,retdisplay,retgc,retvals)
         call plotpoint(nx,ny,retdisplay,retdraw,retgc)
         nx=nx+1
         ny=ny+1
 10   continue
 5    continue
      call closewindow(retdisplay,retwindow)
      stop
      end
