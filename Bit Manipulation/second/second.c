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

    unsigned short index;
    unsigned short numPairs = 0;
    unsigned short numOnes = 0;
    unsigned short numPrevOnes = 0;

    for(index = number; index >0; index = index>>1  ){
      unsigned short currentBit = get(0,index);
      if(currentBit == 1){
        numOnes++;
        if(numPrevOnes!=0){
          numPairs++;
          numPrevOnes = 0;
        }else{
          numPrevOnes =1;
        }
      }else{
        numPrevOnes=0;
      }
    }

    if(get(0, numOnes)==1){
      printf("Odd-Parity\t%d\n", numPairs);
    }else{
      printf("Even-Parity\t%d\n", numPairs);
    }









    return 0;
}
