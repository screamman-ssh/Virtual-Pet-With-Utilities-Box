# Virtual Pet With Utilities Box

[](https://github.com/screamman-ssh/Virtual-Pet-With-Utilities-Box#virtual-pet-with-utilities-box)
<p align="center">
    <img width="30%" src="https://github.com/screamman-ssh/Virtual-Pet-With-Utilities-Box/assets/77887797/720e2cb3-3069-4b31-bb9b-7d4f01ebe654">
</p>
An embedded project for our embedded course inspired by the Tamagotchi game to a 16x16 LED pixel display virtual pet (cat) with useful tools, clock, weather, calendar and game. The virtual pet can be controlled online via the LINE application using Google AppScript as a webhook. The project was implemented with esp32 and Odroid-C4 (Optional), it's just a requirement from the course but you can use only esp32 to operate all functions.

## Functions and Features

 - Tamagotchi like (virtual pet/cat) game
 - Changing the background mode
 - Changing the cat's colour and pattern mode
 - Digital clock with customizable light colour pattern mode
 - Current weather and temperature mode
 - A snake game
## Hardwares
The main component in making this project included:
 - ESP-WROOM-32 (Main Microcontroller)
 - Odroid-C4 (Optional)
 - WS2812B 16x16 (256 leds display)
 - DHT11 (Temperature and humid sensor module)
 - DS1307 (Real-Time clock module)
 - 4-Push Button (Push button module)

## Schematic Diagram
<p align="center">
    <img width="50%" src="https://github.com/screamman-ssh/Virtual-Pet-With-Utilities-Box/assets/77887797/06eb9e08-b0c9-4c77-b0b7-11c66796cf06">
</p>
In making this project we have designed and used this schematic diagram to connect all parts together.

## Demo
| Normal Mode             |  Control using LINE |
:-------------------------:|:-------------------------:
[![Normal Mode Demo](https://img.youtube.com/vi/gPgk_Gjs5QA/0.jpg)](https://www.youtube.com/watch?v=gPgk_Gjs5QA) | [![control using line](https://img.youtube.com/vi/ehnaCmpzqFE/0.jpg)](https://www.youtube.com/watch?v=ehnaCmpzqFE)
