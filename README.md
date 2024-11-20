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

### 1. **Double Buffering**
   - **What it is**: Double buffering involves using two buffers for the graphics. One buffer is drawn while the other is displayed, and then they swap. This eliminates tearing by ensuring the entire screen is updated at once.
   - **Pros**: Provides smooth transitions and prevents tearing.
   - **Cons**: More memory is required to store the two buffers, which might be limited in some embedded systems.
   - **Suitability**: Double buffering might not be ideal for ATmega32, as it has limited SRAM (2KB). This solution would require careful memory management or an external frame buffer, which may not be feasible in this case.

### 2. **V-Sync (Vertical Synchronization)**
   - **What it is**: V-Sync synchronizes the frame update with the vertical blanking interval of the display. This ensures the screen is updated only during the non-visible portion of the display refresh cycle.
   - **Pros**: Prevents tearing and ensures smooth animation.
   - **Cons**: Not directly applicable to ATmega32, as it doesn't have hardware support for V-Sync and graphical LCDs typically don't provide V-Sync features.
   - **Suitability**: V-Sync is not a practical solution for this project because ATmega32 lacks the necessary hardware support, and most graphical LCDs don't support it.

### 3. **Critical Section for Drawing Updates**
   - **What it is**: Using critical sections to prevent the display from being updated while another update is in progress. By disabling interrupts during updates, you can ensure that drawing is completed without interruption.
   - **Pros**: Simple to implement and avoids tearing without requiring extra memory or hardware features.
   - **Cons**: May cause slight delays if too many updates occur frequently, as the processor is busy with the critical section.
   - **Suitability**: This is a good solution for ATmega32, as you can control when updates happen and ensure that only one operation is modifying the screen at a time.

### 4. **Partial Screen Updates**
   - **What it is**: Instead of updating the entire screen at once, only the portion of the screen that changes is updated. This reduces the risk of tearing by limiting the area of the screen being updated at any given time.
   - **Pros**: Reduces the impact of tearing by making smaller updates.
   - **Cons**: Can be more complex to implement, especially when managing obstacles and bird movements separately.
   - **Suitability**: This is a good solution for optimizing the use of resources on ATmega32. It requires more careful handling but can minimize tearing by focusing updates on specific regions.

### 5. **Use of Timer Interrupts for Updates**
   - **What it is**: Update the screen at regular intervals (e.g., using timer interrupts) to synchronize with the refresh rate of the display. This can help in preventing partial updates and tearing.
   - **Pros**: Ensures a more consistent update frequency, reducing tearing.
   - **Cons**: It might introduce slight lag or delays if the updates are not optimized, especially in an event-driven architecture.
   - **Suitability**: This is an ideal solution for your current setup, as the system is already event-driven, and using timer interrupts for regular updates can reduce tearing while avoiding complex buffer handling.

## Recommended Solution for ATmega32

Given the limited memory and hardware capabilities of ATmega32, the most practical solution for this project is **using critical sections** to prevent tearing during obstacle updates. Additionally, using **timer interrupts** to control update intervals ensures smooth transitions without overloading the system.

You can also experiment with **partial screen updates** to limit the drawing to the necessary regions (bird and obstacles) to further optimize the system.

## Conclusion

In this project, you’ve applied advanced concepts like stack-based state machines, event-driven architecture, and floating point calculations. The tearing issue can be mitigated using techniques such as critical sections, timer interrupts, and partial screen updates. These solutions align well with the constraints of the ATmega32 microcontroller, ensuring that the game runs smoothly without overwhelming the system’s limited resources.
