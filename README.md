This repository holds all the lab deliverables. This README covers each task in detail.

**Lab 2 Task 10**

I chose the noisy threshold by the seeing the volume of my voice talking and comparing it to the ambient noise of my room and choosing a value in between. I decided to go to the higher end of the values though so that typing or noise from another room didn’t set it off. I chose Dark/bright by covering the sensor and turning off my light and setting it halfway to my the value when my room light is on. For moving I shook it and chose a value just slightly larger than when it is stationary. For Near vs Far I decided to choose a value where my hand was only a few inches away from the sensor.

**Lab 2 Task 11**

I selected my threshold by comparing baseline measurements to when I breathed on the board, moved my phone close to it, and turned on and off the light. I tried to keep it so that minor changes in the environment did not cause an event trigger but larger changes did. I decided to simply implement a 2 second cooldown instead of debounce delay, simply disabling the event triggers if the cooldown was on.
