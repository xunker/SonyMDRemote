#ifndef COMMANDS_TRACK_BATTERY_H
#define COMMANDS_TRACK_BATTERY_H

/* Commands as tested between MZ-NHF800 MD Player and RM-CDF7L CD Player remote */

/* Battery level, 2/3 bars: */
// const signed int commandBuffer[] = {
// -1068, +32794,
// -1090, +1018, -98, +22, -189, +107, -105, +22, -191, +21, -191, +21, -191, +21, -191, +20, -193, +104, -109, +106, -206, +5, -208, +5, -208, +5, -208, +5, -208, +5, -209, +5, -209, +209, -3, +277, -3, +210, -3, +6, -209, +4, -209, +7, -207, +5, -208, +209, -4, +5, -208, +277, -4, +209, -4, +209, -4, +211, -3, +209, -4, +211, -2, +7, -207, +209, -4, +618, -207, +5, -208, +5, -208, +5, -208, +5, -208, +5, -209, +4, -208, +5, -208, +107, -207, +5, -208, +5, -208, +5, -208, +5, -208, +5, -209, +4, -208, +5, -208, +141, -207, +5, -208, +5, -208, +5, -208, +5, -208, +5, -208, +5, -207, +7, -206, +90, -207, +5, -208, +5, -208, +5, -208, +5, -208, +5, -207, +5, -208, +5, -208, +90, -207, +5, -208, +5, -208, +5, -208, +5, -208, +6, -209, +6, -208, +5, -208, +89, -208, +5, -208, +4, -209, +5, -209, +5, -208, +5, -208, +5, -208, +5, -208, +89, -208, +5, -208, +5, -208, +5, -208, +5, -208, +5, -210, +4, -207, +7, -207, +345, -207, +5, -209, +5, -209, +5, -208, +5, -208, +5, -208, +5, -208, +5, -208, +73, -207, +5, -209, +208, -3, +211, -3, +209, -4, +210, -3, +210, -4, +209, -3, +11019,
// };


/* Track number (02): */
// const signed int commandBuffer[] = {
// -1067, +32877,
// -1108, +1015, -101, +24, -188, +102, -110, +24, -189, +22, -190, +23, -190, +22, -190, +23, -190, +103, -109, +107, -207, +5, -208, +5, -208, +4, -210, +4, -208, +5, -208, +5, -209, +209, -3, +73, -208, +5, -208, +5, -208, +5, -208, +5, -209, +210, -2, +6, -209, +210, -2, +92, -207, +5, -208, +5, -208, +5, -209, +3, -210, +4, -208, +5, -208, +5, -208, +73, -207, +5, -208, +5, -208, +5, -209, +5, -208, +5, -208, +5, -208, +5, -208, +73, -207, +5, -208, +5, -208, +5, -208, +4, -208, +5, -208, +5, -208, +5, -208, +89, -208, +209, -3, +6, -208, +5, -208, +5, -208, +5, -208, +5, -208, +5, -208, +225, -208, +3, -209, +7, -207, +5, -208, +5, -208, +5, -208, +5, -208, +5, -208, +73, -207, +5, -209, +5, -208, +5, -208, +6, -207, +5, -209, +5, -208, +5, -208, +73, -208, +5, -208, +4, -208, +5, -208, +5, -208, +5, -208, +5, -208, +5, -208, +91, -207, +5, -208, +5, -208, +5, -207, +5, -208, +5, -208, +5, -208, +5, -208, +90, -208, +5, -208, +5, -209, +5, -208, +5, -208, +5, -208, +5, -208, +4, -208, +261, -207, +210, -2, +6, -208, +5, -209, +5, -208, +209, -3, +7, -208, +209, -3, +11582,
// };

/* Track number (04): */
// const signed int commandBuffer[] = {
// -1067, +32537,
// -1105, +1018, -98, +25, -188, +103, -108, +23, -190, +23, -190, +22, -191, +21, -191, +19, -192, +107, -106, +141, -207, +5, -208, +5, -209, +5, -208, +5, -208, +5, -207, +6, -207, +210, -3, +414, -207, +5, -208, +5, -208, +5, -208, +5, -208, +209, -2, +7, -208, +209, -2, +92, -207, +5, -208, +5, -208, +5, -209, +4, -210, +4, -208, +5, -208, +5, -209, +90, -207, +5, -208, +5, -208, +5, -208, +5, -208, +5, -208, +5, -209, +5, -208, +73, -207, +5, -208, +5, -207, +5, -208, +5, -208, +5, -208, +5, -208, +5, -208, +208, -209, +4, -208, +209, -2, +6, -208, +4, -209, +4, -207, +5, -208, +5, -208, +89, -208, +5, -208, +5, -208, +5, -209, +5, -209, +5, -208, +5, -208, +5, -208, +89, -207, +6, -208, +5, -208, +5, -208, +5, -209, +5, -209, +5, -207, +5, -208, +210, -207, +5, -208, +5, -208, +5, -209, +3, -210, +4, -208, +5, -208, +5, -208, +90, -207, +6, -209, +5, -208, +5, -208, +5, -208, +5, -208, +5, -208, +5, -208, +242, -207, +5, -208, +5, -209, +5, -208, +5, -208, +5, -208, +5, -208, +5, -209, +123, -207, +5, -209, +209, -2, +7, -208, +5, -208, +209, -3, +6, -208, +208, -3, +11148,

// };

/* Above Track number 04 message changes to show track number 02: */
// const signed int commandBuffer[] = {
// -1067, +32537,
// -1105, +1018, -98, +25, -188, +103, -108, +23, -190, +23, -190, +22, -191, +21, -191, +19, -192, +107, -106, +141, -207, +5, -208, +5, -209, +5, -208, +5, -208, +5, -207, +6, -207, +210, -3, +414, -207, +5, -208, +5, -208, +5, -208, +5, -208, +209, -2, +7, -208, +209, -2, +92, -207, +5, -208, +5, -208, +5, -209, +4, -210, +4, -208, +5, -208, +5, -209, +90, -207, +5, -208, +5, -208, +5, -208, +5, -208, +5, -208, +5, -209, +5, -208, +73, -207, +5, -208, +5, -207, +5, -208, +5, -208, +5, -208, +5, -208, +5, -208, +208, -209, +209, -3, +6, -208, +6, -208, +4, -209, +4, -207, +5, -208, +5, -208, +89, -208, +5, -208, +5, -208, +5, -209, +5, -209, +5, -208, +5, -208, +5, -208, +89, -207, +6, -208, +5, -208, +5, -208, +5, -209, +5, -209, +5, -207, +5, -208, +210, -207, +5, -208, +5, -208, +5, -209, +3, -210, +4, -208, +5, -208, +5, -208, +90, -207, +6, -209, +5, -208, +5, -208, +5, -208, +5, -208, +5, -208, +5, -208, +242, -207, +5, -208, +5, -209, +5, -208, +5, -208, +5, -208, +5, -208, +5, -209, +123, -207, +210, -2, +6, -208, +7, -208, +5, -208, +209, -3, +6, -208, +208, -3, +11148,

// };

#define IDLE_MESSAGE -1067, +32537
#define SYNC_RESET -1105, +1018
#define ZERO_BIT -220, +18
#define ONE_BIT +220, -18, +18
#define BYTE_PAUSE +100
#define ALL_ZERO_BYTE ZERO_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT
#define MESSAGE_HEADER_105 SYNC_RESET, ZERO_BIT, ALL_ZERO_BYTE, ZERO_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT, ONE_BIT
#define TRACK_NUMBER_MESSAGE_0XA0 ZERO_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT, ONE_BIT, ZERO_BIT, ONE_BIT
#define VOLUME_MESSAGE_0X40 ZERO_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT, ONE_BIT, ZERO_BIT
#define BATTERY_MESSAGE_0X43 ONE_BIT, ONE_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT, ONE_BIT, ZERO_BIT

// hundreds/??
// ones/tens
#define TRACK_NUMBER_MESSAGE MESSAGE_HEADER_105, \
  TRACK_NUMBER_MESSAGE_0XA0, \
  ALL_ZERO_BYTE, \
  ALL_ZERO_BYTE, \
  ALL_ZERO_BYTE, \
  ONE_BIT, ONE_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT, ONE_BIT, \
  ALL_ZERO_BYTE, \
  ALL_ZERO_BYTE, \
  ALL_ZERO_BYTE, \
  ALL_ZERO_BYTE, \
  ALL_ZERO_BYTE, \
  ONE_BIT, ONE_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT, ONE_BIT, ZERO_BIT, ZERO_BIT // checksum

// first 4 bits of first byte after VOLUME_MESSAGE_0X40 is volume number 0-30 BCD
#define VOLUME_MESSAGE MESSAGE_HEADER_105,  \
    VOLUME_MESSAGE_0X40,  \
    ZERO_BIT, ZERO_BIT, ZERO_BIT, ONE_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT,  \
    ZERO_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT, ZERO_BIT, ONE_BIT, ZERO_BIT,  \
    ZERO_BIT, ONE_BIT, ONE_BIT, ONE_BIT, ONE_BIT, ZERO_BIT, ZERO_BIT, ONE_BIT,  \
    ALL_ZERO_BYTE,  \
    ALL_ZERO_BYTE,  \
    ALL_ZERO_BYTE,  \
    ALL_ZERO_BYTE,  \
    ALL_ZERO_BYTE,  \
    ALL_ZERO_BYTE,  \
    ZERO_BIT, ONE_BIT, ONE_BIT, ZERO_BIT, ONE_BIT, ZERO_BIT, ZERO_BIT, ONE_BIT

// first byte after BATTERY_MESSAGE_0X43 is battery level
#define BATTERY_LEVEL_MESSAGE MESSAGE_HEADER_105,  \
    BATTERY_MESSAGE_0X43,  \
    ONE_BIT, ONE_BIT, ONE_BIT, ONE_BIT, ONE_BIT, ONE_BIT, ZERO_BIT, ONE_BIT,  \
    ALL_ZERO_BYTE,  \
    ALL_ZERO_BYTE,  \
    ALL_ZERO_BYTE,  \
    ALL_ZERO_BYTE,  \
    ALL_ZERO_BYTE,  \
    ALL_ZERO_BYTE,  \
    ALL_ZERO_BYTE,  \
    ALL_ZERO_BYTE,  \
    ZERO_BIT, ZERO_BIT, ONE_BIT, ONE_BIT, ONE_BIT, ONE_BIT, ONE_BIT, ONE_BIT

const signed int commandBuffer[] = {
    IDLE_MESSAGE,
    IDLE_MESSAGE,
    IDLE_MESSAGE,
    BATTERY_LEVEL_MESSAGE,
    IDLE_MESSAGE,
    IDLE_MESSAGE,
    IDLE_MESSAGE,
    TRACK_NUMBER_MESSAGE,
    IDLE_MESSAGE,
    IDLE_MESSAGE,
    IDLE_MESSAGE,
    VOLUME_MESSAGE};

#endif
