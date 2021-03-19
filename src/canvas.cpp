#include "canvas.h"
#include <cassert>

using namespace std;
using namespace agl;

canvas::canvas(int w, int h) : _canvas(w, h)
{
}

canvas::~canvas()
{
}

void canvas::save(const std::string& filename)
{
   _canvas.save(filename);
}

void canvas::begin(PrimitiveType type)
{
   shape = type;
}

void canvas::end()
{
   switch(shape){
      case LINES: 
         while(vertices.size()>1){
            Vertex v1 = vertices.front();
            vertices.pop_front();
            Vertex v2 = vertices.front();
            vertices.pop_front();
            drawLine(v1, v2);
         } 
         break;
      case TRIANGLES:
         while(vertices.size()>2){
            Vertex v1 = vertices.front(); 
            vertices.pop_front();
            Vertex v2 = vertices.front();
            vertices.pop_front();
            Vertex v3 = vertices.front();
            vertices.pop_front();
            drawTriangle(v1, v2, v3);
         } 
         break; 
      case POINTS:
         while(vertices.size()>0){
            Vertex v = vertices.front();
            vertices.pop_front();
            _canvas.set(v.y, v.x, v.color);
         }
         break;
      default: 
         cout << "Shape is undefined" << endl;
   }
}

void canvas::vertex(int x, int y)
{
   Vertex v = {x,y,currColor};
   vertices.push_back(v);
}

void canvas::color(unsigned char r, unsigned char g, unsigned char b)
{
   currColor.r = r;
   currColor.g = g;
   currColor.b = b;
}

void canvas::background(unsigned char r, unsigned char g, unsigned char b)
{
   back.r = r;
   back.g = g;
   back.b = b;
   for(int i = 0; i<_canvas.height(); i++){
      for(int j = 0; j<_canvas.width(); j++){
         _canvas.set(i, j, back);
      }
   } 
}

void canvas::drawLine(Vertex v1, Vertex v2)
{
   int W = v2.x - v1.x;
   int H = v2.y - v1.y;
   if(abs(H) < abs(W)){
      if(v1.x < v2.x)
         lowLine(v1, v2);
      else{ 
         lowLine(v2, v1);
      }
   }
   else {
      if(v1.y < v2.y)
         highLine(v1, v2);
      else{
         highLine(v2, v1);
      }
   }
}

void canvas::lowLine(Vertex v1, Vertex v2)
{
   int y = v1.y;
   int W = v2.x - v1.x;
   int H = v2.y - v1.y;
   int inc = 1;
   float t;
   ppm_pixel color;
   if(H<0){
      H = -H;
      inc = -1;
   }
   int F = 2*H - W;
   for(int x = v1.x; x<v2.x; x++){
      t = (x-v1.x)/(float)(v2.x-v1.x);
      color.r = v1.color.r*(1-t)+v2.color.r*t;
      color.g = v1.color.g*(1-t)+v2.color.g*t;
      color.b = v1.color.b*(1-t)+v2.color.b*t;
      if(y<_canvas.height() && y>=0) // if in range, set pixel
         _canvas.set(y,x,color);

      if(F>0){
         y+=inc;
         F+=2*(H-W);
      }
      else{
         F+=2*H;
      }
   }
}
         
void canvas::highLine(Vertex v1, Vertex v2)
{
   int x = v1.x;
   int W = v2.x - v1.x;
   int H = v2.y - v1.y;
   int inc = 1;
   float t;
   ppm_pixel color;
   if(W<0){
      W=-W;
      inc = -1;
   }
   int F = 2*W - H;
   for(int y = v1.y; y<v2.y; y++){
      t = (y-v1.y)/(float)(v2.y-v1.y);
      color.r = v1.color.r*(1-t)+v2.color.r*t;
      color.g = v1.color.g*(1-t)+v2.color.g*t;
      color.b = v1.color.b*(1-t)+v2.color.b*t;
      if(x<_canvas.width() && x>=0) // if in range, set pixel
         _canvas.set(y,x,color);

      if(F>0){
         x+=inc;
         F+=2*(W-H);
      }
      else{
         F+=2*W;
      }
   }
}

void canvas::drawTriangle(Vertex v1, Vertex v2, Vertex v3)
{
   ppm_pixel color;
   if(nofill){
      drawLine(v1,v2);
      drawLine(v2,v3);
      drawLine(v1,v3);
   }
   else{
      float f_alpha = (v3.y-v2.y)*(v1.x-v2.x)-(v3.x-v2.x)*(v1.y-v2.y);
      float f_beta = (v3.y-v1.y)*(v2.x-v1.x)-(v3.x-v1.x)*(v2.y-v1.y);
      float f_gamma = (v2.y-v1.y)*(v3.x-v1.x)-(v2.x-v1.x)*(v3.y-v1.x);
      cout << f_alpha << " " << f_beta << " " << f_gamma << endl;
      int minx = min(v1.x, min(v2.x, v3.x));
      int miny = min(v1.y, min(v2.y, v3.y));
      int maxx = max(v1.x, max(v2.x, v3.x));
      int maxy = max(v1.y, max(v2.y, v3.y));
      for(int i = miny; i<=maxy; i++){
         for(int j = minx; j<=maxx; j++){
            float alpha = ((v3.y-v2.y)*(j-v2.x)-(v3.x-v2.x)*(i-v2.y))/f_alpha;
            float beta = ((v3.y-v1.y)*(j-v1.x)-(v3.x-v1.x)*(i-v1.y))/f_beta;
            float gamma = ((v2.y-v1.y)*(j-v1.x)-(v2.x-v1.x)*(i-v1.x))/f_gamma;
            color.r = alpha*v1.color.r + beta*v2.color.r + gamma*v3.color.r;
            color.g = alpha*v1.color.g + beta*v2.color.g + gamma*v3.color.g;
            color.b = alpha*v1.color.b + beta*v2.color.b + gamma*v3.color.b;
            if(alpha>=0 && beta>=0 && gamma>=0 && (color.r!=back.r | color.g!=back.g | color.b!=back.b))
               _canvas.set(i,j,color);
         }
      }
   }
} 

void canvas::rectangle(int x, int y, int w, int h){
   if(nofill){
      Vertex v1 = {x-w/2, y-w/2, currColor};
      Vertex v2 = {x+w/2, y-w/2, currColor};
      Vertex v3 = {x-w/2, y+w/2, currColor};
      Vertex v4 = {x+w/2, y+w/2, currColor};
      drawLine(v1,v2);
      drawLine(v1,v3);
      drawLine(v4,v2);
      drawLine(v4,v3);
   }
   else {
      for(int i = y-h/2; i<=y+h/2; i++){
         for(int j = x-w/2; j<=x+w/2; j++){
            _canvas.set(i, j, currColor);
         }
      } 
   }
}

void canvas::differenceBlend(ppm_pixel blend){
   for(int i = 0; i<_canvas.height(); i++){
      for(int j = 0; j<_canvas.width(); j++){
         ppm_pixel result;
         result.r = abs(blend.r - _canvas.get(i,j).r);
         result.g = abs(blend.g - _canvas.get(i,j).g);
         result.b = abs(blend.b - _canvas.get(i,j).b);
         _canvas.set(i, j, result);
      }
   } 
}

void canvas::outline(bool b){
   nofill = b;
}