# Powerfunction-MotorDriver
A motor driver to control two power function motors.

This driver is a target remote control system with two channels like a car. It works for RC car with motor for speed and a servo for the steer. But specially it programmed for tracked vehicles with two motors to control. For my project, I used the 42095 Lego Stunt-Racer with power functions. That uses remote control with infrared. The disadvantage thing is, it doesn't work fine outside by sunshine.

## Parts for build a electronic speed control (ESC)
Arduino mini pro 5V (you can used an other arduino)
Motor driver module MX1508
2x Servo cable
2x lego power function extension cables
Remote control system and a receiver with pwm output signal.

## Build the driver
For some more information to build the electronic speed control, look at my blog post.

![Bild von der Schaltung](https://github.com/Codexzier/Powerfunction-MotorDriver/blob/master/Schaltung.jpg)

## How to configure the motor driver 
Have you build the motor driver, you can start setup the driver.

###### 1. Connecting remote control with arduino
Connect the remote control with the Arduino.
Don't connect the lego power function motors, well it can be overload your Arduino.
- Receiver PWM Channel 1 to Arduino Pin A3
- Receiver PWM Channel 2 to Arduino Pin A2

###### 2. Setup motor driver - Set enable serial monitor
At first, you set the variable mSerialMonitor true and write it to the arduino.

```
 bool mSerialMonitor = true
```

###### 3. All stick must be middle
Channel 1
Read the value from 'Input 1' and enter the result to:
```
 int mInputMiddleA = 1500;
```

Channel 2
Read the value from 'Input 2' and enter the result to:
```
 int mInputMiddleB = 1500;
```

TIP: If the value jumps in a range, you must set a min max for tolerance.
```
 int mDeathbandPlusMinus = 10;
```

###### 4. Sticks to the minimum and maximum 
Channel 1
Set the speed control stick down and enter the result value to:
```
 int mInputMinA = 1000;
```
Set the speed control stick up and enter the result value to:
```
 int mInputMaxA = 2000;
```

Channel 2
Set the steer stick or wheel for drive **left** and enter the result value to:
```
 int mInputMinB = 1000;
```
Set the steer stick or wheel for drive **right** and enter the result value to:
```
 int mInputMaxB = 2000;
```

###### 5. Write code to arduino
Befor you write the code to arduino, return the variable to false:
```
 bool mSerialMonitor = false
```

###### 6. Test directions
Check up the direction.
If speed control up, but the vehicle drive back.
```
 boolean mReverseA = true; 
```

If steering left is go to the other.
```
 boolean mReverseB = true; 
```
TIP: Or for only tracked vehicle, you must change:
```
 boolean mVehicleMode_1_Steer = false;
```

## Have fun or send me a message
It is a simple project and this is my first complete writing readme file. But if you have any question, do not hesitete write a message.

