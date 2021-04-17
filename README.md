# Lumia

Lumia is a golf platformer mobile game. The objective of finishing the level is to light up all magical plants through colliding with them.

## Controls
- 1 finger aim and launch to move Lumia
- tap on the Lumia body to switch control
- 2 finger tap to split controlling Lumia
- 3 finger tap/ hold to merge nearby Lumias to the controlling Lumia
- 4 finger tap to reset level (used for debug only)

## Notes
*[Updated: 04/11/2021 for Alpha Release]*
### Enemies
The enemy runs away from the player Lumia if Lumia has a larger size than the enemy, at this time, the enemy is a moving energy source than the Lumia would "eat up" and gain size once they collide with the enemy. If Lumia has a smaller size than the enemy, the enemy would chase after Lumia to eat up Lumia. 

### Lumia Size Levels
For better control, we have limited Lumia to have four possible sizes with their corresponding densities. 

### Doors and Buttons
We have implemented buttons and doors. While Lumia stays on the button, the corresponding door moves to another position. Since we don't have the corresponding assets, we are currently using the square blue block on the platform as the button and the blue block high up in the screen as the door it is controlling. Unfortunately, since we merged this feature a bit late, we have not updated the level design with buttons and doors. 
