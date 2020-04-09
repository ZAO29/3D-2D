/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CameraTrackBall.h
 * Author: nicolas
 *
 * Created on October 7, 2019, 5:11 PM
 */

#ifndef CAMERATRACKBALL_H
#define CAMERATRACKBALL_H

#include "Camera.h"


class CameraTrackBall : public Camera {
public:
    CameraTrackBall();

    virtual ~CameraTrackBall();
    
    virtual glm::mat4 getView() override;
    
    
private:
    
    
    
     
    
    


};

#endif /* CAMERATRACKBALL_H */

