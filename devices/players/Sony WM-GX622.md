Sony WM-GX622 Radio Cassette-Corder (Player/Recorder)
=====================================

## Remote Connector Differences

This player shares the same remote button wiring that I first found in the [WM-FX855](Sony%20WM-FX855.md), where button presses are triggerd by pulling the remote-key pin *down* to ground via a resistor network, instead of pulling them *up* to vcc via a resistor network as happens in Minidisc players.

1) Ground
  - Same as average connector
2) Remote In
  - this pin is *pulled-up*, and remote control buttons pull it to ground through a resistor network
3) NOT CONNECTED
4) NOT CONNECTED

Note that because pins 3 and 4 are not connected, a remote control with an LCD will not work without some internal changes.

### Restoring missing pins 3 and 4

Pins 3 and 4 (remote-data-out and +B) are not connected on this model. However, it *might* be possible to retrofit those pins.

The main board is very similar to the WM-GX612 and WM-GX614 models, except those two have pins 3 and 4 properly connected. On the WM-GX612/614, the pins are connected as:

* Pin 3, labeled `DATA`
  - Connects to IC702 (SMC62L3A LCD DRIVE)
    * Pin 25, labeled `RDATA`
    * Pin 28, labeled `INT`
  - Connects to IC701 (M5M63120B SYSTEM CONTROLLER)
    * Pin 44, labeled `RO`
    * Pin 40, labeled `DRQT`
    * Pin 38, labeled `RMUM`
* Pin 4, labeled `SCLK`
  - Connects to IC702 (SMC62L3A LCD DRIVE)
    * Pin 23, labeled `RCLK`
  - Connects to IC701 (M5M63120B SYSTEM CONTROLLER)
    * Pin 46, labeled `R_CLK`

WM-GX622 uses the same ICs and they are connected to one another in the same way, but the lines are not connected to the remote connector. In theory, wiring those to lines to the remote connector would enable the GX622 to use the same remote as the GX612/614 (and other similar models that use `DATA` and `SCLK` pins on their connectors).

Peter Vis notes on [his page for the RM-WME23 remote](https://www.petervis.com/electronics%20guides/Sony%20WM-EX610/RM-WME23.html) (empasis mine):

> The remote connection is a combination of a standard jack and Sony's own design that uses an **I2C type communication bus**. The stereo jack connects to the headphones and an additional mechanical rotary volume control, whilst the rest of the connections are for tape control.

This makes sense seeing as how those two pins are essentially a direct connection to the bus between IC701 and IC701.


