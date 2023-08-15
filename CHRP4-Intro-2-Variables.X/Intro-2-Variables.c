/*==============================================================================
 Project: Intro-2-Variables             Activity: mirobo.tech/intro-2
 Date:    August 11, 2023
 
 This introductory programming activity for the mirobo.tech CHRP4 demonstrates
 the use of byte (char) constants and variables to count button presses and to
 then trigger actions when a limit is reached.
  
 Additional program analysis and programming activities demonstrate the use of
 bit (Boolean, or bool) variables to store state for preventing multiple counts
 of the same button being pressed during successive program loops. Additional
 activities include the creation of a two-player rapid-clicker game, simulation
 of a real-world toggle button and multi-function button, and writing code to
 measure and deal with switch contact bounce.
==============================================================================*/

#include    "xc.h"              // Microchip XC8 compiler include file
#include    "stdint.h"          // Include integer definitions
#include    "stdbool.h"         // Include Boolean (true/false) definitions

#include    "CHRP4.h"           // Include CHRP4 constants and functions

// TODO Set linker ROM ranges to 'default,-0-7FF' under "Memory model" pull-down.
// TODO Set linker code offset to '800' under "Additional options" pull-down.

// Program constant definitions
const unsigned char maxCount = 50;

// Program variable definitions
unsigned char SW2Count = 0;
bool SW2Pressed = false;

int main(void)
{
    // These configuration functions run once during program start-up.
    OSC_config();               // Configure internal oscillator for 48 MHz
    CHRP4_config();             // Configure I/O for on-board CHRP4 devices
    
    // The code in this while loop repeats forever.
    while(1)
	{
        // Count SW2 button presses
        if(SW2 == 0)
        {
            LED2 = 1;
            SW2Count = SW2Count + 1;
        }
        else
        {
            LED2 = 0;
        }
        
        // Has the max count number been reached?
        if(SW2Count >= maxCount)
        {
            LED3 = 1;
        }
        
        // Use SW3 to reset the count and turn off LED D3
        if(SW3 == 0)
        {
            LED3 = 0;
            SW2Count = 0;
        }
        
        // Add a short delay to the main while loop.
        __delay_ms(10);
        
        // Reset the microcontroller and start the bootloader if SW1 is pressed.
        if(SW1 == 0)
        {
            RESET();
        }
    }
}

/* Learn More -- Program Analysis Activities
 * 
 * 1. The 'SW2Count' variable is created as an 8-bit memory location within RAM
 *    by the program declaration: 'unsigned char SW2Count = 0;'
 * 
 *    What is the the maximum value an 8-bit variable can store? What are some
 *    of the benefits and drawbacks of using 8-bit variables in an 8-bit
 *    microcontroller?
 * 
 * 2. The constant 'maxCount' is defined using a declaration similar to that
 *    used for the SW2Count variable, but with the addition of 'const' prefix in
 *    the declaration statement, like this: const unsigned char maxCount = 50;
 * 
 *    The maxCount constant is used to check if the maximum count has been
 *    reached by this statement in the main while loop: if(SW2Count >= maxCount)
 * 
 *    It seems like a lot of work to define a constant. The program could have
 *    simply been written to use this statement instead: if(SW2Count >= 50)
 * 
 *    Can you think of any advantages of declaring a constant first, using a 
 *    declaration statement above the main code like this program does, rather
 *    than just embedding the value of the constant into the code wherever it
 *    is needed?
 * 
 * 3. This program should light LED D2 every time SW2 is pressed, and light
 *    LED D3 once the count reaches 50. Try it, and count how many times you
 *    have to press the button until LED D3 turns on. (SW3 resets the count so
 *    you can perform repeated attempts.)
 * 
 *    Did your count reach 50? Can you describe what the program is doing?
 *    (Hint: try pressing and quickly releasing the button, or pressing and
 *    slowly releasing the button.)
 * 
 * 4. Let's add to the second condition block to help figure out what the
 *    program is doing. Modify the second 'if' structure in the program to add
 *    the 'else' block shown below:

        if(SW2Count >= maxCount)
        {
            LED3 = 1;
        }
        else
        {
            LED3 = 0;
        }

 *    Now, press and hold pushbutton SW2 for at least 10 seconds while watching
 *    LED D3. If the value of SW2 count is higher than maxCount, LED D3 should
 *    stay lit continuously. Does it do that?
 * 
 *    If LED D3 turns off and does not stay lit, what can you infer about the
 *    value of the SW2Count variable? Can you explain what is happening to the
 *    SW2Count variable as the SW2 button is held?
 * 
 * 5. We can set a limit on the SW2Count variable by encapsulating its increment
 *    statement inside an if condition statement. In your program, replace the
 *    single line 'SW2Count = SW2Count + 1;' with the block of code, below:
 
            if(SW2Count < 255)
            {
                SW2Count += 1;
            }

 *    This code demonstrates the use of the assignment operator '+=' to shorten
 *    the statement 'SW2Count = SW2Count + 1;' The same operation is performed,
 *    but in a more compact form. After adding this code, what is the maximum
 *    value that the SW2Count variable will reach? How does this affect the
 *    operation of LED D3 when SW2 is held?
 *
 * 6. The fundamental problem with this program is that pushbutton SW2 is sensed
 *    during each cycle of the main() loop. If SW2 happens to be pressed when
 *    it is read during each cycle through the loop, another count is added to
 *    the SW2Count variable. Since microcontrollers are very fast, the count 
 *    can add up very quickly!
 * 
 *    Instead of responding to the state of SW2, we want the program to respond
 *    to a *change* of SW2 state, from the not-pressed state to the pressed
 *    state. Doing this requires the use of a second variable to store the prior
 *    state of SW2, so that its current state can be evaluated as being either
 *    the same, or different, from its state during the previous iteration of
 *    the main() loop. Replace the initial SW2 if-else condition structure with
 *    the following two if condition structures:

        // Count new SW2 button presses
        if(SW2 == 0 && SW2Pressed == false)
        {
            LED2 = 1;
            SW2Pressed = true;
            if(SW2Count < 255)
            {
                SW2Count = SW2Count + 1;
            }
        }

        // Clear pressed state if SW2 is released
        if(SW2 == 1)
        {
            LED2 = 0;
            SW2Pressed = false;
        }
        
 *    These two if conditions make use of the Boolean SW2Pressed variable to
 *    store the current state of SW2 for every cycle of the main() while loop.
 *    Boolean variables can store either 0/false, or 1/true. The first 'if'
 *    condition, above, compares the current SW2 state with the previously
 *    stored SW2Pressed variable, and only adds to the count when SW2 is being
 *    pressed *and* the SW2Pressed Boolean variable is false. In this case the
 *    switch press must be a new press, and the state of the Boolean variable
 *    will be switched to true before checking and incrementing SW2Count.
 * 
 *    After SW2Pressed is set, the SW2Count variable will not be able to 
 *    increment again so it must be reset when the button is released. The next
 *    if structure does that, resetting the SW2Pressed Boolean to false when SW2
 *    is released, and enabling the process to repeat. Try the code to verify
 *    that it works.
 * 
 *    The conditional statement in the first if condition can also be written:

        if(SW2 == 0 && !SW2Pressed)

 *    The '!SW2Pressed' expression is read as 'not SW2Pressed' and is equivalent
 *    to SW2Pressed being false. Similarly, using the variable name by itself
 *    (eg. SW2Pressed) in a condition would be equivalent to it being true.
 * 
 * 7. A pushbutton's logic state can also be defined using a constant definition
 *    to help make the code more readable. Add the following lines to the
 *    'Program constant definitions' section at the top of your program code:
 
#define pressed 0
#define notPressed 1

 *    Now, instead of comparing the state of the button to 0 or 1, the button
 *    input can be compared with the named definition just provided, making the
 *    program more readable. (This comes at the expense of hiding the actual 
 *    switch logic value in the definition statement, whereas before it was
 *    plainly visible in the if structure.)
 * 
 *    Try it in your code, and modify the SW3 reset button to work with the same
 *    pressed and notPressed definitions.
 
        // Count new SW2 button presses
        if(SW2 == pressed && SW2Pressed == false)
        {
            LED2 = 1;
            SW2Pressed = true;
            if(SW2Count < 255)
            {
                SW2Count = SW2Count + 1;
            }
        }

        // Clear pressed state if released
        if(SW2 == notPressed)
        {
            LED2 = 0;
            SW2Pressed = false;
        }
        
 * 
 * Programming Activities
 * 
 * 1. Can you make a two-player rapid-clicker style game using this program as 
 *    a starting point? Use SW5 for the second player's pushbutton so that the
 *    two players can face each other from opposite sides of the circuit board.
 *    Duplicate the SW2Count and SW2Pressed variables to create SW5Count and
 *    SW5Pressed variables. Then, duplicate the if condition structures to
 *    modify the variable names to represent the second player. LED D3 can still
 *    light if player 1 is the first to reach maxCount and wins. Use LED D4 to
 *    indicate when player 2 wins. Use a logical condition statement to reset
 *    the game by clearing both players' counts as well as to turn off the LEDs
 *    if either SW3 or SW4 is pressed.
 * 
 * 2. Use your knowledge of Boolean variables and logical conditions to simulate
 *    a toggle button. Each new press of the toggle button must 'toggle' an LED
 *    to its opposite state. (Toggle buttons are commonly used as push-on, 
 *    push-off power buttons in digital devices.) Pressing and holding the
 *    toggle button should only cause the LED to toggle once, and not keep
 *    toggling continuously until the button is released.
 * 
 * 3. A multi-function button can be used to enable one action when pressed, and
 *    a second or alternate action when held. A variable that counts loop cycles
 *    can be used to determine how long a button is held (exactly as the first
 *    program unitentionally did because of the loop structure). Make a
 *    multifunction button that lights one LED as soon as a button is pressed,
 *    and lights a second LED after the button is held for more that one second.
 * 
 * 4. Do your pushbuttons bounce? Switch bounce is the term that describes
 *    switch contacts repeatedly closing and opening before settling in their
 *    final (usually closed) state. Switch bounce in a room's light switch is
 *    not a big concern, because we wouldn't notice the lights rapidly flashing
 *    before staying on. But, switch bounce is an issue in a microcontroller
 *    toggle button program because the speed of a microcontroller lets it see
 *    each bounce as a new, separate event, making it perform multiple actions
 *    in response to one button press.
 * 
 *    Create a program that uses a variable to count the number of times a
 *    pushbutton is pressed and to then display the count on the LEDs. Use a
 *    second pushbutton to reset the count and to turn off the LEDs so that the
 *    test can be repeated. To determine if your switches bounce, try pressing
 *    them at various rates of speed and using different amounts of force.
 * 
 * 5. Did your pushbuttons bounce? Can you think of a technique similar to the
 *    multi-function button that could be implemented to allow your program to
 *    ignore switch bounces?
 * 
 *    Typical switch bounces appear as multiple switch activations within an
 *    approximatley 10ms time span. So, if a program can be made to ignore any
 *    new switch activations for 20ms after the first switch closure, it should
 *    totally eliminate the effects of switch bounce on its operation. See if
 *    you can add switch de-bouncing code to your bounce counting program in 4,
 *    above, and then test it to see how effective it is.
 */
