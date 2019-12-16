/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: nicolas
 *
 * Created on December 15, 2019, 7:56 PM
 */

#include <cstdlib>
#include "App.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    App appli;
  

  appli.Init();
  
  
  while(appli.Run())
  {
      
  }
  
      
  appli.Destroy();    

    return 0;
}

