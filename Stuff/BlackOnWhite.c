  Dougee_Colormap=XCreateColormap(Dougee_Display,DefaultRootWindow(Dougee_Display),
				  Dougee_Visual,AllocNone);
  XSync(Dougee_Display,False);

XAllocColorCells(Dougee_Display,Dougee_Colormap,False,(unsigned long *) NULL,
		 (unsigned int) 0, pixels, (unsigned int) num_entries_colormap);

  for (i=0;i<num_entries_colormap;i++) pixels[i]=i;

  NewColors=(XColor *)malloc(num_entries_colormap*sizeof(XColor));
  TempNewColors=(XColor *)malloc(sizeof(XColor));
  TempNewColors=NewColors;
colorinc=65536/num_entries_colormap;
printf("num colors is %d, colorinc is %d\n",num_entries_colormap,colorinc);
printf("\n\nColors set:  ");
  for (i=0,j=65535;i<num_entries_colormap;i++,j-=colorinc,TempNewColors++) {
    TempNewColors->pixel=(unsigned long)i;
    TempNewColors->red=(unsigned short)j;
    TempNewColors->green=(unsigned short)j;
    TempNewColors->blue=(unsigned short)j;
printf("%d ",j);
    TempNewColors->flags= DoRed | DoGreen | DoBlue;
  }
printf("\n\n");

TempNewColors=NewColors;

  XStoreColors(Dougee_Display,Dougee_Colormap,TempNewColors,64);
  XSync(Dougee_Display,False);

  XInstallColormap(Dougee_Display,Dougee_Colormap);
  XSync(Dougee_Display,False);
