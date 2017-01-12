#define TRUE 1
#define FALSE 0

int x,y,num_entries_colormap;
unsigned int width,height,border_width;
unsigned long background_pixel,GCAttributes;
char choice[80];
Display *Dougee_Display;
Window Dougee_Window;
Visual *Dougee_Visual;
XSetWindowAttributes *Dougee_Attributes;
Colormap Dougee_Colormap;
XColor Dougee_Color,*NewColors,*TempNewColors;
Status Dougee_Status;
int contig,ncolors,nplanes,*planes;
int Dougee_Screen,Dougee_Depth;
unsigned long pixels[80];
GC Dougee_GC,Drawing_GC,Choice_GC_1,Text_GC,Shapes_GC;
XGCValues *Dougee_GCValues;
Drawable Dougee_Drawable;

void Initialize();

int ColorRedInc, ColorGreenInc, ColorBlueInc;
int RedValue, GreenValue, BlueValue;
int RedStart,RedEnd,GreenStart,GreenEnd,BlueStart,BlueEnd;
