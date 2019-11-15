/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UIHandler.h
 * Author: nicolas
 *
 * Created on October 8, 2019, 4:46 PM
 */

#ifndef UIHANDLER_H
#define UIHANDLER_H

#include <string>
#include <time.h>

class Application;
class WindowEnv;



class UIHandler {
public:
    UIHandler();
    UIHandler(const UIHandler& orig);
    
    static void Init(WindowEnv * pwinEnv);
    
    void Render(Application & app);
    
    void CameraProjection(Application & app);

    
    virtual ~UIHandler();
private:
     std::string m_text = " texte a editer";
     clock_t m_timeCurrent;
    
};

#endif /* UIHANDLER_H */

