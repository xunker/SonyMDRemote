Sony WM-FX855 Cassette Walkman
==============================

## Remote Connector Differences

This player (and possibly other Cassette Walkman players) is slightly different than the average connector:

1) Ground (same)
2) Remote In (this pin is *pulled-up* to about 2.7v, and remote control buttons pull it to ground through a resistor network unlike the average connector which is pulled to Vcc via a resistor network)
3) Data Out
4) 1.4v (different voltage; connection to battery, usually it is to regulated power)


Remote gives 0.227meg between pins 1 and 3 while connected to player

While connected to player, oscope connected between 1 and 3 shows digital pulses when a button on remote or player is pressed, or when the player performs and action like switching to play after blank-skip.

Signal is active-high, about 1.35v for high.

When Next Preset pressed, 24-bit message sent followed by 8-bit message about 60ms after start of previous.

Assume 2ms high followed by 400us low is 1, and 2ms low followed by 400us high 0.

After initial 8 bits, there is an additional low of 2.5ms before the 16 bit payload (of applicable)

Message prefixed by high pulse of 5ms followed by low of 2ms.
  t1 = 5ms (2.5 * t2)
  t2 = 2ms (40% of t1)
  t2 = 400us (1/5 of t1)

Next preset 24-bit message: 11000010 0000000001110110 (100.7fm, preset 2)
                            11000001 1001011101010010 (97.5fm, preset 1)
                            11000011 0000000100010110 (101.1fm, preset 3)
                            11000100 0000000101010110 (101.5fm, preset 4)

Band switch                 11000001 0101100000000000 (580am, preset 1)
                            11000010 1000000000000000 (800am, preset 2)
                            11000011 0001011000000100 (1160am, preset 3)
                            11000101 0100001100000100 (1430am, preset 5)

Manual tuning               11000000 0100010000000100 (1440am, no preset)
                            11000000 0100010100000100 (1450am, no preset)
                            11000000 0111000000000100 (1700am, no preset)
                            11000000 0111000100000100 (1710am, no preset)

(I think the current preset number is the last nibble of the first byte, as BCD)

16-bit payload structure for radio:
  1001011101010010 (97.5fm, preset 1)
    1001  9
    0111  7
    0101  5
    00    0
    10    FM

    0..3  tens
    4..7  ones
    8..11 decimal
    12..13  hundreds
    14..15  band indicator (10 for FM, 00 for AM)

  0000000001110110 (100.7fm, preset 2)
    0000
    0000
    0111  7
    01    1 (hundreds)
    10    FM

  0111000100000100 (1710am, no preset)
    0111  7
    0001  1
    0000  0
    01    1
    00    AM



(It looks like the data is being sent as MSB first, instead of LSB first as with others?)

Prev preset 24-bit message: 11100011 1001000010010010
Play button 8-bit message: 00000100 ("FWD)
                           00001100 ("REV") (press play second time)
                           00010100

FFwd button 8-bit message: 00010000
                           01110000

Rwnd button 8-bit message: 00100000
                           00110000
                           00111000

Stop button 8-bit message: 00000010
                           00001100

Radio button 8-bit message: 00110000

AMS Start 8-bit: 10101010
AMS Complete 24-bit partial: 11000001 1001011101010010
