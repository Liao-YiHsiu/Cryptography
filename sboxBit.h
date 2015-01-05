void sboxBit(unsigned long a[4], unsigned long out[4]){
   static unsigned long x[1000];
   x[2] = a[0] | a[1];
   x[1] = a[2] & x[2];
   x[4] = a[0] & a[1];
   x[5] = a[2] ^ a[3];
   x[3] = x[4] | x[5];
   x[0] = x[1] ^ x[3];
   out[3] = x[0]; 
   x[7] = a[1] & a[2];
   x[9] = a[0] ^ a[1];
   x[10] = a[0] & a[3];
   x[8] = x[9] | x[10];
   x[6] = x[7] ^ x[8];
   out[2] = x[6]; 
   x[12] = ~a[0]; 
   x[14] = a[0] ^ a[2];
   x[15] = a[1] & a[3];
   x[13] = x[14] | x[15];
   x[11] = x[12] ^ x[13];
   out[1] = x[11]; 
   x[18] = a[0] | a[3];
   x[17] = x[4] ^ x[18];
   x[16] = a[2] ^ x[17];
   out[0] = x[16]; 
}
