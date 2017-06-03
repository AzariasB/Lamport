/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "svg_file.h"

svg* svg_create(char* path, char* mode){
    svg *f = malloc(sizeof(svg));
    f->file = fopen(path,mode);
    fprintf(f->file, HEADER);
    return f;
}
void svg_draw_line(svg* file, int x_start, int y_start, int x_end, int y_end,
                                                color c){
    fprintf(file->file,LINE,x_start, y_start, x_end, y_end, c.red, c.green, c.blue);
}
void svg_draw_line_points(svg* file, point* start, point* end,
                                                color c){
    fprintf(file->file,LINE,start->x, start->y, end->x, end->y, c.red, c.green, c.blue);
}
void svg_draw_circle_pt(svg* file, point* center, int radius, color c){
    fprintf(file->file,CIRCLE,center->x, center->y, radius, c.red, c.green, c.blue);
}
void svg_draw_circle(svg* file, int x, int y, int radius, color c){
    fprintf(file->file,CIRCLE,x, y, radius, c.red, c.green, c.blue);
}

void svg_draw_arrow(svg* file, int x_start, int y_start, int x_end, 
                                                int y_end, color c){
    point* start = point_create(x_start,y_start);
    point* end = point_create(x_end, y_end);
    int dist = (int)point_dist(start,end);
    point* pt_1 = point_new_at(start,end,0.5,dist*2/4);
    point* pt_2 = point_new_at(start,end,-0.5,dist*2/4);
    svg_draw_line_points(file,start,end,c);
    svg_draw_line_points(file,pt_1,end,c);
    svg_draw_line_points(file,pt_2,end,c);
}
void svg_draw_pt_arrow(svg* file, point* start, point* end,
                                                color c){
    svg_draw_arrow(file,start->x,start->y,end->x,end->y,c);
}

void svg_end(svg* file){
    fprintf(file->file,FOOTER);
    fclose(file->file);
}