cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c  Traditional method
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
      real function XMSet(cx,cy,maxiter)
      real cx, cy
      integer maxiter
      integer niter
      real x, y, x2, y2, temp
      x=0.0
      y=0.0
      x2=0.0
      y2=0.0
      niter=0
      do while ((niter.lt.maxiter).and.((x2+y2).lt.2500000.0))
         temp=x2-y2+cx
         y=2.0*x*y+cy
         x=temp
         x2=x*x
         y2=y*y
         niter=niter+1
      end do
 60   XMSet=niter
      return
      end
