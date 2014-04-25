glitchworks_serial
==================

Multi-platform Serial Port Library in C

Purpose
-------

This library exists to provide a simple serial communications interface for C programs targeted at multiple platforms. It is being written to support several personal projects that require serial communications. Current planned supported targets include:

* POSIX-compliant systems (Linux, BSD, et c.)
* MS-DOS systems

Limitations
-----------

Since serial communication is radically different between the supported platforms, a subset of available serial functionality will be supported by this library. Specifically, this library will support:

* Named serial ports (UNIX filenames, MS-DOS port specs)
* Variable serial speeds
* Variable word length

At this time, only bytewise serial communications are being implemented. Supported speed/baudrates are limited to those supported by all target platforms. A single serial port is supported per instance.

Examples
--------

See `driver.c` for example usage of the libaray. Run `make posix_test` to build the driver program with the POSIX libraries.
