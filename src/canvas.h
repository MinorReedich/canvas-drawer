#ifndef canvas_H_
#define canvas_H_

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <list>
#include "ppm_image.h"

namespace agl
{
   enum PrimitiveType {UNDEFINED, LINES, TRIANGLES, POINTS};

    struct Vertex 
   {
      int x;
      int y;
      ppm_pixel color;
   };

   class canvas
   {
   public:
      canvas(int w, int h);
      virtual ~canvas();

      // Save to file
      void save(const std::string& filename);

      // Draw primitives with a given type (either LINES or TRIANGLES)
      // For example, the following draws a red line followed by a green line
      // begin(LINES);
      //    color(255,0,0);
      //    vertex(0,0);
      //    vertex(100,0);
      //    color(0,255,0);
      //    vertex(0, 0);
      //    vertex(0,100);
      // end();
      void begin(PrimitiveType type);
      void end();

      // Specifiy a vertex at raster position (x,y)
      // x corresponds to the column; y to the row
      void vertex(int x, int y);

      // Specify a color. Color components are in range [0,255]
      void color(unsigned char r, unsigned char g, unsigned char b);

      // Fill the canvas with the given background color
      void background(unsigned char r, unsigned char g, unsigned char b);

      // Draw a rectangle with the given center coordinates, width and height
      void rectangle(int x, int y, int w, int h);

      // Difference blend the canvas with the given color
      void differenceBlend(ppm_pixel blend); 

      // Sets the shape to be outlined, default is to fill
      void outline(bool b);

   private:
      // Draw a line with the given vertices
      void drawLine(Vertex v1, Vertex v2);
      
      // Draw a triangle with the given vertices
      void drawTriangle(Vertex v1, Vertex v2, Vertex v3);

      // Draw low line (drawLine helper)
      void lowLine(Vertex v1, Vertex v2);

      // Draw high line (drawLine helper)
      void highLine(Vertex v1, Vertex v2);

      ppm_image _canvas; // the image we are filling
      PrimitiveType shape; // the shape we are plotting
      std::list<Vertex> vertices; // list of vertices
      ppm_pixel currColor; // current color
      ppm_pixel back; // background color
      bool nofill; // whether or not we fill the shape
   };
}

#endif


