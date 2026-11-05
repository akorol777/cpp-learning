================================================================================
TASK: CAR MOVEMENT OPTIMIZATION
================================================================================

TASK OBJECTIVE
==============

Write a program that finds an OPTIMAL solution for a car to travel a specific 
distance. The car starts with speed 0 and can change speed by at most 1 unit 
per step.

OPTIMAL means using the MINIMUM number of steps to reach the target distance.

At each step:
  1. Analyze remaining distance to destination
  2. Decide action: accelerate, brake, or maintain speed
  3. Move forward at the current speed

The car must:
  - Reach the target distance exactly (no overshooting)
  - Use the minimum number of steps possible
  - End at speed 0 (stopped at the finish line)


INPUT PARAMETERS
================

  - Target distance: positive integer (units the car must travel)

Initial state:
  - Speed: 0
  - Distance traveled: 0


CONSTRAINTS
===========

Speed:
  - Minimum: 0
  - Maximum: 4
  - Can change by at most 1 per step (accelerate +1, brake -1, or stay same)

Movement:
  - Each step, car moves forward by distance equal to its speed
  - Example: if speed = 3, car moves 3 units

Accuracy:
  - Must reach target distance exactly (no overshooting)


OUTPUT FORMAT
=============

Print each step as:

Step | Action       | Length | Total Distance

Where:
  - Step: step number starting from 1
  - Action: start / accel / brake / const_speed
  - Length: speed used in this step (distance moved)
  - Total Distance: cumulative distance traveled

Then print summary:

FINAL RESULTS:
  Total steps: [number]
  Distance traveled: [number] units
  Target distance: [number] units
  Status: [SUCCESS or FAILED]


TEST CASES
==========

Your solution should work for:
  - Distance 1
  - Distance 5
  - Distance 10
  - Distance 15
  - Distance 25

All should reach the target distance exactly.

================================================================================
