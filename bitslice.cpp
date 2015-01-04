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

void find(word target){

   init();
   printf("target = ");
   print2(target);

   // a1 ~ a6
   word one = 1;
   word a[IBIT] = {0};
   for(int i = 0; i < IALL ; ++i)
      for(int j = 0; j < IBIT; ++j)
         a[j] |= ((i >> j) & one) << i; 

   for(int i = 0; i < IBIT; ++i){
      printf("a%d = ", i);
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

static const unsigned int SBox[8][64] =
{
   {  /* S0 */
      14,  0,  4, 15, 13,  7,  1,  4,  2, 14, 15,  2, 11, 13,  8,  1,
      3, 10, 10,  6,  6, 12, 12, 11,  5,  9,  9,  5,  0,  3,  7,  8,
      4, 15,  1, 12, 14,  8,  8,  2, 13,  4,  6,  9,  2,  1, 11,  7,
      15,  5, 12, 11,  9,  3,  7, 14,  3, 10, 10,  0,  5,  6,  0, 13
   },
   {  /* S1 */
      15,  3,  1, 13,  8,  4, 14,  7,  6, 15, 11,  2,  3,  8,  4, 14,
      9, 12,  7,  0,  2,  1, 13, 10, 12,  6,  0,  9,  5, 11, 10,  5,
      0, 13, 14,  8,  7, 10, 11,  1, 10,  3,  4, 15, 13,  4,  1,  2,
      5, 11,  8,  6, 12,  7,  6, 12,  9,  0,  3,  5,  2, 14, 15,  9
   },
   {  /* S2 */
      10, 13,  0,  7,  9,  0, 14,  9,  6,  3,  3,  4, 15,  6,  5, 10,
      1,  2, 13,  8, 12,  5,  7, 14, 11, 12,  4, 11,  2, 15,  8,  1,
      13,  1,  6, 10,  4, 13,  9,  0,  8,  6, 15,  9,  3,  8,  0,  7,
      11,  4,  1, 15,  2, 14, 12,  3,  5, 11, 10,  5, 14,  2,  7, 12
   },
   {  /* S3 */
      7, 13, 13,  8, 14, 11,  3,  5,  0,  6,  6, 15,  9,  0, 10,  3,
      1,  4,  2,  7,  8,  2,  5, 12, 11,  1, 12, 10,  4, 14, 15,  9,
      10,  3,  6, 15,  9,  0,  0,  6, 12, 10, 11,  1,  7, 13, 13,  8,
      15,  9,  1,  4,  3,  5, 14, 11,  5, 12,  2,  7,  8,  2,  4, 14
   },
   {  /* S4 */
      2, 14, 12, 11,  4,  2,  1, 12,  7,  4, 10,  7, 11, 13,  6,  1,
      8,  5,  5,  0,  3, 15, 15, 10, 13,  3,  0,  9, 14,  8,  9,  6,
      4, 11,  2,  8,  1, 12, 11,  7, 10,  1, 13, 14,  7,  2,  8, 13,
      15,  6,  9, 15, 12,  0,  5,  9,  6, 10,  3,  4,  0,  5, 14,  3
   },
   {  /* S5 */
      12, 10,  1, 15, 10,  4, 15,  2,  9,  7,  2, 12,  6,  9,  8,  5,
      0,  6, 13,  1,  3, 13,  4, 14, 14,  0,  7, 11,  5,  3, 11,  8,
      9,  4, 14,  3, 15,  2,  5, 12,  2,  9,  8,  5, 12, 15,  3, 10,
      7, 11,  0, 14,  4,  1, 10,  7,  1,  6, 13,  0, 11,  8,  6, 13
   },
   {  /* S6 */
      4, 13, 11,  0,  2, 11, 14,  7, 15,  4,  0,  9,  8,  1, 13, 10,
      3, 14, 12,  3,  9,  5,  7, 12,  5,  2, 10, 15,  6,  8,  1,  6,
      1,  6,  4, 11, 11, 13, 13,  8, 12,  1,  3,  4,  7, 10, 14,  7,
      10,  9, 15,  5,  6,  0,  8, 15,  0, 14,  5,  2,  9,  3,  2, 12
   },
   {  /* S7 */
      13,  1,  2, 15,  8, 13,  4,  8,  6, 10, 15,  3, 11,  7,  1,  4,
      10, 12,  9,  5,  3,  6, 14, 11,  5,  0,  0, 14, 12,  9,  7,  2,
      7,  2, 11,  1,  4, 14,  1,  7,  9,  4, 12, 10, 14,  8,  2, 13,
      0, 15,  6, 12, 10,  9, 13,  0, 15,  3,  3,  5,  5,  6,  8, 11
   }
};

int getID(){
   static int i = 1;
   return i++;
}

void backtrace(int tag, long pos){
   int tag1, tag2;
   assert(pos >= 0);
   switch(traO[pos]){
      case '&':
      case '|':
      case '^':
         tag1 = getID();
         tag2 = getID();
         backtrace(tag1, traA[pos]);
         backtrace(tag2, traB[pos]);
         printf("x%d = x%d %c x%d \n", tag, tag1, traO[pos], tag2);
         break;
      case '~':
         tag1 = getID();
         backtrace(tag1, traA[pos]);
         printf("x%d = ~x%d \n", tag, tag1);
         break;
      case 'a':
         printf("x%d = a%ld\n", tag, traA[pos]); 
         break;
      default:
         assert( false );
   }

}


int main(int argc, char* argv[]){

   for(int i = 0; i < 4; ++i){
      word target = 0;
      for(int k = 0; k < 16; ++k)
         target |= ((TBox[k] >> i) & 1) << k;

      find(target);

      backtrace(getID(), count-1);
      printf("----------------------------------\n");
   }
   // for 8 s-box
//   for(int i = 0; i < 8; i++){
//      for(int j = 0; j < 4; j++){
//         unsigned long target = 0;
//         for(int k = 0; k < 64; k++){
//            target |= ((SBox[i][k] >> j) & 1) << k;
//         }
//
//         find(target);
//      }
//
//   }


}
