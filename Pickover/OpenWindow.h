#define TRUE 1
#define FALSE 0

static int x,y,num_entries_colormap;
static int colorinc,ColorChoice;
static unsigned int width,height,border_width;
static unsigned long background_pixel,GCAttributes,draw_foreground;
char choice[80];
static Display *Dougee_Display;
static Window Dougee_Window,Parent,to_kill[500];
static Visual *Dougee_Visual;
static XSetWindowAttributes *Dougee_Attributes;
/*  colormap stuff */
static Colormap Dougee_Colormap;
static XColor Dougee_Color,*NewColors,*TempNewColors;
static Status Dougee_Status;
static int Dougee_Screen,Dougee_Depth;
static unsigned long pixels[300];
static GC Dougee_GC,Drawing_GC;/*,Choice_GC_1,Text_GC,Shapes_GC;*/
static XGCValues *Dougee_GCValues;
static Drawable Dougee_Drawable;
static int ColorRedInc,ColorBlueInc,ColorGreenInc;
static int RedValue,BlueValue,GreenValue;
static int RedStart, BlueStart, GreenStart;
static int RedEnd, BlueEnd, GreenEnd;
