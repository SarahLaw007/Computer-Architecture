/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   second.c
 * Author: slaw
 *
 * Created on February 8, 2019, 2:51 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
/*
 *
 */

unsigned short value = 0;

unsigned short get(unsigned short x){
  unsigned short finder = 1;
  finder = finder << x;
  unsigned short toReturn = value & finder;
  toReturn = toReturn >> x;
  return toReturn;

}

unsigned short comp(unsigned short x){
  unsigned short finder = 1;
  finder = finder << x;
  unsigned short toReturn = value ^ finder;
  return toReturn;
}

unsigned short set(unsigned short x, unsigned short y){
  unsigned short finder = 1;
  finder = 1 << x;
  finder = ~finder;
  unsigned short clearedBit = value & finder;

  //now set the xth bit

  unsigned short setter = y;
  setter = setter << x;
  unsigned short toReturn = clearedBit | setter;
  return toReturn;
}

int main(int argc, char** argv) {

  FILE *toTest;
  toTest = fopen(argv[1], "r");

  //if does not exist
  if( toTest==NULL)  {
    printf("error");
    return 0;
    //NO FCLOSE OR ELSE IT WILL SEG FAULT  int numNodes = 0;

  }
  //if blank
  if (NULL != toTest) {
    fseek (toTest, 0, SEEK_END);
    int size = ftell(toTest);

    if (0 == size) {
        printf("0\n");
        fclose(toTest);
        return 0;
    }
  }


  fseek(toTest, 0, SEEK_SET);

//  char command[] = "";
  char command[5] = "sos";
  //char ptr = &command;
  int number;
  int x = 0;
  int y = 0;
  fscanf(toTest,"%d\n",&number);
  //printf("THIS IS MY NUMBER: %d\n", number);
  value = number;

  while(fscanf(toTest,"%s\t%d\t%d",command, &x, &y)!=EOF){

    //printf("THIS IS MY COMMAND on %d: %s: %d, %d\n", value, command, x, y);
  //  int isGet = strcmp(command, "get");
    if(strcmp(command, "get")==0){
      //printf("GET\n");

      printf("%d\n", get(x));

    }else if(strcmp(command, "comp")==0){
      //printf("COMP\n");
      value = comp(x);
      printf("%d\n", value);

    }else if(strcmp(command, "set")==0){
      value = set(x,y);
      printf("%d\n", value);



    }else{
      printf("ERROR\n");

    }




  }




  fclose(toTest);

  return (EXIT_SUCCESS);

}
