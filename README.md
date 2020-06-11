 picoVM - Virtual Machine for embedded devices
==========================================================

picoVM is a fully featured, minimal footprint, VM for embedded devices.

Written in pure C for maximum efficiency and portability. Essentially just a header file and one source file that you can include in your project. 

Features
--------

* Supports 8bits, 16bits and 32bits integer arithmetic and logic operations
* Supports floating point arithemtic
* Stack based

Footprint
---------
picoVM is mainly targeted for embedded devices that usually have restricted memory. 
On ARM with all the features compiled supporing 8,16 and 32 bit integers, 32 bit 
floats and the full instruction set the code sums up to less than 1 KB. Depending on 
the application's needs this can go down to 632 bytes without floating point arithmetic etc.

Typical number with the current version compiled for the ARMv7-M architecture:

|                       | Code size |
|-----------------------|:---------:|
| Full features         | 994 bytes |
| - floating point      | 790 bytes |
| - Single integer type | 770 bytes |
| - Bitwise operations  | 728 bytes |
| - Advanced operations | 632 bytes |

Dependencies
------------

None.

Building
--------

Type:

```make```

Licensing
---------

picoVM is published under the MIT License, see LICENSE for details.
