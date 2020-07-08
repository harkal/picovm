 picoVM - Virtual Machine for embedded devices
==========================================================

picoVM is a fully featured, minimal footprint, VM for embedded devices.

Written in pure C for maximum efficiency and portability. Essentially just a header file and one source file that you can include in your project. 

Features
--------

* Supports 8bits, 16bits and 32bits integer arithmetic and logic operations
* Supports floating point arithemtic
* Stack based
* picolang C like compiler to generate vm specific code (https://github.com/harkal/picolang)

Footprint
---------
picoVM is mainly targeted for embedded devices that usually have restricted memory. 
On ARM with all the features compiled supporing 8,16 and 32 bit integers, 32 bit 
floats and the full instruction set the code sums up to about 1KB. Depending on 
the application's needs this can go down to 700 bytes without floating point arithmetic etc.

Typical number with the current version compiled for the ARMv7-M architecture:

|                       | Code size     |
|-----------------------|:-------------:|
| Full features         |    1118 bytes |
| - floating point      |     926 bytes |
| - Single integer type |     872 bytes |
| - Bitwise operations  |     822 bytes |
| - Advanced operations |     754 bytes |

Dependencies
------------

None.

Building
--------

Type:

```make```

Documentation
-------------

Please refer to the files in the ``docs`` directory.

Licensing
---------

picoVM is published under the MIT License, see LICENSE for details.
