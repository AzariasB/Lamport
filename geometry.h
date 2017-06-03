/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   geometry.h
 * Author: user
 *
 * Created on 19 mai 2017, 17:08
 */
#ifndef GEOMETRY_H
#define GEOMETRY_H
#define M_PI 3.141592
#include <math.h>
typedef struct point point;
struct point{
    int x;
    int y;
};
point* point_create(int x, int y);
point* point_new_at(point* p,point* origin, float angle, int dist);
float point_angle_with(point* p, point* origin);
float point_dist(point* p, point* target);
#endif /* GEOMETRY_H */