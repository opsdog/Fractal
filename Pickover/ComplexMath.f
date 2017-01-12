      program ComplexMath
      complex a, z, u
      real zr, zi, ur, ui, resultr, resulti
      z=(0.1, 0.2)
      u=(0.3, 0.4)
      zr=real(z)
      zi=imag(z)
      ur=real(u)
      ui=imag(u)
      print *,'z = ',z
      print *,real(z), imag(z)
      print *,'u = ',u
      a=z*z
      resultr = zr*zr - zi*zi
      resulti = zr*zi + zi*zr
      print *,'complex square:  ',a
      print *,'real square:     ',resultr, resulti
      a=z+u
      resultr = zr+ur
      resulti = zi+ui
      print *,'complex sum:  ',a
      print *,'real sum      ',resultr, resulti
      a=z*u
      resultr = zr*ur - zi*ui
      resulti = zr*ui + zi*ur
      print *,'complex mult:  ',a
      print *,'real mult:     ',resultr, resulti
      a=z*z+u
      resultr = zr*zr - zi*zi
      resulti = zr*zi + zi*zr
      resultr = resultr + ur
      resulti = resulti + ui
      print *,'complex ^2+:  ',a
      print *,'real ^2+:     ',resultr, resulti
      stop
      end
