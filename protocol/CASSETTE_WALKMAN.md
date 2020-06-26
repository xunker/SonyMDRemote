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

Examples:
* 00000010 (Stop button pressed in radio mode, radio turning off)
* 00110000 (Band / Radio on button pressed from off state)
* 00000100 (Play button pressed from off state, forward direction)
* 00111000 (Play button pressed from off state, reverse direction)
* 00000010 (Stop button pressed in tape mode, tape stopping)
* 00001100 (Play button pressed in playing state, changing plan direction from forward to reverse)
* 00000100 (Play button pressed in playing state, changing plan direction from reverse to forward)
* 00100000 (Rewind button pressed from playing state, forward direction)
* 00010000 (Fast-forward button pressed from playing state, forward direction)
* 00101000 (Rewind button pressed from playing state, reverse direction)
* 00011000 (Fast-forward button pressed from playing state, reverse direction)
* 01010101 (ASP button long-pressed from radio mode)

Radio Mode:

Position | Meaning
---------|---------------------------------
0        | 0 indicates radio mode
1        |
2        | Tape direction, 0 = rev, 1 = fwd
3        |
4..7     | Current preset number as BCD

Tape Mode (normal):

Position | Meaning
---------|----------------------------------
0        | 1 indicates tape mode
1        | Rewind
2        | Fast-forward
3        | Tape direction, 0 = rev, 1 = fwd
4        | Play
5        | Stop
6        | ?
7        | ?

Tape Mode (FFwd or Rwd):

Position | Meaning
---------|----------------------------------
0        | 1 indicates tape mode
1        | Rewind
2        | Fast-forward
3        | Tape direction, 0 = rev, 1 = fwd
4..7     | BCD of current "AMS" selection

#### First Nibble

It appears that the upper nibble is further divided in to two pairs.

(Unsure) The first pair of the upper nibble may be the current "mode":
* `00`: Tape mode
* `11`: Radio mode

(Unsure) The second pair of the upper nibble may be the play direction:
* `00`: Forward
* `01`: Reverse

The second pair is always `00` when in radio mode.

#### Second Nibble

(Unsure) In tape mode, this could indicate the current action (playing, ffwd, rwd, etc). Examples:
* `00001000` (tape mode, playing forward)
* `00011000` (tape mode, playing reverse)

00010100


00100000 # from playing fwd state, press ffwd button
01000000 # from playing fwd state, press rwd button
00110000 # from playing reverse state, press ffwd button
01010000 # from playing reverse state, press rwd button

11100010 1001011110010010 # radio in reverse state
11000010 ................ # radio in fwd state



In radio mode, this is the current preset number in BCD form.

Examples (additional 16-bit frequency information omitted):
* `11000001` (radio mode, preset 1)
* `11000010` (radio mode, preset 2)
* `11000011` (radio mode, preset 3)
* `11000100` (radio mode, preset 4)
* `11000101` (radio mode, preset 5)
* `11000000` (radio mode, no preset)

### Second and Third Bytes

In tape mode, only the first byte is sent and these additional bytes are not sent. In radio mode, these bytes indicate the current frequency and frequency band.

The two bytes are divided in to a series of nibbles. The first 3 nibbles are parts of the frequency in BCD. The high pair of the last nibble is the "hundreds" (for FM) or "thousands" (for AM), and the low pair is the band indicator (`00` for AM, `10` for FM).
Position | FM Mode Meaning  | AM Mode Meaning
---------|------------------|-----------------
0..3     | Tens             | Hundreds
4..7     | Ones             | Tens
8..11    | Decimal          | Ones
12..13   | Hundreds         | Thousands
14..15   | Band (10 for FM) | Band (00 for AM)

Examples:
* `11000001 1001011101010010` (97.5mHz, preset 1)
* `11000010 0000000001110110` (100.7mHz FM, preset 2)
* `11000011 0000000100010110` (101.1mHz FM, preset 3)
* `11000100 0000000101010110` (101.5mHz FM, preset 4)
* `11000101 0100001100000100` (1430kHz AM, preset 5)
* `11000000 1000000000000000` (800kHz AM, no preset)
* `11000000 0100010000000100` (1440kHz AM, no preset)
