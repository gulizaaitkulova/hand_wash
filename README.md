# Hand Wash Timer
![box](assets/box.jpg)    
Welcome to the Touchless Musical Hand Wash Timer project! Our goal is to help stop the spread of Covid-19 and improve hygiene discipline in a fun and interesting way.

This touchless hand wash timer displays a countdown clock to remind users to wash their hands for the recommended amount of time, and also displays the room's humidity and temperature. Additionally, it plays music while the timer is running to make the hand washing process more enjoyable.

The target audience for this project is students at INHA University, and it can be used in the university's restrooms. Our aim is to raise awareness about the importance of handwashing and encourage people to adopt proper hygiene habits.

We hope you find this project useful and informative. Thank you for your interest!    

## Hardware Requirements
* Arduino Board (Mega)
* Ultrasonic Sensor
* Buzzer
* 4-Digit 7 Segment Display
* Humidity Sensor
* LCD Display 

## Setup
1. Connect the infrared sensor to the breadboard. Connect the VCC pin to 3.3V or 5V on the Arduino board, the GND pin to GND on the Arduino board, the TRIG pin to digital pin 2, and the ECHO pin to digital pin 3.
2. Connect the speaker or buzzer to the breadboard. Connect one end to GND and the other end to digital pin 8.
3. Upload the code to the Arduino board using the Arduino IDE.

## Usage
1. Place your hand under the infrared sensor to start the hand-washing timer and music.
2. Wash your hands for at least 20 seconds while the music plays.
3. The music will stop automatically after 20 seconds.

## Notes
* Make sure the infrared sensor is positioned at a distance that allows it to accurately detect the presence of a hand.
* The volume of the music may need to be adjusted depending on the type of speaker or buzzer used.    
     
## Circuit
![circuit](assets/circuit.jpeg)

## Demo
You can check out out demo video [here](https://www.youtube.com/watch?v=D734M-M7Pic&ab_channel=GulizaAitkulova).
    
I hope this project helps you promote proper hand-washing habits and adds a little bit of fun to your daily routine. Enjoy!