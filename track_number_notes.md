  track 2       track 2       track 4
    209           209     *      4      *
    -2            -3      *    -208     *
     5             6      *     209     *
   -208          -208     *     -2      *


    209           210     *      5      *
    -2            -2      *    -209     *
     6             6      *     209     *
   -208          -208     *     -2      *

 2 is b00000010
 4 is b00000100
12 is b00001100
72 is b01001000

first difference starts at transition index 102 (counted from 1)
second difference starts at transition index 198 (counted from 1)

This causes track display to show "0":

+6,//+209, //
-209,//-3,   //
+6,   //
-208, //
+6,   // decimal?
-208, // decimal?
+4,   // ten
-209, // ten
+4,   // twenty
-207, // twenty
+5,   // forty
-208, // forty
...
+6,//+210, //
-208,//-2,   //
+6,   //
-208, //
+7,   // decimal?
-208, // decimal?
+5,   // ten
-208, // ten
+209, // twenty
-3,   // twenty
+6,   // forty
-208, // forty

This causes track display to show "08:
+6,//+209, //
-209,//-3,   //
+6,   //
-208, //
+208,//+6,   // decimal?
-6,//-208, // decimal?
+4,   // ten
-209, // ten
+4,   // twenty
-207, // twenty
+5,   // forty
-208, // forty
...
+6,//+210, //
-208,//-2,   //
+6,   //
-208, //
+208,//+7,   // decimal?
-7,//-208, // decimal?
+5,   // ten
-208, // ten
+209, // twenty
-3,   // twenty
+6,   // forty
-208, // forty

This change causes the track display to show "12":

+209, //
-3,   //
+6,   //
-208, //
+6,   // decimal?
-208, // decimal?
+209,//+4,
-4,//-209,
...
+210, //
-2,   //
+6,   //
-208, //
+7,   // decimal?
-208, // decimal?
+208,//+5,
-5,//-208,

This causes track display to show "52":

+209, //
-3,   //
+6,   //
-208, //
+6,   // decimal?
-208, // decimal?
+209,//+4,   // ten
-4,//-209, // ten
+4,   // twenty
-207, // twenty
+208,//+5,   // forty
-5,//-208, // forty
...
+210, //
-2,   //
+6,   //
-208, //
+7,   // decimal?
-208, // decimal?
+208,//+5,   // ten
-5,//-208, // ten
+209, // twenty
-3,   // twenty
+208,//+6,   // forty
-6,//-208, // forty

This causes track display to show "72":

+209, //
-3,   //
+6,   //
-208, //
+6,   // decimal?
-208, // decimal?
+209,//+4,   // ten
-4,//-209, // ten
+207,//+4,   // twenty
-4,//-207, // twenty
+208,//+5,   // forty
-5,//-208, // forty
...
+210, //
-2,   //
+6,   //
-208, //
+7,   // decimal?
-208, // decimal?
+208,//+5,   // ten
-5,//-208, // ten
+3,//+209, // twenty
-209,//-3,   // twenty
+208,//+6,   // forty
-6,//-208, // forty

This causes track display to show "76":

+209, //
-3,   //
+208,//+6,   //
-6,//-208, //
+6,   // decimal?
-208, // decimal?
+209, //+4,   // ten
-4,   //-209, // ten
+207, //+4,   // twenty
-4,   //-207, // twenty
+208, //+5,   // forty
-5,   //-208, // forty
...
+210, //
-2,   //
+208,//+6,   //
-6,//-208, //
+7,   // decimal?
-208, // decimal?
+208, //+5,   // ten
-5,   //-208, // ten
+3,   //+209, // twenty
-209, //-3,   // twenty
+208, //+6,   // forty
-6,   //-208, // forty

This causes track display to show "7-" (seven-hyphen):

+209, //
-3,   //
+208,//+6,   //
-6,//-208, //
+208,//+6,   // decimal?
-6,//-208, // decimal?
+209, //+4,   // ten
-4,   //-209, // ten
+207, //+4,   // twenty
-4,   //-207, // twenty
+208, //+5,   // forty
-5,   //-208, // forty
...
+210, //
-2,   //
+208,//+6,   //
-6,//-208, //
+208,//+7,   // decimal?
-7,//-208, // decimal?
+208, //+5,   // ten
-5,   //-208, // ten
+3,   //+209, // twenty
-209, //-3,   // twenty
+208, //+6,   // forty
-6,   //-208, // forty

This causes track display to show "0≡" (zero-triple-equals):

+6,//+209, //
-209,//-3,   //
+208,//+6,   //
-6,//-208, //
+208,//+6,   // decimal?
-6,//-208, // decimal?
+4,   // ten
-209, // ten
+4,   // twenty
-207, // twenty
+5,   // forty
-208, // forty
...
+6,//+210, //
-208,//-2,   //
+208,//+6,   //
-6,//-208, //
+208,//+7,   // decimal?
-7,//-208, // decimal?
+5,   // ten
-208, // ten
+209, // twenty
-3,   // twenty
+6,   // forty
-208, // forty

This causes track display to show "1≡" (one-triple-equals):
+6,//+209, //
-209,//-3,   //
+208,//+6,   //
-6,//-208, //
+208,//+6,   // decimal?
-6,//-208, // decimal?
+209,//+4,   // ten
-5,//-209, // ten
+4,   // twenty
-207, // twenty
+5,   // forty
-208, // forty
...
+6,//+210, //
-208,//-2,   //
+208,//+6,   //
-6,//-208, //
+208,//+7,   // decimal?
-7,//-208, // decimal?
+208,//+5,   // ten
-5,//-208, // ten
+209, // twenty
-3,   // twenty
+6,   // forty
-208, // forty

This causes track display to show "-0" (hyphen-zero) - NOTE DIFFERENT TRANSPOSITION ON FINAL BIT!:

+6,//+209, //
-209,//-3,   //
+6,   //
-208, //
+6,   // decimal?
-208, // decimal?
+4,   // ten
-209, // ten
+207,//+4,   // twenty
-4,//-207, // twenty
+208,//+5,   // forty
-5,//-208, // forty
+208,//+5, // hypen tens
-5,//-208, // hyphen tens
...
+6,//+210, //
-208,//-2,   //
+6,   //
-208, //
+7,   // decimal?
-208, // decimal?
+5,   // ten
-208, // ten
+5,//+209, // twenty
-209,//-3,   // twenty
+208,//+6,   // forty
-6,//-208, // forty
+5,//+208, // hyphen tens
-208,//-3, // hyphen tens

This works to show "10". Note that the high pulse of hyphen tens agrees, but low pulse doesn't

+5,//+208,
-209, // final bit of one?
+6,   //+209, //
-209, //-3,   //
+6,   //
-208, //
+6,   // decimal?
-208, // decimal?
+209, //+4,   // ten
-5,   //-209, // ten
+4,   // twenty
-207, // twenty
+5,   // forty
-208, // forty
+5,   // hypen tens
-208, // hyphen tens
...
+5,//+123,
-207, // final bit of one?
+6,   //+210, //
-208, //-2,   //
+6,   //
-208, //
+7,   // decimal?
-208, // decimal?
+208, //+5,   // ten
-5,   //-208, // ten
+209, // twenty
-3,   // twenty
+6,   // forty
-208, // forty
+5,   //+208, // hyphen tens
-5,//-208, //-3, // hyphen tens

This works to show "11". Note that the high pulse of hyphen tens agrees, but low pulse doesn't:

+208,
-5,//-209, // final bit of one?
+6,   //+209, //
-209, //-3,   //
+6,   //
-208, //
+6,   // decimal?
-208, // decimal?
+209, //+4,   // ten
-5,   //-209, // ten
+4,   // twenty
-207, // twenty
+5,   // forty
-208, // forty
+5,   // hypen tens
-208, // hyphen tens
...
+123,
-5,//-207, // final bit of one?
+6,   //+210, //
-208, //-2,   //
+6,   //
-208, //
+7,   // decimal?
-208, // decimal?
+208, //+5,   // ten
-5,   //-208, // ten
+209, // twenty
-3,   // twenty
+6,   // forty
-208, // forty
+5,   //+208, // hyphen tens
-5,//-208, //-3, // hyphen tens
----

track
shown

0     +6    -209    +6    -208    +6   -208   +4    -209    +4    -207    +5    -208  +5  -208
1
2     +209  -3      +6    -208    +6   -208   +4    -209    +4    -207    +5    -208  +5  -208
4     +4    -208    +209  -2      +6   -208   +4    -209    +4    -207    +5    -208  +5  -208
5
6
7
8     +6    -209    +6    -208    +208 -6     +4    -209    +4    -207    +5    -208  +5  -208
9
10
11
12    +209  -3      +6    -208    +6   -208   +209  -4      +4    -207    +5    -208  +5  -208
18    +6    -209    +6    -208    +208 -6     +209  -5      +4    -207    +5    -208  +5  -208
20    +6    -209    +6    -208    +6   -208   +4    -209    +207  -4      +5    -208  +5  -208
30    +6    -209    +6    -208    +6   -208   +209  -5      +207  -4      +5    -208  +5  -208
38    +6    -209    +6    -208    +208 -6     +209  -5      +207  -4      +5    -208  +5  -208
52    +209  -3      +6    -208    +6   -208   +4    -209    +4    -207    +208  -5    +5  -208
60    +6    -209    +6    -208    +6   -208   +4    -209    +207  -4      +208  -5    +5  -208
72    +209  -3      +6    -208    +6   -208   +209  -4      +207  -4      +208  -5    +5  -208
76    +209  -3      +208  -6      +6   -208   +209  -4      +207  -4      +208  -5    +5  -208
7-    +209  -3      +208  -6      +208 -6     +209  -4      +207  -4      +208  -5    +5  -208
0≡    +6    -209    +208  -6      +208 -6     +4    -209    +4    -207    +5    -208  +5  -208
1≡    +6    -209    +208  -6      +208 -6     +209  -5      +4    -207    +5    -208  +5  -208

0     0             0             0           0             0             0           0
1
2     1             0             0           0             0             0           0
3
4     0             1             0           0             0             0           0
5
6
7
8     0             0             1           0             0             0           0
9
10
11
12    1             0             0           1             0             0           0
18    0             0             1           1             0             0           0
20    0             0             0           0             1             0           0
30    0             0             0           1             1             0           0
38    0             0             1           1             1             0           0
52    1             0             0           0             0             1           0
60    0             0             0           0             1             1           0
72    1             0             0           1             1             1           0
76    1             1             0           1             1             1           0
7-    1             1             1           1             1             1           0
0≡    0             1             1           0             0             0           0
1≡    0             1             1           1             0             0           0

---

Two digits, each as Binary Coded Decimal with LSB transmitted FIRST.

ones column binary
0   0000
1   1000
2   0100
3   1100
4   0010
5   1010
6   0110
7   1110
8   0001
9   1001
_   0101 (10)
    1101 (11)
    0011 (12)
    1011 (13)
≡   0111 (14)
-   1111 (15)


tens column binary

0   0000
1   1000
2   0100
3   1100
4   0010
5   1010
6   0110
7   1110
8   0001
9   1001
_   0101 (10)
    1101 (11)
    0011 (12)
    1011 (13)
≡   0111 (14)
-   1111 (15)

