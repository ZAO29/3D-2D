/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: nicolas
 *
 * Created on February 27, 2020, 8:42 PM
 */

#include <cstdlib>
#include "GeometryShaderApp.h"
/*
 * 
 */
int main(int argc, char** argv)
{
    App appli;
  

  if(!appli.Init())
      return 0;
  
  
  while(appli.Run())
  {
      
  }
  
      
  appli.Destroy();    

    return 0;
}

