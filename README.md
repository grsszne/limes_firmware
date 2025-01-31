# Lunar Rover Firmware

## Overview
This document provides an overview of the firmware for the lunar rover, detailing its core functionalities, hardware interactions, and firmware architecture.

## Features
- ESP32-based control system
- Up to 200' video streaming
- Sensor integration (IMU, LiDAR, encoders)
- 2 DoF Motor control for locomotion

## Hardware Components
- **MCU**: ESP32S3
- **Motors**: BLDC motors with DRV8837 drivers
- **Sensors**:
  - IMU: MPU6050
  - LiDAR: VL53L1X
- **Power**:
  - Solar panels
  - USB charging fallback
  - 500 mAh LiPo Battery
- **Communication**:
  - WiFi AP for control

## Software Architecture
### 1. Initialization
- Setup WiFi and AP mode
- Initialize sensors and peripherals
- Start camera and data servers

### 2. Main Loop
- Read sensor data
- Process navigation commands, serve to web interface
- Control motors and actuators based on requested URLs from web interface
- Stream video feed

## Communication Protocol
- **Control Commands**: JSON-based over HTTP/WebSockets
- **Telemetry Data**: Periodic JSON packets

## Future Improvements
- SLAM-based navigation
- ML For optimized navigation and performance
- Improved power efficiency
- Enhanced video transmission protocol
