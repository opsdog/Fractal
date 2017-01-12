cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c  Potential method
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
      real function XPot(cx,cy,maxiter)
      real cx, cy
      integer maxiter
      real x, x2, y, y2, temp, two2iter
      integer iter, i
      x = cx
      x2 = x * x
      y = cy
      y2 = y * y
      iter = 0
c      do while ((iter.lt.maxiter).and.((x2+y2).lt.5.0e+40))
      do while ((iter.lt.maxiter).and.((x2+y2).lt.1000000.0))
c      do while ((iter.lt.maxiter).and.((x2+y2).lt.5.0))
         y = 2.0 * x * y + cy
         x = x2 - y2 + cx
         x2 = x * x
         y2 = y * y
c         print*,'    in XPot',x2,y2
         iter=iter+1
      end do

c      print*,'dougee'

      two2iter=2.0
      if (iter.lt.maxiter) then
         do 10 i=1,iter
 10         two2iter=two2iter*2.0
c         print*,'    ',iter,two2iter
         XPot=0.5*log(x2+y2)/two2iter
      else
         XPot=0.0;
      endif

c      print*,XPot

      return
      end
