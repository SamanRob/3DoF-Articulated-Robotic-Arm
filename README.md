# 3DoF and 4Dof-Articualated-Robotic-Arm
Self constructed and fully 3D printed 3DoF robotic arm.

![Untitled Project 2](https://user-images.githubusercontent.com/83728692/177980119-6e5f3cc5-1cf4-4b21-a91a-fac3ba990add.jpg)

The robotic arms contain 3 and 4 degree of freedom. They are controlled both by Arduino and using Nema 17 stepper motor in combination with both Planetary gear or Timing belt and pullies. The robotic arm is fully self contructed and 3d printed and used for pick and placing components. An idea was to use the robotic arm in combination with OPC UA communication Protocol to communicate with 3d printer to pick and place the printed componens fully automated from the 3d printer's platform.

The arms are controlled as mentioned with Arduino in combination with python. There is graphical user interface designed via Python which send different commands to the Arduino board. These commands will be send as an array and later will be modified via arduino code to use for different functions. The robotic arms have the following functions:

Homing function using limit switches to come to the 0 Position
Gripper Control
Inverse kinematics
Forward Kinematics

![Screenshot 2022-07-08 131000](https://user-images.githubusercontent.com/83728692/177981308-cef9f6ef-97be-44e3-9730-49b95bbc23a4.png)

![Untitled Project](https://user-images.githubusercontent.com/83728692/177980128-f48e22b6-9d35-4c78-9a0d-d6e8c94bec17.jpg)
