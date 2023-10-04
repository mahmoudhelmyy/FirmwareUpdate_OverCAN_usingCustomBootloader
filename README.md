# Firmware Update Over CAN Using Custom Bootloader

## Table of Contents
- [Team Members](#team-members)
- [Introduction](#introduction)
- [Memory Layout](#memory-layout)
- [Usage](#usage)


# Team Members


| Name                         | ID    | GitHub                                                                                     | LinkedIn                                                                                      |
|------------------------------|-------|--------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------|
| Mahmoud Sayed Mahmoud Helmy  | 1285  | [![GitHub](https://img.shields.io/badge/github-%23121011.svg?style=flat&logo=github&logoColor=white)](https://github.com/mahmoudhelmyy)  | [![LinkedIn](https://img.shields.io/badge/linkedin-%230077B5.svg?style=flat&logo=linkedin&logoColor=white)](linkedin.com/in/mahmoudhelmyy)  |
| Mohamed Mahmoud Masoud       | 200   |                                                                                            |                                                                                               |
| Mohamed Abdel Samie          | 482   |                                                                                            |                                                                                               |
| Ahmed Essam                  | 765   |                                                                                            |                                                                                               |
| Mahmoud Abdrabo              |       |                                                                                            |                                                                                               |

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
**Firmware Receiver**
In ECU1, and handles CAN communication and receive the new firmware from ECU2 over CAN and flash the new version to address `0x0800dc00`.
**Firmware Sender**
In ECU2, Communicates over CAN, serves new updates to ECU1.
**New Firmware**
This the New firmware received by ECU1 from ECU2.
