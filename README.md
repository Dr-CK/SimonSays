# SimonSays
 A reimagining of the ![Simon](https://en.wikipedia.org/wiki/Simon_(game)) memorization game.
 
 Press the colored buttons in the right order to progress in the game!

<img src="https://github.com/Dr-CK/SimonSays/blob/main/SimonSays_v3.jpg?raw=true"
  alt="Simon Says game"
  title="Simon Says game"
  style="display: inline-block; margin: 0 auto; max-width: 300px">

## Putting it together

 Follow the instructions below to build your own!

### 3D printed enclosure

 The enclosure is also available on ![Printables](https://www.printables.com/model/404689). If you do make your own, consider uploading a Make!

 The enclosure prints in two parts, I printed the bottom part completely without supports, which is not the prettiest on the inside, but it works. For the top part I recommend blocking support structures for the speaker mount and hole, but using supports for the roof and power switch mount.

 I printed mine in white PLA on an Ender 5 Pro (0.4mm nozzle) with 0.2mm layer height, 15% grid infill, and 4 vertical shells.

### Bill of materials
 - Arduino pro mini
 - 4x 28mm LED illuminated arcade buttons (something like ![this](https://www.arcadeworlduk.com/products/arcade-world-uk-ultimate-5v-illuminated-button-28mm.html))
 - Piezo buzzer + current limiting resistor
 - Power switch
 - 9V battery + connector
 - 4x 3mm screws

### Assembly
 1. Upload the supplied code to the Arduino.
 2. Mount the buttons in the following clockwise order: Red - Green - Yellow - Blue
 3. Fit the piezo buzzer in the circular mount.
 4. Mount the power switch in the rectangular hole.
 5. Connect all components according to the schematic.
 6. Attach the 9V battery.
 7. Close the two halves of the enclosure using the four 3mm screws.

## Play time!

 The basics should be fairly intuitive, but here are some details about how it works: The goal is to repeat the displayed sequence for as long as possible.
 There's no explicit way to see the current sequence length, but there will be a little melody playing after finishing 8, 16, 24, etc... steps.
 It will also play another melody when setting a new high score. 

### Hold down buttons during power up to set the game mode
 - Green: Easy mode (three attempts instead of just one) - Easy mode has it's own high score
 - Red: Reverse mode (enter the sequence in reverse order)
 - Blue: Multiplayer mode (see below)
 - Yellow: Silent mode (No sound)
 - All buttons: Free play
 - Hold down all buttons until all lights are out to reset the highscore
 - (Buttons can be pressed in combination)
   
### Multiplayer mode
 1. Unlimited number of players
 2. A simple animation is shown before the game starts
 3. Player 1 picks the first color
 4. Player 2 presses the same color and then choses the next color
 5. Every player in turn has to input the correct sequence as usual, and finally chose the next color in the sequence
 6. Press the wrong color and you are out!
 7. Last player standing wins!
 
### Screen saver

 After a while of inactivity, it will start flashing the buttons just for fun. To get back to the game, hold down any button during the change between animation sequences.
 
 Wait a bit longer and there will be another surprise :)