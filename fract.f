      program julia
      print 700
  700 format( ' input xmin ')
      read *,pmin
      print 710
  710 format( ' input xmax ')
      read *,pmax
      print 720
  720 format( ' input ymin ')
      read *,qmin
      print 730
  730 format( ' input ymax ')
      read *,qmax
      print 735
C
C	Epsilon should be varied from 0 to 0.4 in 0.1 increments
C	(I chose 0.1).
C
  735 format( ' input epsilon ')
      read *,epsi
	open(unit=50,file="fort.50")
	open(unit=11,file="fort.11")
	open(unit=12,file="fort.12")
	open(unit=13,file="fort.13")
	open(unit=14,file="fort.14")
	open(unit=15,file="fort.15")
	open(unit=16,file="fort.16")
	open(unit=17,file="fort.17")
      write(50,*)pmin
      write(50,*)pmax
      write(50,*)qmin
      write(50,*)qmax
      write(50,*)epsi
	close(unit=50)
      a=639.
      b=349.
C     k= number of iterations
      k=35
      do 400 j=1,a
 2    do 100 i=1,b
      deltap=(pmax-pmin)/(a)
      deltaq=(qmax-qmin)/(b)
      p=pmin+float(i)*deltap
      q=qmin+float(j)*deltaq
C       p=.12
C       q=.5
      k=0
      x=0
      y=0
  300 continue
      xx=sin(x)*cosh(y)-epsi*cos(x)*sinh(y)+p
      y=epsi*sin(x)*cosh(y)+cos(x)*sinh(y)+q
      x=xx
      k=k+1
      if(abs(y).gt.50.)go to 200
      if(k.eq.35)go to 200
      go to 300
  200 continue
      if(k.lt.35.and.k.ge.30)write(11,*)i,j
      if(k.lt.30.and.k.ge.25)write(12,*)i,j
      if(k.lt.25.and.k.ge.20)write(13,*)i,j
      if(k.lt.20.and.k.ge.15)write(14,*)i,j
      if(k.lt.15.and.k.ge.10)write(15,*)i,j
      if(k.lt.10.and.k.ge.5)write(16,*)i,j
      if(k.lt.5.and.k.ge.1)write(17,*)i,j
  100 continue
  400 continue
   10 format(a8)
      end
