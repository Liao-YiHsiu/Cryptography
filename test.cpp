#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

using namespace std;

// one 4x4 sbox
void sboxBit(unsigned long a[4], unsigned long out[4]);
int sboxArr(int in);


struct timeval	start_tv, end_tv;

void tic(){
   gettimeofday (&start_tv, NULL);
}

double toc(){
   gettimeofday (&end_tv, NULL);

   int t;
   double td;

   t = (end_tv.tv_sec - start_tv.tv_sec) * 1000000
      + (end_tv.tv_usec - start_tv.tv_usec);
   td = (double) t / 1000000.0;
   return td;
}

int main(int argc, char* argv[]){
   long n = 1L << 31;
   double time;

   tic();
   for(long i = 0; i < n/64; ++i)
      sboxBit();
   time = toc();
   printf("BitSlicing: %lf times per second.\n", n/time);

   tic();
   for(long i = 0; i < n; ++i)
      sboxArr();
   time = toc();
   printf("Array: %lf times per second.\n", n/time);


   return 0;
}
