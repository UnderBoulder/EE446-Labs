This repository holds all the lab deliverables. This README covers each task in detail.

**Lab 2 Task 10**

This project implements a rule-based workspace awareness system using:
- Microphone (PDM)
- IMU (BMI270)
- Light + proximity (APDS9960)

It classifies the desk into one of four states:
1. QUIET_BRIGHT_STEADY_FAR
2. NOISY_BRIGHT_STEADY_FAR
3. QUIET_DARK_STEADY_NEAR
4. NOISY_BRIGHT_MOVING_NEAR

I chose the noisy threshold by the seeing the volume of my voice talking and comparing it to the ambient noise of my room and choosing a value in between. I decided to go to the higher end of the values though so that typing or noise from another room didn’t set it off. I chose Dark/bright by covering the sensor and turning off my light and setting it halfway to my the value when my room light is on. For moving I shook it and chose a value just slightly larger than when it is stationary. For Near vs Far I decided to choose a value where my hand was only a few inches away from the sensor.

**Lab 2 Task 11**
