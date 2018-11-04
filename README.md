# NVENC-EMU

**This is a code dump. It currently doesn't do anything useful, and I won't be
finishing it. There are several dead-ends in the code marked with `FIXME`.**

This project attempts to partially implement NVENC emulation on top of VA-API.
Partial in the sense that it specifically targets the API subset used by Steam
streaming on Linux.

Unfortunately, I no longer have the motivation to finish it. As it turns out,
the hardware I was targeting has no video encoding capability at all. One might
say I was a bit late to find out, having written all this already. 🙃
