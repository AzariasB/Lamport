/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "color.h"
color* color_create(int red, int blue, int green){
    color * c = malloc(sizeof(color));
    c->red = red;
    c->blue = blue;
    c->green = green;
    return c;
}