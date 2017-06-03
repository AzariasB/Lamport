/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   svg_file.h
 * Author: user
 *
 * Created on 19 mai 2017, 16:46
 */

#ifndef SVG_FILE_H
#define SVG_FILE_H
#include "geometry.h"
#include "color.h"
#define HEADER "<!DOCTYPE html>\n<html>\n<body>\n<h1>Lamport SVG</h1>\n<svg width=\"1000\" height=\"1000\">\n"
#define FOOTER "</svg> \n</body>\n</html>"
#define LINE   "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"stroke:rgb(%d,%d,%d);stroke-width:2\" />\n "
#define CIRCLE "<circle cx=\"%d\" cy=\"%d\" r=\"%d\" stroke=\"black\" stroke-width=\"3\" fill=\"rgb(%d,%d,%d)\" />"
typedef struct svg svg;
struct svg{
    FILE* file;
};
svg* svg_create(char* path, char* mode);
void svg_draw_line(svg* file, int x_start, int y_start, int x_end, int y_end,
                                                color c);
void svg_draw_line_points(svg* file, point* start, point* end,
                                                color c);
void svg_draw_circle_pt(svg* file, point* center, int radius, color c);
void svg_draw_circle(svg* file, int x, int y, int radius, color c);
void svg_draw_arrow(svg* file, int x_start, int y_start, int x_end, 
                                                int y_end, color c);
void svg_draw_pt_arrow(svg* file, point* start, point* end,
                                                color c);
void svg_end(svg* file);
#endif /* SVG_FILE_H */