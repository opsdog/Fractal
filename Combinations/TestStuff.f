      program TestStuff
      character*256 FileName
      character*16 String1
      character*16 String2
c
c  let's figure out string handling
c
      string1 = 'stringone'
      string2 = 'stringtwo'
      print*,string1,string2
      FileName = string1//string2
      print*,FileName
      i1=index(string1,' ')-1
      i2=index(string2,' ')-1
      FileName=string1(1:i1)//string2(1:i2)
      i3=index(FileName,' ')-1
      print*,FileName(1:i3)
      FileName='dougee'//string1(1:i1)//string2(1:i2)
      i3=index(FileName,' ')-1
      print*,FileName(1:i3)
      stop
      end
