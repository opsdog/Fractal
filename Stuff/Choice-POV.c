  printf("\n\nCOLORMAP CHOICES:");
  printf("\n\n  0)  Default Colormap");
  printf("\n  1)  Greyscale, Black Background");
  printf("\n  2)  Greyscale, White Background");
  printf("\n  3)  Experimental\n");
  printf("\nChoose a Colormap:  ");
  scanf("%d",&ColorChoice);

  if (ColorChoice) {

/*****************************************************************************/
/*****************************************************************************/
  if (ColorChoice==1) {
  for (i=0,j=0;i<num_entries_colormap;i++,j+=colorinc,TempNewColors++) {
    TempNewColors->pixel=(unsigned long)i;
    TempNewColors->red=(unsigned short)j;
    TempNewColors->green=(unsigned short)j;
    TempNewColors->blue=(unsigned short)j;
    TempNewColors->flags= DoRed | DoGreen | DoBlue;
  }
}  /* ColorChoice 1 */
/*****************************************************************************/
  if (ColorChoice==2) {
  for (i=0,j=65535;i<num_entries_colormap;i++,j-=colorinc,TempNewColors++) {
    TempNewColors->pixel=(unsigned long)i;
    TempNewColors->red=(unsigned short)j;
    TempNewColors->green=(unsigned short)j;
    TempNewColors->blue=(unsigned short)j;
    TempNewColors->flags= DoRed | DoGreen | DoBlue;
  }
}  /* ColorChoice 2 */
/*****************************************************************************/
  if (ColorChoice==3) {

printf("\nStarting point:  Red Green Blue\n");
scanf("%d %d %d",&RedStart,&GreenStart,&BlueStart);
printf("\nEnding point:  Red Green Blue\n");
scanf("%d %d %d",&RedEnd,&GreenEnd,&BlueEnd);

  ColorRedInc=(RedEnd-RedStart)/num_entries_colormap;
  ColorGreenInc=(GreenEnd-GreenStart)/num_entries_colormap;
  ColorBlueInc=(BlueEnd-BlueStart)/num_entries_colormap;

printf("\ninc's:  %d, %d, %d\n",ColorRedInc,ColorGreenInc,ColorBlueInc);

  for (i=0,RedValue=RedStart,GreenValue=GreenStart,BlueValue=BlueStart;
       i<num_entries_colormap;i++,
       RedValue+=ColorRedInc,GreenValue+=ColorGreenInc,BlueValue+=ColorBlueInc,
       TempNewColors++) {
    TempNewColors->pixel=(unsigned long)i;
    TempNewColors->red=(unsigned short)RedValue;
    TempNewColors->green=(unsigned short)GreenValue;
    TempNewColors->blue=(unsigned short)BlueValue;
    TempNewColors->flags= DoRed | DoGreen | DoBlue;
  }
}  /* ColorChoice 3 */
/*****************************************************************************/
/*****************************************************************************/


}  /*if ColorChoice*/
