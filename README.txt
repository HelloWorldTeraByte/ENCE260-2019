How to upload:
    1) First connect the UCFK to a computer with avr toolchain installed
    2) The UCFK must be put into reset mode for the porgramming, This is done by pressing S1(the white switch near the external headers)
    3) Go into the directory location /assigment/group406
    4) Run command make program to program the UCFK

How to play the game.
        1)  you and the other player have to press the analogue stick on both device once and the game count down should start.
        controls
            left analogue stick : movement to the left;
            Right analogue stick : movement to the Right;  
            Block from left : hold the white button located on the left side of the platform or just above the characters head and press the up analogue to block left.
            Block from Right : hold the white button located on the left side of the platform or just above the characters head and press the analogue down to block right.
            left Hook: press up.
            Right Hook: press down.
            making a Jab: press the analoge stick would form an X pattern. 
        2)Details:
            The enemy is shown on the LED matrix with a stickman reperesntaion
            The player is shown as a dot on the lowest row of the matrix
            Inorder to get a hit on the character, your character position has to be in the center of the other character position and this will be able to get a 
            decisive blow on your rivary.
        3)  Health:
           -) you will see a blue led(LED1) blinking indicating that's the player health. 
           -) The dimmer the light the less health you have the lesser the chance of you winning.
           -) you get 100 health initally and each hit reduces this health
        4) scoring of damaging the player:
            left and right Hook: 5
            making a Jab: 10
            Block: Null
        The game can be restarted by pressing S1 and S2 simultaneously