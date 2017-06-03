/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "geometry.h"
point* point_create(int x, int y){
    point * p = malloc(sizeof(point));
    p->x = x;
    p->y = y;
    return p;
}
point* point_new_at(point* p,point* origin, float angle, int dist){
    float current_angle = point_angle_with(p,origin);
    float nouv_angle = current_angle+angle;
    return point_create(origin->x+(int)(cos(nouv_angle)*dist),
                        origin->y+(int)(sin(nouv_angle)*dist));
}
float point_angle_with(point* p, point* origin){
    float dist = point_dist(p,origin);
    if(dist ==0){
        return 0;
    }
    float x = (p->x-origin->x)/dist;
    float angle = acos(x);
    if (p->y>origin->y){
        return angle;
    }
    return 2*M_PI-angle;
} 
float point_dist(point* p, point* target){
    return sqrt((p->x-target->x)*(p->x-target->x)+(p->y-target->y)*(p->y-target->y));
}