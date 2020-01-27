/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   first.c
 * Author: slaw
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ull unsigned long long int

//*********************************************************************GLOBAL VARIABLES
//to toReturn
int noPrefRead;
int noPrefWrite;
int noPrefHit;
int noPrefMiss;

int withPrefRead ;
int withPrefWrite;
int withPrefHit ;
int withPrefMiss;

//Initial info
int cacheSize ;
int blockSize ;
int offSet;
FILE *toTest;

//to calculate based off of cacheSize
int numRows;
int numColumns;

int b1;
int s1;

//*********************************************************************GLOBAL VARIABLES
int isDirect = -1;
int isAssoc = -1;
int isAssocN = -1;

int valueN = -1;

ull getIndex(ull address){
  int s = log2(numRows);
	ull index = (address >> offSet) & ((1 << s) - 1);
	return index;
}

ull getTag(ull address){
  int s = log2(numRows);
	ull tag = address >> (offSet + s);
	return tag;
}


int isPowerOfTwo(int toCheck){
  double check = toCheck;
  while(check > 1 ){
    check = check * .5;
  }

  if(check == 1){
    return 1;
  }else{
    return -1;
 }
}

int checkCache(ull **cache, ull address) {
  int index = getIndex(address);
  ull tag = getTag(address);
  int i;

  for(i = numColumns - 1; i >= 0; i--)
    if(cache[index][i] == tag) return 1;
  return 0;
}

void insertCache(ull **cache, ull address) {
  int index = getIndex(address);
  ull tag = getTag(address);
  int i;

  for(i = numColumns - 1; i > 0; i--)
    cache[index][i] = cache[index][i - 1];
  cache[index][0] = tag;
}

void moveUp(ull **cache, ull address) {
  int index = getIndex(address);
  ull tag = getTag(address);
  int location;
  int i;

  for(i = numColumns - 1; i >= 0; i--)
    if(cache[index][i] == tag) {
      location = i;
      break;
    }

  for(i = location; i > 0; i--)
    cache[index][i] = cache[index][i - 1];
  cache[index][0] = tag;
}



int main(int argc, char** argv) {

//GETTING INPUT

  //***************************************WRONG NUMBER OF ARGUMENTS
  if(argc!=6){
    printf("error\n");
    return 0;
  }
  //***************************************

  //***************************************CACHE SIZE
  cacheSize = atoi(argv[1]);

  //***************************************

  //*************************************** ASSOCIATIVE TEST

  char strDirect[] = "direct";
  char strAssoc[] = "assoc";
  char strAssocC[] = "assoc:";

  int retDirect = strcmp(argv[2], strDirect);
  int retAssoc = strcmp(argv[2], strAssoc);

  int assocN = -1;

  if(retDirect == 0){
    assocN = assocN + 2;
    isDirect = 1;
  }else if (retAssoc == 0){
    isAssoc = 1;
  }else{
    //get beginning and ending index
    char *colon;
    int index;
    colon = strchr(argv[2], ':');
    index = (int)(colon - argv[2]) + 1;
    int endIndex = sizeof(argv[2]);
    char strAssocN [endIndex-index];
    strncpy(strAssocN, argv[2] + index, endIndex-index);
    int assocNumber = atoi(strAssocN);
    valueN = assocNumber;


    if(isPowerOfTwo(assocNumber)!=1){
      printf("error\n");
      return 0;
    }

    char previousReal [1+index];
    strncpy(previousReal, argv[2], index);

    int retTestAssocN = strcmp(previousReal, strAssocC);
    if(retTestAssocN!=0){
      printf("error\n");
      return 0;
    }
    isAssocN = 1;

    numColumns = assocNumber;

  }

  char strCachePolicy[] = "lru";
  int testCachePolicy = strcmp(argv[3], strCachePolicy);
  if(testCachePolicy!=0){
    printf("error\n");
    return 0;
  }

  blockSize = atoi(argv[4]);
  if(isPowerOfTwo(blockSize)!=1){
    printf("error\n");
    return 0;
  }

  offSet = (int) log2(blockSize);
  if(cacheSize<blockSize){
    printf("error\n");
    return 0;
  }

  toTest = fopen(argv[5], "r");
  //if does not exist
  if( toTest==NULL)  {
    printf("error");
    return 0;
  }

  if(isDirect == 1){
    numRows = cacheSize/blockSize;
    numColumns = 1;


  } else if(isAssoc==1){
    numRows = 1;
    numColumns = cacheSize/blockSize;

  }else if(isAssocN ==1){
    numRows = cacheSize/(blockSize * numColumns);
  }else{
    printf("error");
    return 0;
  }
  //
  // printf("rows: %d cols %d\n", numRows, numColumns);
  // printf("block_offset: %d index_offset: %d\n", offSet, (int) log2(numRows));

  char operation;
  ull address;
  int i;

  ull** cache1 = (ull **) malloc(numRows * sizeof(ull *));
  ull** cache2 = (ull **) malloc(numRows * sizeof(ull *));
  for(i = 0; i < numRows; i++)
    cache1[i] = (ull *) malloc(numColumns * sizeof(ull)), cache2[i] = (ull *) malloc(numColumns * sizeof(ull));

  while(fscanf(toTest, "%*x: %c %llx", &operation, &address)==2){

    if(operation == 'R') {
      if(checkCache(cache1, address) == 1) {
        noPrefHit++;
        moveUp(cache1, address);
      }
      else {
        noPrefRead++;
        noPrefMiss++;
        insertCache(cache1, address);
      }
    }
    else {
      if(checkCache(cache1, address) == 1) {
        noPrefHit++;
        noPrefWrite++;
        moveUp(cache1, address);
      }
      else {
        noPrefRead++;
        noPrefWrite++;
        noPrefMiss++;
        insertCache(cache1, address);
      }
    }

    if(operation == 'R') {
      if(checkCache(cache2, address) == 1) {
        withPrefHit++;
        moveUp(cache2, address);
      }
      else {
        withPrefRead++;
        withPrefMiss++;
        insertCache(cache2, address);

        address += blockSize;
        if(checkCache(cache2, address) == 0) {
          withPrefRead++;
          insertCache(cache2, address);
        }
      }
    }
    else {
      if(checkCache(cache2, address) == 1) {
        withPrefHit++;
        withPrefWrite++;
        moveUp(cache2, address);
      }
      else {
        withPrefRead++;
        withPrefWrite++;
        withPrefMiss++;
        insertCache(cache2, address);

        address += blockSize;

        if(checkCache(cache2, address) == 0) {
          withPrefRead++;
          insertCache(cache2, address);
        }

      }
    }

  }

  printf("no-prefetch\n");
  printf("Memory reads: %d\n", noPrefRead);
  printf("Memory writes: %d\n", noPrefWrite);
  printf("Cache hits: %d\n", noPrefHit);
  printf("Cache misses: %d\n", noPrefMiss);
  printf("with-prefetch\n");
  printf("Memory reads: %d\n", withPrefRead);
  printf("Memory writes: %d\n", withPrefWrite);
  printf("Cache hits: %d\n", withPrefHit);
  printf("Cache misses: %d\n", withPrefMiss);


  //FREE THINGS
  /*
  int free1;
  for (free1= 0; free1 < numRows; free1++){
    struct entry* currentPtrNoPref = cacheNoPref[free1];
    free(currentPtrNoPref);
    struct entry* currentPtrWithPref = cacheWithPref[free1];
    free(currentPtrWithPref);

  }
  free(cacheNoPref);
  free(cacheWithPref);
*/


  for (i = 0; i < numRows; i++){
    ull* ptr1 = cache1[i];
    ull*ptr2 = cache2[i];
    free(ptr1);
    free (ptr2);
  }
  free(cache1);
  free(cache2);
  fclose(toTest);
  return (0);
}
