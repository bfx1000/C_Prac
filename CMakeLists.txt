cmake_minimum_required(VERSION 3.24)
project(dianzhen C)

set(CMAKE_C_STANDARD 11)

add_executable(dianzhen  Matrix_Led.c External_Interrupt.c Timer_Counter_timing.c Timer_Counter_calc.c Serial_Transmit.c)
