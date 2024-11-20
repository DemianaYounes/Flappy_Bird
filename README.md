# Flappy Bird Game on ATmega32 with Graphical LCD

## Overview

This project implements a Flappy Bird game on the ATmega32 microcontroller with a graphical LCD. The game is built using three important concepts:

1. **Stack State Machine**: The game uses a stack-based state machine to manage different game states, such as game start, game over, and game paused.
2. **Event-Driven Architecture**: The system responds to events such as button presses and timer interrupts to control the game flow.
3. **Floating Point Unit**: Floating point operations are utilized for movement and speed calculations to ensure smooth gameplay.

## UML Diagram
<div style="text-align: center;">
    <img src="https://github.com/user-attachments/assets/4ce4a8fa-2c02-4d69-9432-f6c32d8fd4b6" alt="Traffic Light" width="900" height="400"/>
</div>

## Problem: Tearing Issue

In embedded systems, specifically with ATmega32 and graphical displays, a common problem is "tearing." This issue occurs when an image is updated while it is being displayed, causing visual artifacts where part of the screen shows an outdated image while the other part shows the updated content.

### Cause of Tearing in This Project

In the Flappy Bird game, tearing happens when obstacles are frequently updated without synchronization, leading to a mismatch between the updated and old parts of the display.

## Possible Solutions for Tearing

There are several solutions that can address the tearing issue in embedded systems like ATmega32. Below are a few strategies and their relevance to this project:
## 1. Ping-Pong Buffering
- **What it is**: Use two separate regions (buffers) in memory. One buffer is actively being displayed while the other is updated. Once the update is complete, the roles of the buffers are swapped.
- **Pros**:
  - Ensures no tearing occurs, as updates are done off-screen.
  - Can be scaled to a partial buffer model for specific regions.
- **Cons**:
  - Memory intensive—requires space for at least two buffers.
  - Adds complexity in buffer swapping logic.
- **Suitability**: Works well for ATmega32 if implemented with small partial buffers to save memory.


## 2. Time-Sliced Screen Updates
- **What it is**: Divide the screen into multiple sections (e.g., top, middle, bottom) and update them sequentially using a timer.
- **Pros**:
  - Prevents tearing by limiting visible updates to small regions.
  - Efficient use of memory and processing time.
- **Cons**:
  - Requires complex logic for section updates.
- **Suitability**: Ideal for systems with strict memory limits, providing smooth animations.


## 3. Adaptive Frame Rate
- **What it is**: Dynamically adjust the frame update rate based on system load.
- **Pros**:
  - Balances performance and rendering quality.
  - Prevents overloading the CPU.
- **Cons**:
  - May cause slightly jerky animations during high load.
- **Suitability**: Excellent for ATmega32, where CPU resources are limited.


## 4. Interrupt-Driven Dynamic Regions
- **What it is**: Use interrupts to track regions requiring updates (e.g., bird movement or obstacles).
- **Pros**:
  - Efficiently uses processing time.
  - Minimizes unnecessary updates.
- **Cons**:
  - Requires logic to handle overlapping updates.
- **Suitability**: Ideal for event-driven architectures like your project.


## 5. Vertical Alignment of Updates
- **What it is**: Align updates to specific rows or columns to ensure orderly updates.
- **Pros**:
  - Simplifies logic for partial updates.
  - Reduces visual artifacts.
- **Cons**:
  - Requires tight control over rendering logic.
- **Suitability**: Feasible for ATmega32, particularly with time-sliced updates.


## 6. Efficient GLCD Driver Optimization
- **What it is**: Optimize the GLCD driver to speed up write operations and reduce redundant commands.
- **Pros**:
  - Reduces tearing by accelerating display updates.
- **Cons**:
  - Requires in-depth knowledge of the GLCD communication protocol.
- **Suitability**: Directly improves performance for ATmega32 systems.


## Enhancements to Existing Solutions

### Critical Section Updates
- Combine with **Partial Screen Updates** to ensure no concurrent modifications.
- Use event flags to safely manage updates in critical sections.

### Partial Screen Updates
- Maintain a **dirty region tracker** to log and prioritize regions requiring updates.

### Timer Interrupts
- Use timers to create predictable update windows (e.g., 60 Hz refresh rate).
- Synchronize timer events with event-driven tasks for reduced latency.


