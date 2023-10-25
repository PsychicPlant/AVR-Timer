# AVR-Timer

Project encompassing **embedded electronics' circuit design and programming**. Class of 2022.

This project has two parts: the **integrated circuit's combinational logic assembly** on a breadboard, and the **embedded programming** of an **ATMEGA8515 microcontroller** in pure **Assembly**.

The goal of the project was to create an embedded system that could **receive user input through a PC's Console Terminal using USART**, and by using **dip-switches**, to control **memory-mapped devices** on the board.

Said devices were: one 16x2 LCD display, one 7-segment LED display & one Analog-to-Digital Converter.

# Features

I designed the program loop as a **Finite State Machine**, meaning it loops through a state until the user (or internal logic) comes to disrupt that loop. In which case the program counter jumps to another loop, or falls back to the default IDLE loop.

Once powered on, the system would send **banner messages** to the Terminal through its **serial TxR interface**, and display banner messages on the **LCD**. The internal clock will start ticking by using an external **interrupt**, triggered by an **oscillator** circuit.

The system will then fall into the loop defined by the on-board dip-switch, which can be to either show and interact with the internal clock, or to trigger **ADC sampling** of a on-board **potentiometer**.

The documentation explaining the program logic, commands, design choices, memory allocation and errors can be found above in the ***avrtimer_report.pdf*** file.

Here are the project's instructions and goals defined by my teacher: [project_instructions.pdf](https://github.com/PsychicPlant/AVR-Timer/files/13169928/project_instructions.pdf)
