# Data Formats on Different Devices

# MZ-NHF800 MiniDisc Player

Depending on the DATA-to-GROUND pulldown used, the data format is slightly
different. See the dumps [mz-nhf800_pulldown_100k.txt](dumps/mz-nhf800_pulldown_100k.txt) and
[mz-nhf800_pulldown_470k.txt](dumps/mz-nhf800_pulldown_470k.txt) for all data.

Obvious differences are:
* Length of the low-part of the reset/sync pulse is different slightly
  - Length with 100k pulldown is around 100us shorter than with 470k
* Length of the high-part of the reset/sync pulse is different
  - With 100k pulldown is averages to 940us
  - With 470k pulldown is averages to just under 920us (916us-919us)
* The timing of the first 9 bit transitions is different
  - With 100k pulldown a "zero" is ~190us-210us low and ~9us-12us high
  - With 470k pulldown a zero is ~178us-181us low and ~30us-35us high

Things that are the same with either pulldown:
* The timing of bits after the first 9 bits
* The number of bits per message
* The number/order of messages send to the remote

Could the length of the high-part of the sync/reset pulse denote the kind of
message being sent?

# D-EJ815 CD Player

See [d-ej815_pulldown_100k.txt](dumps/d-ej815_pulldown_100k.txt) for full dump.

Differences from MZ-NHF800:
* Length of the low-part of the reset/sync pulse is usually much shorter
  - ~800us, though there are some scattered messages that are ~1035us to 1060us
  - Length with 100k pulldown is around 100us shorter than with 470k
* Length of the high-part of the reset/sync pulse varies wildly
  - swings from 2us to ~1200us+
* Bit transition timing is different (inconsistent?)
  - "Zero" looks like low for around 175us and high for around 55us
  - Guessing that a "one" looks like low for around 175us as well, but high under 5us
* Regular one-bit transitions after the end of a message or just before the sync/reset of a new message

D-EJ815 may also use a ~1000us as a byte delimiter, as it shows up after the
first 9 bits on a message.
