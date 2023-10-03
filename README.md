# Firmware Update Over CAN Using Custom Bootloader

## Table of Contents

- [Introduction](#introduction)
- [Memory Layout](#memory-layout)
- [Usage](#usage)



## Introduction

- The bootloader is designed for updating and launching firmware on a microcontroller, It manages firmware updates and switch between different applications.

## Memory Layout

| **Memory Range**      | **Section**          | **Size** |
|-----------------------|----------------------|----------|
| 0x08000000 - 0x080063FF | Bootloader         | 25KB     |
| 0x08006400 - 0x0800BBFF | Firmware Receiver  | 30KB     |
| 0x0800DC00 - 0x080133FF | New Firmware       | 30KB     |


## Usage

**Custom Bootloader**
1. **Update:** Use button 1 to trigger a firmware update. The bootloader will jump to the Firmware Receiver to receive and install new firmware.
2. **Start:** Use button 2 to start the firmware. The bootloader checks for valid firmware at the specified address `NEW_FIRMWARE_START_ADDRESS` and jumps to it if found. If no valid firmware is found, it displays a "No Updates" message.


