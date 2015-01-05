#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fstream>

// IBIT should be smaller than or equal to 6
#define IBIT 4 
#define OBIT 4
#define IALL (1 << IBIT) 
#define OALL (1 << OBIT)
#define MAX (1 << IALL)
using namespace std;

typedef unsigned long long word;
word one = 1;
word mask = MAX-1;

word* arr = NULL;
char* map = NULL;
char* traO = NULL;
long* traA = NULL;
long* traB = NULL;
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

   printf("target = 0b");
   print2(target);

   // a1 ~ a6
   word a[IBIT] = {0};
   for(word i = 0; i < IALL ; ++i)
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
         tmp = (~arr[i]) & mask;
         push(tmp, i, i, '~');
         if(tmp == target) return;
      }
   }
}

void finds(word target[OBIT]){
   for(int i = 0; i < OBIT; ++i){
      find(target[i]);
   }
}

int getID(){
   static int i = 1;
   return i++;
}

void backtrace(FILE *fd, int tag, long pos){
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
            backtrace(fd, tagID1, traA[pos]);
         if(traO[traB[pos]] != 'a')
            backtrace(fd, tagID2, traB[pos]);
         fprintf(fd, "x%d = %s %c %s \n", tag, tag1, traO[pos], tag2);
         break;
      case '~':
         if(traO[traA[pos]] != 'a')
            backtrace(fd, tagID1, traA[pos]);
         fprintf(fd, "x%d = ~%s \n", tag, tag1);
         break;
      case 'a':
         fprintf(fd, "x%d = a[%ld]\n", tag, traA[pos]); 
         break;
      default:
         assert( false );
   }
}

void usage(char* progName){
   printf("usage: %s [-s sbox_file] sboxBit.h sboxArr.h\n", progName);
   printf("\n");
   printf("  Use sbox_file to generate BitSlicing and Array version of implementation\n");
   printf("  If no sbox_file is specified, use random permutation.\n");
   exit(-1);
}

int main(int argc, char* argv[]){

   const char* bitFn = NULL;
   const char* arrFn = NULL;
   const char* sbxFn = NULL;

   FILE* bitFILE = NULL;
   FILE* arrFILE = NULL;

   word SBOX[IALL];
   word target[OBIT] = {0};

   srand(time(NULL));

   // parse arguements
   for(int i = 1; i < argc; ++i){
      if(argv[i][0] == '-'){
         if(argv[i][1] == 's'){
            if(i + 1 >= argc) usage(argv[0]);
            sbxFn = argv[i+1]; 
            ++i;
            continue;
         } else
            usage(argv[0]);
      }else if(bitFn == NULL)
         bitFn = argv[i];
      else if(arrFn == NULL)
         arrFn = argv[i];
      else
         usage(argv[0]);
   }
   bitFILE = fopen(bitFn, "w");
   arrFILE = fopen(arrFn, "w");
   if(bitFILE == NULL || arrFILE == NULL)
      usage(argv[0]);

   // there's sbox 
   if(sbxFn){
      ifstream fin(sbxFn);
      for(word i = 0; i < IALL; ++i){
         if(!(fin >> SBOX[i])) usage(argv[0]);
         if(SBOX[i] > OALL) usage(argv[0]);
      }

   }else{
      for(word i = 0; i < IALL; ++i)
         SBOX[i] = i%OALL;

      word tmp;
      word a;
      for(word i = 0; i < IALL; ++i){
         a = rand() % IALL;
         tmp = SBOX[a];
         SBOX[a] = SBOX[i];
         SBOX[i] = tmp;
      }
   }

   // writeArray version
   {
      fprintf(arrFILE, "int sboxArr(int in){\n");
      fprintf(arrFILE, "   static int sbox[%d] = {\n", IALL);
      fprintf(arrFILE, "      ");
      for(word i = 0; i < IALL-1; ++i)
         fprintf(arrFILE, "%lld, ", SBOX[i]);
      fprintf(arrFILE, "%lld\n", SBOX[IALL-1]);
      fprintf(arrFILE, "   };\n");
      
      fprintf(arrFILE, "   return sbox[in];\n");
      fprintf(arrFILE, "}\n");
   }

   // write BitSlice version

   {
      fprintf(bitFILE, "void sboxBit(unsigned long a[4], unsigned long out[4]){\n");
   }
   // roll all SBOX into target
   for(int i = 0; i < OBIT; ++i)
      for(word k = 0; k < IALL; ++k)
         target[i] |= ((SBOX[k] >> i) & one) << k;

   finds(target);
   find(target[2]);
   backtrace(stdout, getID(), count-1);

}
