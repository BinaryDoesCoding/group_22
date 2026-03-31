# Lab 1: Pre-lab Worksheet: Bitwise Manipulation and Control Registers

This is an individual assignment, but you may discuss the material with others, so long as the work you submit is your own.

Note that we assume you have taken a course in embedded systems, where you were introduced to bitwise manipulation, hardware timers, and using those timers to control pins (e.g., PWM).

For this and everything you submit in this class:
* Show your work and explain your answers. You will not receive credit if you do not explain your steps.
* All calculations must include units not only for the answer, but the intermediate steps, as well.
* In most cases, work problems algebraically and put numbers in at the end. Occasionally, it will be cleaner to insert numbers earlier, but doing so should be rare.

New to markdown? No worries! Here's a good [Cheat Sheet](https://www.markdownguide.org/cheat-sheet/) and here is a [lengthier one](https://github.com/im-luka/markdown-cheatsheet?tab=readme-ov-file#lists).

In VSCode, you can preview the file by clicking the preview icon (the one with the magnifying glass) near the upper right. We'll even show you a picture.

![Click the icon with the magnifying glass.](figures/vscode-widgets.png)

> 1. Consult the datasheet for the ATmega32U4, specifically, the description of `TCCR4A` (“Timer/Counter4 Control Register A”) on page 162. In Normal mode, what effect does setting `COM4A1..COM4A0 = 0b01` have? [Note that we use the Arduino `0b` notation for a binary constant.]

Write your answer here, like this. You can write inline code `like this`, or you can set code in its own section 
```
void main(void)
{
    // like this
}
```

> 2. Consult the Romi Control Board pinout. Which pin is designated `OC4A` ("Output Compare 4A"). If `Timer4` rolls over every 20ms, what will happen when you set the `COM4A` bits as described in the previous question? Assume you have set the pin mode to `OUTPUT`.

The answer to 2 goes here.

> 3. Complete the code in the `bitwise_test` unit test. Specifically, complete the three functions near the top of `test_bitwise.cpp` that perform bit manipulation. Select the `env:bitwise-test` environment in VSCode and run the test program. Edit your code until it passes all tests. You do not need to paste anything here, but **push your repository to GitHub, where it can be autograded.**

> 4. Write a line of code that will set bit 6 in `TCCR4A` using bitwise manipulation (you may not use built in bit setting functions like `sbi()`). Your line of code must not change any other bits. `TCCR4A` is an `lvalue`, meaning to can assign it like a regular variable. Paste your line of code here.

> 5. Write a line of code that will clear bit 6 in `TCCR4A` using bitwise manipulation. Your line of code must not change any other bits. Paste your line of code here.

> 6. Do some research and calculations and show us how you determined the linear distance that corresponds to one encoder tick of the motor. You will need this value for converting wheel (rotational) speed to robot (linear) speed later. *Write your answer in terms of distance / tick.* 

Need to show formulas? You can write math equations with latex style. You can write equations inline, $x=y$, or on their own:

$F=ma \rightarrow a=\frac{F}{m}$

> 7. What microprocessor does the Romi have?

> 8. At what frequency does the system clock on the chip run?

> 9. How much SRAM does the chip have?

> 10. What pin is used to monitor the battery voltage? Explain why Pololu added the interface circuit that they built into the board (see the schematic).

> 11. For the battery monitor, draw a *diagram* that explains your logic. Note: you don't have to implement the monitor for this pre-lab, but we want to see you've thought about it, even if we haven't yet covered such diagrams. Give it a shot!

**Ready to submit?** When you're done, you'll need to submit a pdf file on Canvas (which will make grading much easier). Easiest is to install the Markdown PDF extension and export to pdf as described [here](https://www.programmingbasic.com/convert-markdown-files-to-pdf-vscode/).