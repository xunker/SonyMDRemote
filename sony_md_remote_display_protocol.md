# Notes about the display protocol used in the MZ-NHF800 Minidisc player

## Matthew Nielsen, June 2020

The data line is pulled-up to 2.1v and the protocol is active-low.

Each "frame" begins with data line being pulled low for ~1100us and then high
for another 1000us.


### noodling

ASSUMPTIONS:
Based off https://web.archive.org/web/20090217092709/http://home.no.net/~htoerrin/md_if/md_spek.htm,
assume the first bit after the frame start marker is a "0".



_1014 -911 _205 -5 _206 -4 _207 -3 _206 -4 _207 -4 _207 -3 _207 -4 _207 -3 _207 -260 _3 -3 _207 -5 _207 -3 _208 -3 _206 -5 _207 -3 _206 -209 _3 -33832
_1010 -904 _205 -4 _206 -4 _206 -4 _206 -4 _206 -3 _207 -3 _207 -5 _207 -3 _207 -260 _3 -3 _207 -5 _207 -5 _207 -3 _207 -4 _206 -5 _207 -208 _3 -33837

_1014-911 0 0 0 0 0 0 0 0 1 D 0 0 0 0 0 1 D
_1010-904 0 0 0 0 0 0 0 0 1 D 0 0 0 0 0 1 D

what if:
* long low, short high == 1
* long low, long high == 0
* low for _3 is a byte delimiter? and the high period after it doesn't care?

But, what if the high time doesn't matter? What if it's all about the low time?
* what if long low time is 0, short _3 is the stop-bit?
_1014-911 0000000001000000 D -33832
_1014-911 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 1 D
_1010-904 0 0 0 0 0 0 0 0 1 D 0 0 0 0 0 1 D

### Record stop is a new frame sync?

Look at due_47k_pulldown, this is only the length of the low signal times in uS.

Whenever you get a long line like the first one here, it is always followed by and empty "beginning of frame" marker:
```
1028 222 219 219 220 221 219 218 220 221 219 220 218 219 220 221 219 16 18 218 220 220 222 219 220 219 16 220 219 222 219 220 219 220 222 219 220 222 219 220 219 221 221 219 221 219 220 219 220 219 219 219 220 219 219 220 219 220 221 219 220 220 219 221 219 220 219 219 219 220 219 221 219 220 221 219 219 220 219 221 219 221 219 219 220 219 220 219 219 220 221 220 220 219 220 219 220 219 219 219 220 219 219 221 219 220
1015
```

On above, if you exclude the first bit, the remainding bits (104) evenly divisible by 8.
