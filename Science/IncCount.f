      program IncCount
      integer NumColorsUsed
      integer i
      integer StepCounter
      real*16 fNumColorsUsed
      real*16 ColorRedInc4
      real*16 RedValue
      real*16 RedStart4, RedEnd4
      real*16 QuarterColors
      real*16 PrevValue
c
c
      NumColorsUsed=8735443
      RedStart4=65534.0
      RedEnd4=0.0
      fNumColorsUsed=NumColorsUsed
      QuarterColors = fNumColorsUsed / 4.0;
      ColorRedInc4 = (RedEnd4-RedStart4)/QuarterColors;
      RedValue = RedStart4
      StepCounter=0
c
c
      print*,'Colors:   ',fNumColorsUsed,NumColorsUsed
      print*,'Colors/4: ',QuarterColors
      print*,'Start/End:',RedStart4,RedEnd4
      print*,'Value:    ',RedValue
      print*,'Inc:      ',ColorRedInc4
      print*,'Counter:  ',StepCounter
c
c
      do 100 i=1,NumColorsUsed/4
        PrevValue=RedValue
        RedValue = RedValue + ColorRedInc4;
        StepCounter = StepCounter + 1
c        print*,'  delta',(PrevValue - RedValue)
c        print*,'  ',RedValue
 100  continue
c
c
      print*
      print*,'Value:    ',RedValue
      print*,'Inc:      ',ColorRedInc4
      print*,'Counter:  ',StepCounter
      stop
      end
