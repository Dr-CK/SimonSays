# SimonSays
 A Simon memorization game clone
 
 Press the colored buttons in the right order to progress in the game
 
# Bill of materials:
   - Arduino pro mini
   - 4x 28mm LED illuminated arcade buttons
   - Piezo buzzer + current limiting resistor
   - Power switch
   - 9V battery + connector
   - 4x screws
   
# Hold down buttons during power up to set the game mode:
   - Green: Easy mode (three attempts instead of just one)
   - Red: Reverse mode (enter the sequence in reverse order)
   - Blue: Multiplayer mode (see below)
   - Yellow: Silent mode (No sound)
   - All buttons: Free play
   - Hold down all buttons until all lights are out to reset the highscore
   - (Buttons can be pressed in combination)
   
# Multiplayer mode
   1. Unlimited number of players
   2. A simple animation is shown before the game starts
   3. Player 1 picks the first color
   4. Player 2 presses the same color and then choses the next color
   5. Every player in turn has to input the correct sequence as usual, and finally chose the next color in the sequence
   6. Press the wrong color and you are out!
   7. Last player standing wins!