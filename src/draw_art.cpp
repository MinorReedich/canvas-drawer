#include <iostream>
#include "canvas.h"
using namespace std;
using namespace agl;

int main(int argc, char** argv)
{
   canvas drawer(640, 380);
   drawer.background(0,0,0);
   drawer.begin(POINTS);
   drawer.color(255,255,255);
   for(int i = 0; i<200; i++){
      drawer.vertex(rand()%640, rand()%380);
   }
   drawer.end();
   
   for(int j = 0; j<rand()%2+30; j++){
      if(rand()%2 == 0){
         drawer.outline(true);
      }
      else{
         drawer.outline(false);
      }
      drawer.color(rand()%256, rand()%256, rand()%256);
      drawer.rectangle(rand()%(640-100)+50, rand()%(380-40)+20, rand()%50+10, rand()%20+20);
   }
   drawer.save("../rectangles_in_space.png");

   ppm_pixel dif;
   dif.r = rand()%256;
   dif.g = rand()%256;
   dif.b = 255;

   drawer.differenceBlend(dif);
   drawer.save("../rectangles_difference_blend.png");
}