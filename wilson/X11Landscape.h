#define TRUE 1
#define FALSE 0

struct Dougee_Segment {
  short x1,y1,x2,y2;
  struct Dougee_Segment *next;
};

struct Dougee_Point {
  short x,y;
  struct Dougee_Point *next;
};

struct Dougee_Rect {
  short x,y;
  unsigned short width,height;
  int filled;
  struct Dougee_Rect *next;
};

struct Dougee_Arc {
  short x,y;
  unsigned short width,height;
  short angle1,angle2;
  struct Dougee_Arc *next;
};

struct Dougee_Segment *Top_Seg,*Cur_Seg;
struct Dougee_Point *Top_Point,*Cur_Point;
struct Dougee_Rect *Top_Rec,*Cur_Rec;
struct Dougee_Arc *Top_Arc,*Cur_Arc;

int x,y,num_entries_colormap;
unsigned int width,height,border_width,one_half_width,one_half_height;
unsigned int Zero,One;
unsigned int three_quarters_width,three_quarters_height;
unsigned int one_thirtyth_width,one_thirtyth_height;
unsigned int one_twentyfifth_width,one_twentyfifth_height;
unsigned int one_twentyth_width,one_twentyth_height;
unsigned int one_fifteenth_width,one_fifteenth_height;
unsigned int one_twelfth_width,one_twelfth_height;
unsigned int one_tenth_width,one_tenth_height;
unsigned int one_eighth_width,one_eighth_height;
unsigned int one_fifth_width,one_fifth_height;
unsigned int choice1_x,choice1_y,drawing_x,drawing_y;
unsigned int drawing_width,drawing_height;
unsigned int choice1_width,choice1_height;
unsigned long background_pixel,GCAttributes,draw_foreground;
unsigned long choice_foreground,arc_height,arc_width;
int arc_x,arc_y,arc_angle1,arc_angle2;
char choice[80];
Display *Dougee_Display;
Window Dougee_Window,Parent,to_kill[500];
Visual *Dougee_Visual;
XSetWindowAttributes *Dougee_Attributes;
/*  colormap stuff */
Colormap Dougee_Colormap;
XColor Dougee_Color,*NewColors,*TempNewColors;
Status Dougee_Status;
int contig,ncolors,nplanes,*planes;
int Dougee_Screen,Dougee_Depth;
unsigned long pixels[80];
GC Dougee_GC,Drawing_GC,Choice_GC_1,Text_GC,Shapes_GC;
XGCValues *Dougee_GCValues;
Drawable Dougee_Drawable;
Font Dougee_Font;
XRectangle Dougee_Rectangle[2];
XFontStruct *Dougee_FontStruct;
char Font_Char[30];
char alphabet[28];

void Initialize(),
     CreateGCs(),
     Add_Rectangle(), /* x,y,width,height,filled */
     Draw_Rectangles(),
     Free_Memory(),
     Constants();
