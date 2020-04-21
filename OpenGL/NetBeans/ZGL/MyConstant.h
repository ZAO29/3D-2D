/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MyConstant.h
 * Author: nicolas
 *
 * Created on September 20, 2019, 3:17 PM
 */

#ifndef MYCONSTANT_H
#define MYCONSTANT_H



#define __FILEFUNC__ __FILE__<<__FUNCTION__<< " : "

#define INVALID_UNIFORM_LOCATION 0xffffffff
#define INVALID_OGL_VALUE 0xffffffff


#define INVALID_MATERIAL 0xFFFFFFFF

#define COLOR_UNIT 0
#define NORMALMAP_UNIT 1

#define PI 3.141592653f

#define DEG2RAD (2*PI/360.0f)
#define RAD2DEG 1.0f/DEG2RAD


enum eCameraType
{
    TRACKBALLCAMERA,
    FREECAMERA
};


enum eState
{
    NOT_INIT,
    INITIALIZED,
    RUNNING,
    TOCLOSE,
    CLOSE

};





#endif /* MYCONSTANT_H */



