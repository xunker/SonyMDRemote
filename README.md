# Sony MD Remote - Pins, connections, protocol

The Sony MD (MiniDisc) Remote is a TRS + 4-pin connector found on many Sony
products made from the 1990's to the 2000's.

The goal of this repository is to document physical and electrical
parameters of the connector and the and protocols used with it.

(TODO: image of connector and remote)

**IMPORTANT**: This is for the TRS + 4-pin connector *only*. It is **not** for:
* Green 9-pin connecter used on some cassette Walkman players and DAT recorders
* 7-pin "Digital I/O REMOTE" jack on some DAT recorders
* EXT connector on PlayStation DualShock4 controller
* Headset connector on PS Vita

## What devices have a connector for this remote control?

The port is ubiquitous on Sony's MiniDisc players and recorders, hence the name,
but it also appears on many other Sony devices:

* most Minidisc (MD) players/recorders
* many Discman/CD-Walkman players
* some Walkman cassette players/recorders
* few Network Walkman digital media players
* a couple of portable Dat recorders
* part of the Sony Vaio U-Series notebook computer range

For a more detailed list, please see [devices/players/README.md](devices/players/README.md).

### Note on compatibility

Not all devices will have the same capabilities when connected to a
remote control. Just because the remote control will plug is does not mean it
will work.

Often, basic transport (play, pause, stop, ffwd, rwd) and volume controls will work on any device that support those functions within the same device family. Beyond those basic functions, button behavior will depend on the remote control and the device being controlled. Even basic button compatibility cannot be guaranteed however; for example, the cassette Walkman does not respond to the remote control for a CD or MD player or vice-versa.

While almost all devices will support being controlled by buttons on the appropriate remote control, not all devices will send data to the remote information display (if so equipped). This is especially common on Discman/CD-Walkman and Cassette Walkman players. If the device in question was not originally available with an LCD remote control, it is likely the device is not designed to send data to one.

There are also *at least* four different versions of the LCD / Information Display protocol, and remote controls designed for one will generally not work for the other.

## File and Directories in this repository

* [connector](connector/)
  - Physical and electrical description of the connector
* [protocol](protocol/)
  - Details on the electrical and logical signaling for sending data to the LCD
* [devices](devices/)
  - Information on individual devices, such as players or remotes

* [remote_dumper](remote_dumper/)
  - Arduino sketch for dumping LCD display data from a player device
* [remote_sender](remote_sender/)
  - Arduino sketch for sending data to the LCD of a connected remote control device
* [remote_composer](remote_composer/)
  - Arduino sketch for building remote control LCD messages by hand to sent to the remote control device

## What is the name of the connector?

I don't know the technical name for the connector, which is why I've decided
to call this repo "Sony MD Remote", even though it is used on other devices.

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
connected to. For example, the [Sony RM-MC25C](devices/remotes/Sony+RM-MC25C.md) remote
has a push+turn action that does different things depending on if connected to
a MiniDisc player or a CD player.

### LCD / Information Display

For remote controls that have a display when connected to a compatible player, the player can use it to show things like track titles, times, volume, etc.

## Connector pinout and dimensions

Information on the physical connector and the pinout can be found in the [connector directory](connector/).

### What is the voltage of the VCC pin?

This is probably dependent on the device. For the [MZ-R3 it appears to be 2.9v](https://web.archive.org/web/20090217092709/http://home.no.net/~htoerrin/md_if/md_spek.htm),
while I have tested my MZ-NHF800 and D-EJ815 to be sending 2.1v, and my WM-FX855
to be sending 1.44v to 2.6v depending on the pins probed.

### Are all remotes tolerant of all possible voltages?

It's likely any remote can handle the voltage from any device, but I haven't
tested it yet.

Since standard IC voltage ratings generally step from 1.8v to 3.3v, I believe
it's safe to assume they are designed for 3.3v nominal VCC with the maximum
logic level being something common like Vcc + 0.2-to-0.5v.

Generally speaking, TTL high is anything greater than Vcc * 0.6 and TTL low is
anything less than Vcc * 0.3. This means that a 2.1v "high" signal would be
correctly understood by a 3.0v device.

It's possible that older LCD remotes need ~3v Vcc and won't operate on the
lower 2.1v of newer MD players.

## Playback controls

Logically, the buttons work by connecting pin 2 to either pin 4 or pin 1 (depending on model) through a [resistor ladder](https://en.wikipedia.org/wiki/Resistor_ladder). Different resistances cause different actions on the device.

On some controls the button resistance can be read without VCC or ground being
connected or a power source (ex: [RM-MC25C](devices/remotes/Sony+RM-MC25C.md)).
Others, usually those with an LCD display, require VCC to be connected to power
and ground to be connected before the buttons will work (ex:
[RM-CDF7L](devices/remotes/Sony+RM-CDF7L.md)).

###  Action resistances

Look in the [remotes](devices/remotes/) directory for a list of the currently tested
device remote control devices and the resistances of their buttons.

## LCD / Information Display Protocol

Please see the [protocol description main page](protocol/README.md).

## Author and Location

Matthew Nielsen, May 2020

https://github.com/xunker/SonyMDRemote

## Attributions

[description of the earlier version LCD signaling](https://web.archive.org/web/20090217092709/http://home.no.net/~htoerrin/md_if/md_spek.htm) by Havard Torring.

[MZ-R2 remote control resistances](http://www.minidisc.org/r2_remote.html) by Chris Iffland.

[MZ-R30 remote control resistances](http://www.minidisc.org/r3_remote.html) by Andy James.

## License

Unless otherwise specified, content of this repository is licensed under the
[Creative Commons Attribution 4.0 International License](https://creativecommons.org/licenses/by/4.0/).
