# Sony MD Remote - Pins, connections, protocol

The Sony MD (MiniDisc) Remote is a TRS + 4-pin connector found on many Sony
products made from the 1990's to the 2000's.

The goal of this repository is to document physical and electrical
parameters of the connector and the and protocols used with it.

(TODO: image of connector and remote)

**IMPORTANT**: This connector is *NOT* the 9-pin connector used on some older
Walkman WM & EX models, the EXT connector on the DualShock4 controller or
the headset connector on the PS Vita.

## What devices have a connector for this remote control?

The port is ubiquitous on Sony's MiniDisc players and recorders, hence the name,
but it also appears on many other Sony devices.

This is NOT an exhaustive list:

* most Minidisc (MD) players/recorders
* some Discman and CD-Walkman players
  - includes the CRX10U portable USB CD-R/RW drive
* some Walkman cassette players
* some Network Walkman digital media players
  - NW-A1000, NW-A3000
  - NW-HD1, NW-HD2, NW-HD3, NW-HD5
* some hard-drive-based media players
  - HMP-A1
  - Vaio Pocket VGF-AP1L
* some Vaio computers
  - VGN-U50
  - VGN-U70P
  - VGN-U8G
  - VGN-U8C
  - VGN-U750P
  - VGN-U71P

### Note on compatibility

Not all devices will have the same capabilities when connected to a
remote control. Just because the remote control will plug is does not mean it
will work.

Generally, basic transport (play, pause, stop, ffwd, rwd) and volume controls
will work on any device that support those functions. Beyond those, button
behaviour will depend on the remote control and the device being controlled.

While almost all devices will support being controlled by buttons on the remote
control, not all devices will send data to the remote information display (if so
equipped). This is especially common on Discman/CD-Walkman and Cassette Walkman
players. If the device in question was not originally available with an LCD
remote control, it is likely the device is not designed to send data to one.

There are also *at least* two different versions of the LCD / Information
Display protocol, and remote controls designed for one will generally not work
for the other.

## Whis is the name of the connector?

I don't know the technical name for the connector, which is why I've decided
to call it the "Sony MD Remote" connector for now, even though it is used on
other devices.

The technical description would be something like "TRS + 4-pin edge". There is
undoubtably a Sony-internal part name and manufacturer part number, but I don't
know what they are. If you you do, please let me know.

## What are the functions of the remote control?

### Audio pass-through

Passes the audio from the 3.5mm TRS plug to the jack on the remote. This should
work for any device since it is just a straight electrical connection. Note that
this will only work for TRS (Tip-right-sleeve, 2-channel stereo), and not for
TRRS (stereo + microphone).

### Playback controls

The most basic remote controls will generally have buttons for play, pause,
fast-forward, rewind and volume.

Some have additional actions that do different things depending on what they are
connected to. For example, the [Sony Sony RM-MC25C](./Sony Sony RM-MC25C remote.md) remote
has a push+turn action that does different things depending on if connected to
a MiniDisc player or a CD player.

### LCD / Information Display

#### Compatibility

## Connector

### What is the pinout of the connector?

### What are the dimensions of the connector?

### How to make a DIY connector

### What is the voltage of the VCC pin?

This is probably dependent on the device. For the [MZ-R3 it appears to be 2.9v](https://web.archive.org/web/20090217092709/http://home.no.net/~htoerrin/md_if/md_spek.htm),
while I have tested my MZ-NHF800 to be sending 2.1v.

### Are all remotes tolerant of all possible voltages?

It's likely any remote can handle the volated from any device, but I haven't
tested it yet.

It's doubtful that Sony used custom ICs in their remote controls, and if they
are off-the-shelf I believe it's safe to assume they are designed for 3.3v
nominal VCC with the maximum logic level being something common like
Vcc + 0.2-to-0.5v.

Generally speaking, TTL high is anything greater than Vcc*0.6 and TTL low is
anything less than Vcc*0.3. This means that a 2.1v "high" signal would be
correctly understood by a 3.0v device.

It's possible that older LCD remotes need ~3v Vcc and won't operate on the
lower 2.1v of newer MD players.

## Playback controls

Logically, the buttons work by connecting pin 4 to in 2 through a [resistor ladder](https://en.wikipedia.org/wiki/Resistor_ladder).
Different resistances cause different actions on the device.

On some controls the button resistance can be read without VCC or ground being
connected or a power source (ex: [RM-MC25C](./Sony Sony RM-MC25C remote.md)).
Others, usually those with an LCD display, require VCC to be connected to power
and ground to be connected before the buttons will work (ex: [RM-CDF7L](./Sony RM-CDF7L remote.md)).

###  Action resistances

## LCD / Information Display Protocol

## Author and Location

Matthew Nielsen, May 2020

https://github.com/xunker/SonyMDRemote

## Attributions

[description of the earlier version LCD signaling](https://web.archive.org/web/20090217092709/http://home.no.net/~htoerrin/md_if/md_spek.htm) by Havard Torring.

[MZ-R2 remote control resistances](http://www.minidisc.org/r2_remote.html) by Chris Iffland.

[MZ-R30 remote control resistances](http://www.minidisc.org/r3_remote.html) by Andy James.

## License

Unless otherwise specified, content of this repository is licensed under the
[Creative Commons Attribution 4.0 International Licesnse](https://creativecommons.org/licenses/by/4.0/).
