cmake_minimum_required(VERSION 3.12)
project(Kinematics)

set(CMAKE_CXX_STANDARD 11)

add_executable(Kinematics main.cpp JoystickArm.cpp Servo.cpp Node.cpp Angles.cpp)