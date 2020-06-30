Display Protocol for Cassette Walkman 4-pin Remote
==================================================

**Note**: this is based only on research with one player, WM-FX855. The protocol will likely vary on some other cassette Walkman models (since their remote controls are not universally compatible).

Please see https://github.com/xunker/SonyMDRemote for more information.

## Introduction

The display protocol used for the cassette Walkman (hereafter "Walkman", even though that branding was also used on some CD and MD players) is significantly different from the protocol used in MD and CD players.

## Electrical

Data is sent down remote connector pin 3, and pin 1 is used as the ground. The signal is active-high and the high voltage is slightly below pin 4 (battery). This means that the signal high level will drop as the battery voltage drops, from 1.5v (full) to whatever level the Walkman stops working at.

Signals of this level may be problematic to detect on a 3.3v microcontroller. For example, the AT91SAM3X8E (Ardino Due) requires >= 2.3v for high (0.7 * VDDIO) and 0.99v (0.3 × VDDIO) for low. This means, by itself, the Due will have problems "seeing" the signals with `digitalRead`.

## Logical

Start of message is signaled by high for 5ms, followed by low for 2ms.

`Zero` is indicated by 550-600us high followed by 2ms of low.

`One` is indicated by 2ms high followed by 550-600us of low.

Messages are only sent when a button is pushed, or if the player does an automatic action (like triggering of auto-reverse at the end of the side of a tape).

Data is send most-significant-bit-first (big-endian).


### Message structure and length

Messages are either 8- or 24-bits long. There are no start or stop bits sent.

If the message is 24-bits long, there is a 2ms low between the first 8 bits and and remaining 16 bits. There is no pause between the second two bytes.

8-bit message:

![Capture of 8-bit message from Walkman MN-FX855](images/walkman_8_bit_message.jpg)

24-bit message:

![Capture of 24-bit message from Walkman MN-FX855](images/walkman_24_bit_message.jpg)

### First Byte

Radio Mode:

Position (MSB First) | Meaning
---------------------|---------------------------------
0                    | 1 indicates radio mode if extra 2 bytes present
1                    | 1 indicates radio mode if extra 2 bytes present
2                    | Tape direction, 1 = rev, 0 = fwd
3                    | ?
4..7                 | Current preset number as BCD

Tape Mode (Playing Normally):

Position (MSB First) | Meaning
---------------------|----------------------------------
0                    | 0 indicates tape mode
1                    | 1 indicates currently Rewinding
2                    | 1 indicates currently Fast-forwarding
3                    | Tape direction, 1 = rev, 0 = fwd
4                    | Play
5                    | Stop
6                    | ?
7                    | ?

Tape Mode (while Fast-Forwarding or Rewinding):

Position (MSB First) | Meaning
---------------------|--------------------------------------
0                    | 0 indicates tape mode
1                    | 1 indicates currently Rewinding
2                    | 1 indicates currently Fast-forwarding
3                    | Tape direction, 1 = rev, 0 = fwd
4..7                 | BCD of current "AMS" selection

### Second and Third Bytes

In tape mode, only the first byte is sent and these additional bytes are not sent. In radio mode, these bytes indicate the current frequency and frequency band.

Index (MSB) | Index (LSB) | FM Mode       | AM Mode       | TV Mode             |
------------|-------------|---------------|---------------|---------------------|
0..3        | 12..15      | Tens          | Hundreds      | Ones                |
4..7        | 8..11       | Ones          | Tens          | Unused              |
8..11       | 4..7        | Tenths        | Ones          | 1100(MSB) = TV mode |
12..13      | 2..3        | Hundreds      | Thousands     | Tens                |
14..15      | 0..1        | Band          | Band          | Band                |

`Band` fields:
* `10`: FM
* `00`:
  - AM mode if "ones"/"tv mode" field is less than 12 (1100 MSB)
  - TV mode if "ones" field is equal to 12 (1100 MSB)

Examples:
* `11000001 1001011101010010` (97.5mHz, preset 1)
* `11000010 0000000001110110` (100.7mHz FM, preset 2)
* `11000011 0000000100010110` (101.1mHz FM, preset 3)
* `11000100 0000000101010110` (101.5mHz FM, preset 4)
* `11000101 0100001100000100` (1430kHz AM, preset 5)
* `11000000 0101001100010000` (531kHz AM, no preset)
* `11000000 1000000000000000` (800kHz AM, no preset)
* `11000000 0100010000000100` (1440kHz AM, no preset)
* `11000000 0111000100000100` (1710khz AM, no preset)
* `11000000 0001000011000000` (TV 1ch, no preset)
* `11000000 0010000011000000` (TV 2ch, no preset)
* `11000000 0011000011000000` (TV 3ch, no preset)
* `11000000 0100000011000000` (TV 4ch, no preset)
* `11000000 0101000011000000` (TV 5ch, no preset)
* `11000000 0110000011000000` (TV 6ch, no preset)
* `11000000 0111000011000000` (TV 7ch, no preset)
* `11000000 1000000011000000` (TV 8ch, no preset)
* `11000000 1001000011000000` (TV 9ch, no preset)
* `11000000 0000000011000100` (TV 10ch, no preset)
* `11000000 0001000011000100` (TV 11ch, no preset)
* `11000000 0010000011000100` (TV 12ch, no preset)



11000000 0111011010000010 fm 76.8
11000000 1001000000000010 fm 90.0
11000000 0111011000000010 fm 76.0
11000000 0111011010000010 fm 76.8

