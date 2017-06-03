/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   color.h
 * Author: user
 *
 * Created on 23 mai 2017, 17:29
 */

#ifndef COLOR_H
#define COLOR_H
// this is causing memory leaks and must be removed.
#define RED *color_create(255,0,0)/*(color){.red = 255, .blue = 0, .green = 0}*/
#define GREEN *color_create(0,255,0)
#define YELLOW *color_create(255,255,0)
#define ORANGE *color_create(255,165,0)
#define TURQUOISE *color_create(64,224,208)
#define BLACK *color_create(0,0,0)
#ifdef __cplusplus
extern "C" {
#endif

    typedef struct color color;
    struct color{
        int red;
        int green;
        int blue;
    };
    color* color_create(int red, int blue, int green);


#ifdef __cplusplus
}
#endif

#endif /* COLOR_H */

