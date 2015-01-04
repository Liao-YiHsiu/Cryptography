#!/usr/bin/python
import random
import math
import copy

def qGcd(a, b):
   shift = 0;
   u = a;
   v = b;
   while 1 == 1:
      a = u;
      b = v;

      if b > a:
         u = b;
         v = a;
         continue;
      if b == 0:
         break;
      
      if a & 1:
         if b & 1:
            u = (a - b) >> 1;
            v = b;
         else:
            u = a - b;
            v = b >> 1;
      else:
         if b & 1:
            u = a >> 1;
            v = b;
         else:
            shift = shift+1;
            u = (a - b) >> 1;
            v = b >> 1;
   return a << shift;

def gcd(a, b):
   arr = [a, b, 0];
   index = 0;
   while 1 == 1:
      index2 = (index+2)%3;
      index1 = (index+1)%3;
      arr[index2] = arr[index] % arr[index1];
      if arr[index2] == 0:
         return arr[index1];
      index = index1;

def inverse(a, modN):
   if a == 1:
      return 1;
   elif a == modN-1:
      return modN-1;
   arr=[[modN, 0, 1, 0], [a, 0, 0, 1], [0, 0, 0, 0]];
   index=0;
   while 1 == 1:
      index2 = (index+2)%3;
      index1 = (index+1)%3;
      arr[index2][0] = arr[index][0] % arr[index1][0];
      arr[index2][1] = arr[index][0] / arr[index1][0];
      arr[index2][2] = arr[index][2] - arr[index2][1] * arr[index1][2];
      arr[index2][3] = arr[index][3] - arr[index2][1] * arr[index1][3];
      if arr[index2][0] == 1:
         break;
      if arr[index2][0] == 0:
         return 0;
      index = index1;

   return arr[(index+2)%3][3] % modN;

# use double and add
def power(a, b, modN):
   inv = 0;
   if b < 0:
      inv = 1;
      b = -b;
   ans = 1;
   base = a;
   while b != 0:
      if b & 1:
         ans = ans * base % modN;
      base = base * base % modN;
      b = b >> 1;

   if inv == 1:
      ans = inverse(ans, modN);
   return ans;

def miller_rabin(n, a):

   if gcd(n, a) != 1:
      return 0;
   # compute k, q
   k = 0;
   q = n - 1;
   while (q & 1) == 0:
      k = k + 1;
      q = q >> 1;
   a = power(a, q, n);
   
   if a == 1:
      return 1;

   pre = a;
   for i in xrange(0, k):
      if a == n-1:
         return 1;
      if a == 1:
         return 0;
      pre = a;
      a = a*a %n;
   return 0;

def testPrime(n):
   for i in xrange(0, 100):
      a = random.randint(2, n-1);
      if miller_rabin(n, a) == 0:
         return 0;
   return 1;

def pollard(n, max_range=100000):
   a=2;
   for j in xrange(2, max_range):
      a = power(a, j, n);
      d = gcd(a-1, n);
      if d > 1 and d < n:
         print n, ' = ', d, ' * ', n/d
         return;

def factor(n):
   sqrt = int(math.sqrt(n));
   for j in xrange(2, sqrt+1):
      if n % j == 0:
         return j, factor(n/j);
   return n;

#
class GF:
   def __init__(self, a, N = 2):
      assert (N > 0), "Modulus is zero!"
      self.a = a%N;
      self.N = N;

   def str(self):
      #return str(self.a);
      return "{0} (mod {1})".format(self.a, self.N)

   def __str__(self):
      return self.str();
   def __repr__(self):
      return self.str();

   def __add__(self, other):
      if isinstance(other, GF):
         assert (self.N == other.N), "Modulus different!!!"
         return GF((self.a + other.a)%self.N, self.N);
      elif isinstance(other, (int, long)):
         return GF((self.a + other)%self.N, self.N);
      else:
         return NotImplemented;
   def __radd__(self, other):
      if isinstance(other, (int, long)):
         return GF((self.a + other)%self.N, self.N);
      else:
         return NotImplemented;

   def __sub__(self, other):
      if isinstance(other, GF):
         assert (self.N == other.N), "Modulus different!!!"
         return GF((self.a - other.a)%self.N, self.N);
      elif isinstance(other, (int, long)):
         return GF((self.a - other)%self.N, self.N);
      else:
         return NotImplemented;
   def __rsub__(self, other):
      if isinstance(other, (int, long)):
         return GF((other - self.a)%self.N, self.N);
      else:
         return NotImplemented;

   def __mul__(self, other):
      if isinstance(other, GF):
         assert (self.N == other.N), "Modulus different!!!"
         return GF((self.a * other.a)%self.N, self.N);
      elif isinstance(other, (int, long)):
         return GF((self.a * other)%self.N, self.N);
      else:
         return NotImplemented;
   def __rmul__(self, other):
      if isinstance(other, (int, long)):
         return GF((other * self.a)%self.N, self.N);
      else:
         return NotImplemented;

   def __div__(self, other):
      if isinstance(other, GF):
         assert (self.N == other.N), "Modulus different!!!"
         inv = inverse(other.a, self.N);
         return GF((self.a * inv)%self.N, self.N);
      elif isinstance(other, (int, long)):
         inv = inverse(other, self.N);
         return GF((self.a * inv)%self.N, self.N);
      else:
         return NotImplemented;
   def __rdiv__(self, other):
      if isinstance(other, (int, long)):
         inv = inverse(self.a, self.N);
         return GF((other * inv)%self.N, self.N);
      else:
         return NotImplemented;

   def __pow__(self, other):
      if isinstance(other, GF):
         return GF(power(self.a, other.a, self.N), self.N);
      elif isinstance(other, (int, long)):
         return GF(power(self.a, other, self.N), self.N);
      else:
         return NotImplemented;
 
   def __neg__(self):
      return GF(self.N - self.a, self.N);

   def __eq__(self, other):
      if isinstance(other, GF):
         assert (self.N == other.N), "Modulus different!!!"
         return self.a == other.a; 
      elif isinstance(other, (int, long)):
         return self.a == other;
      else:
         return NotImplemented;

# Point in E: Y^2 = X^3 + A*X + B
class Point:
   def __init__(self, x, y, A, B, zero=False):
      self.x = x;
      self.y = y;
      self.A = A;
      self.B = B;
      self.zero = zero;
      assert (y**2 == x**3 + A*x + B or self.zero), "Not in the ecllipse"
   def __eq__(self, other):
      if isinstance(other, Point):
         assert (self.A == other.A and self.B == other.B), "Not in the ecllipse";
         return self.x == other.x and self.y == other.y;
      elif isinstance(other, (int, long)) and other == 0:
          return self.zero;
      else:
         return NotImplemented;

   def str(self):
      if self.zero:
         return "(O)"
      else:
         return "({0}, {1})".format(self.x, self.y);
   def __str__(self):
      return self.str();
   def __repr__(self):
      return self.str();

   def add(self, other):
      if isinstance(other, Point):
         if self.zero:
            return other;
         elif other.zero:
            return self;
         elif self == -other:
            return Point(0, 0, self.A, self.B, True);
         else:
            if self == other:
               landa = (3*self.x*self.x + self.A)/(2*self.y);
            else:
               landa = (self.y - other.y)/(self.x - other.x);
            x3 = landa*landa - self.x - other.x;
            y3 = landa*(self.x - x3) - self.y;
            return Point(x3, y3, self.A, self.B);
      elif isinstance(other, (int, long)) and other == 0:
         return self;
      else:
         return NotImplemented;

   def mul(self, other):
      if isinstance(other, (int, long)):
         #use double and add
         inv = 0;
         if other < 0:
            inv = 1;
            other = -other;
         ans = Point(GF(0, self.x.N), GF(0, self.x.N), self.A, self.B, True);
         base = copy.deepcopy(self);
         while other != 0:
            if other & 1:
               ans = ans + base;
            base = base + base;
            other = other >> 1;
         if inv == 1:
            ans = -ans;
         return ans;
      else:
         return NotImplemented;

   def __radd__(self, other):
      return self.add(other);
   def __add__(self, other):
      return self.add(other);

   def __sub__(self, other):
      return self.add(-other);
   
   def __mul__(self, other):
      return self.mul(other);
   def __rmul__(self, other):
      return self.mul(other);

   def __neg__(self):
      return Point(self.x, -self.y, self.A, self.B, self.zero);

# compute gP,Q(X)
def em_g(P, Q, X):
   assert isinstance(P, Point), "Not Point"
   assert isinstance(Q, Point), "Not Point"
   assert isinstance(X, Point), "Not Point"
   if P == -Q:
      return X.x - P.x;
   elif P == Q:
      landa = (3*P.x*P.x + P.A)/(2*P.y);
   else:
      landa = (P.y - Q.y)/(P.x - Q.x);
   return (X.y-P.y-landa*(X.x-P.x))/(X.x + P.x + Q.x - landa*landa);

# compute fmP(S)
def em_f(m, P, S):
   T = copy.deepcopy(P);
   f = 1;
   bits = "{0:b}".format(m)[1:];

   for bit in bits:
      f = f*f*em_g(T, T, S);
      T = T + T;
      if bit == '1':
         f = f*em_g(T, P, S);
         T = T + P;

   return f;

# compute em(P, Q, S) //given S
def weil_pairing(m, P, Q, S):
   return em_f(m, P, Q+S)*em_f(m, Q, -S)/(em_f(m, P, S)*em_f(m, Q, P-S));

