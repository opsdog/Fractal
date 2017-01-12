#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define MIN(x,y) ((x)<(y) ? (x) : (y))
#define MAX(x,y) ((x)>(y) ? (x) : (y))
#define Max(x,y,z) (x>y && x>z ? x:(y>z ? y:z))
#define Min(x,y,z) (x<y && x<z ? x:(y<z ? y:z))

void Intensity();

char ObjectFile[32];
char file_in={"DougeeTest.pc"};
int Xres, Yres;
int Height[2][800];
int max_color=63,max_iterations,flag;
int Scaling;
struct Vector {
  float x,y,z;
} view,
  light,
  color_vec,
  Pix,
  newPix,
  oldPix,
  ambient,
  diffuse,
  specular,
  background,
  alt_background;
float

#define Max3(x,y,z) (x<y && x<z ? x : (y<z ? y : z))

void AlternateIntensity()
{
  struct Vector {
    float x,y,z;
  };

  struct Vector view,light,color_vec;

  float ViewPhi=65.0, LightPhi=75.0, LightTheta= -35.0,
        Xmax=1.2, Xmin= -2.0, Ymax=1.2, Ymin= -1.2, Zmax=1.0, Zmin=0.0;

  struct Vector n1, n2, difference, normal, reflected, ambient;
  struct Vector diffuse, specular;
  float CosTheta, CosAlpha, temp, maximum, b;
  int dougeei,dougeej, max_color=63;

  ViewPhi=65.0*0.0174533; LightPhi=75.0*0.0174533; LightTheta= -35.0*0.0174533;
  light.x=sin(LightTheta)*cos(LightPhi); light.y=sin(LightTheta)*sin(LightPhi);
  light.z=cos(LightTheta);
  view.x=0.0; view.y= -cos(ViewPhi); view.z=sin(ViewPhi);

  for (dougeei=0;dougeei<dougeen-1;dougeei++) {
    for (dougeej=0;dougeej<dougeen;dougeej++) {
      n1.x=(Xmax-Xmin)/(getmaxx+1); n1.y=0.0;
      n1.z=(ZMap[dougeei+1][dougeej]-ZMap[dougeei][dougeej])/256.0;
      n2.x=0.0; n2.y=(Ymax-Ymin)/(getmaxy+1);
      n2.z=(ZMap[dougeei][dougeej+1]-ZMap[dougeei][dougeej])/256.0;
      normal.x= -n1.z*n2.y; normal.y= -n1.x*n2.z; normal.z= n1.x*n2.y;
      temp=sqrt(normal.x*normal.x+normal.y*normal.y+normal.z*normal.z);
      if (temp!=0) {
	normal.x/=temp; normal.y/=temp;	normal.z/=temp;
      }
      CosTheta=normal.x*light.x+normal.y*light.y+normal.z*light.z;
      if (CosTheta<0) {
	color_vec.x=ambient.x*max_color;
	color_vec.y=ambient.y*max_color;
	color_vec.z=ambient.z*max_color;
      } else {
	reflected.x=(normal.x-light.x)*(2.0*CosTheta);
	reflected.y=(normal.y-light.y)*(2.0*CosTheta);
	reflected.z=(normal.z-light.z)*(2.0*CosTheta);
	CosAlpha=view.x*reflected.x+view.y*reflected.y+view.z*reflected.z;
	color_vec.x=((specular.x*pow(CosAlpha,b) + ambient.x*diffuse.x +
		      (diffuse.x*CosTheta))*max_color);
	color_vec.y=(specular.y*pow(CosAlpha,b) + ambient.y*diffuse.y +
		     (diffuse.y*CosTheta))*max_color;
	color_vec.z=(specular.z*pow(CosAlpha,b) + ambient.z*diffuse.z +
		     (diffuse.z*CosTheta))*max_color;
      }
      maximum=Max3(color_vec.x,color_vec.y,color_vec.z);
      if (maximum>max_color) {
	color_vec.x*=(max_color/maximum);
	color_vec.y*=(max_color/maximum);
	color_vec.z*=(max_color/maximum);
      }
    }
  }
}

