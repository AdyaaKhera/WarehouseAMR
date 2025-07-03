# Warehouse Autonomous Mobile Robot (AMR)

- An Arduino-based autonomous mobile robot that follows lines, detects and avoids obstacles, and picks up/drops loads using servo mechanisms. 
- This project combines **embedded systems**, **robot control**, and **real-time sensor integration**.

---

## Features

- **Line Following** using two digital IR sensors
- **Obstacle Avoidance** via Ultrasonic Distance Sensor
- **Servo-Controlled Arm & Gripper** for load handling
- **Fail-Safe Logic** to reacquire path after obstacle

---

## Hardware Used

| Component             | Description                     |
|----------------------|---------------------------------|
| Arduino Uno          | Main microcontroller            |
| L298N Motor Driver   | For controlling 2 DC motors     |
| 2x IR Line Sensors   | Digital output for line detect  |
| Ultrasonic Sensor    | HC-SR04 for obstacle detection  |
| 2x Servo Motors      | 1 for arm, 1 for gripper        |
| 3D printed           | To form the base                |
| Chassis + Wheels     |                                 |
| Power Supply         | 12V Battery pack*               |

* Might need to use a separate power source for the grippers. Our battery could not provide enough power to the entire system.
  

## How It Works

1. **Line Following:**  
   - Reads both IR sensors to determine alignment.
   - Adjusts motor output to stay centered on line.

2. **Obstacle Avoidance:**  
   - Stops when an object is <5 cm away.
   - Executes backup + side-move + re-alignment sequence.

3. **Load Handling:**  
   - Grabs a load at startup.
   - Drops it if line is lost for more than 60 seconds (Assuming that it stays offline for more than 1 minute when it has reached its destination).

## Future Improvements

This project was created collectively as a part of the E100 class. It still has a lot of modifications left and a few errors in the code when integrated with hardware due to power supply inefficiency. After all the adjustments have been made, there is still room for more improvements like:

- Adding **PID control** for smoother line following
- Replacing IR with **camera + OpenCV** for visual navigation
- Integrating **Bluetooth or IoT** for remote override
