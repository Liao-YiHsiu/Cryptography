#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// IBIT should be smaller than or equal to 6
#define IBIT 4 
#define IALL (1 << IBIT) 
#define MAX (1 << IALL)
using namespace std;

typedef unsigned short word;

word* arr = NULL;
char* map = NULL;
char* traO = NULL;
long * traA = NULL;
long * traB = NULL;
long count = 0;

char color = 1;

void init(){
   if(arr == NULL)
      arr = new word[MAX];
   if(map == NULL)
      map = new char[MAX];

   if(traO == NULL)
      traO = new char[MAX];
   if(traA == NULL)
      traA = new long[MAX];
   if(traB == NULL)
      traB = new long[MAX];

   color++;
   count = 0;
}

void push(word a, long iA, long iB, char oper){
   assert( count < MAX );

   //for(int i = 0; i < count; i++)
   //   if(arr[i] == a) return;
   if(map[a] == color)
      return;
   map[a] = color;

   arr[count] = a;
   traO[count] = oper;
   traA[count] = iA;
   traB[count] = iB;

   count++;
}

void print2(word a){
   for(int i = IALL-1; i >= 0; i--)
      cout << ((a >> i) & 1);
   cout << endl;
}

void find(word target[4]){

   init();
   printf("target = 0b");
   print2(target);

   // a1 ~ a6
   word one = 1;
   word a[IBIT] = {0};
   for(int i = 0; i < IALL ; ++i)
      for(int j = 0; j < IBIT; ++j)
         a[j] |= ((i >> j) & one) << i; 

   for(int i = 0; i < IBIT; ++i){
      printf("a%d = 0b", i);
      print2(a[i]);
      push(a[i], i, i, 'a');
   }

   word tmp;
   long tmp_count;
   int num = 0;
   while(true){
      num++;
      tmp_count = count;

      for(long i = 0; i < tmp_count; ++i)
         for(long j = i+1; j < tmp_count; ++j){
            tmp = arr[i] & arr[j];
            push(tmp, i, j, '&');
            if(tmp == target) return;

            tmp = arr[i] | arr[j];
            push(tmp, i, j, '|');
            if(tmp == target) return;

            tmp = arr[i] ^ arr[j];
            push(tmp, i, j, '^');
            if(tmp == target) return;
         }

      for(long i = 0; i < tmp_count; ++i){
         tmp = ~arr[i];
         push(tmp, i, i, '~');
         if(tmp == target) return;
      }
   }

}

static const unsigned int TBox[16] = {
   14,  0,  4, 15, 13,  7,  1,  4,  2, 14, 15,  2, 11, 13,  8,  1
};

int getID(){
   static int i = 1;
   return i++;
}

void backtrace(int tag, long pos){
   int tagID1, tagID2;
   tagID1 = getID();
   tagID2 = getID();
   char tag1[1024], tag2[1024];

   if(traO[pos] != 'a' && (traO[traA[pos]] == 'a'))
      sprintf(tag1, "a%ld", traA[traA[pos]]);
   else
      sprintf(tag1, "x%d", tagID1);

   if(traO[pos] != 'a' && (traO[traB[pos]] == 'a'))
      sprintf(tag2, "a%ld", traA[traB[pos]]);
   else
      sprintf(tag2, "x%d", tagID2);
      

   assert(pos >= 0);
   switch(traO[pos]){
      case '&':
      case '|':
      case '^':
         if(traO[traA[pos]] != 'a')
            backtrace(tagID1, traA[pos]);
         if(traO[traB[pos]] != 'a')
            backtrace(tagID2, traB[pos]);
         printf("x%d = %s %c %s \n", tag, tag1, traO[pos], tag2);
         break;
      case '~':
         if(traO[traA[pos]] != 'a')
            backtrace(tagID1, traA[pos]);
         printf("x%d = ~%s \n", tag, tag1);
         break;
      case 'a':
         printf("x%d = a[%ld]\n", tag, traA[pos]); 
         break;
      default:
         assert( false );
   }

}

void usage(char* progName){
   printf("usage: %s [-s sbox_file] sboxBit.h sboxArr.h\n", progName);
   printf("       use sbox_file to generate BitSlicing and Array version of implementation\n");
   printf("       if no sbox_file is specified, use random permutation.\n");
   exit(-1);
}

int main(int argc, char* argv[]){

   for(int i = 3; i >= 0; --i){
      word target = 0;
      for(int k = 0; k < 16; ++k)
         target |= ((TBox[k] >> i) & 1) << k;

      find(target);

      backtrace(getID(), count-1);
      printf("----------------------------------\n");
   }

}
