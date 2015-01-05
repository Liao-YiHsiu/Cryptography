int sboxArr(int in){
   static int sbox[16] = {
      2, 7, 6, 10, 9, 4, 1, 5, 11, 15, 13, 14, 0, 12, 8, 3
   };
   return sbox[in];
}
