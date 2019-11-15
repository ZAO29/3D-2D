/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CameraFree.h
 * Author: nicolas
 *
 * Created on October 8, 2019, 9:35 AM
 */

#ifndef CAMERAFREE_H
#define CAMERAFREE_H

#include "Camera.h"

class CameraFree : public Camera {
public:
    CameraFree();

    virtual ~CameraFree();
    
    virtual glm::mat4 getView() override;
    
private:

};

#endif /* CAMERAFREE_H */

