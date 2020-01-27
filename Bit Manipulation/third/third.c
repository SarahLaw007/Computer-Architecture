/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   fifth.c
 * Author: slaw
 *
 * Created on February 8, 2019, 2:52 PM
 */

#include <stdio.h>
#include <stdlib.h>

/*
 *
 */

 unsigned short get(unsigned short x, unsigned short value){
   unsigned short finder = 1;
   finder = finder << x;
   unsigned short toReturn = value & finder;
   toReturn = toReturn >> x;
   return toReturn;
 }


int main(int argc, char** argv) {
    char *futureNum = argv[1];
    unsigned short number = atoi(futureNum);
    //printf("%d\n", number);
    unsigned short x;
    for(x = 0; x < 8; x++  ){
      unsigned short currentBitRight = get(x,number);
      unsigned short currentBitLeft = get((15-x),number);
      if(currentBitRight!=currentBitLeft){
        printf("Not-Palindrome\n");
        return 0;
      }

    }


    printf("Is-Palindrome\n");






    return 0;
}
