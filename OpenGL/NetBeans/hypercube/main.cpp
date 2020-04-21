/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: nicolas
 *
 * Created on December 17, 2019, 8:24 PM
 */

#include <cstdlib>

using namespace std;

/*
 * 
 */

#include <cstdlib>
#include "HyperApp.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    HyperApp appli;
  

  appli.Init();
  
  
  while(appli.Run())
  {
      
  }
  
      
  appli.Destroy();    

    return 0;
}
