#define TRUE 1
#define FALSE 0

int x,y,num_entries_colormap;
unsigned int width,height,border_width;
unsigned long background_pixel,GCAttributes;
char choice[80];
int contig,ncolors,nplanes,*planes;

void Initialize();

int ColorRedInc, ColorGreenInc, ColorBlueInc;
int RedValue, GreenValue, BlueValue;
int RedStart,RedEnd,GreenStart,GreenEnd,BlueStart,BlueEnd;
