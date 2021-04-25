# Lumia

Lumia is a golf platformer mobile game. The objective of finishing the level is to light up all magical plants through colliding with them.

## Controls
### Mobile
- 1 finger aim and launch to move Lumia
- tap on the Lumia body to switch control
- 1 finger double-tap to split controlling Lumia
- 2 finger tap/ hold to merge nearby Lumias to the controlling Lumia; nearby Lumias will move toward the controlling Lumia while two fingers hold, and merges upon contact
- 4 finger tap to reset level (used for debug only)
### Desktop
- Mouse drag: aim and launch to move Lumia
- Click on the Lumia body to switch control
- Key `S` to split controlling Lumia
- Hold `M` to merge nearby Lumias to the controlling Lumia
- Key `R` to reset level (used for debug only)

## Notes
*[Updated: 04/25/2021 for Closed Beta Release]*
### Enemies
The "red ball" in the level is the enemy. If Lumia has a smaller size than the enemy, the enemy would chase after Lumia to eat up Lumia. Each time the enemy comes in contact with Lumia, Lumia looses some size, until it goes below minimum size and dies. If Lumia is larger than the enemy, however, the enemy runs away from Lumia. At this time, the enemy is a moving energy source than the Lumia would "eat up" and gain size once they collide with the enemy.

### Lumia Size Levels
For better control, we have limited Lumia to have six possible sizes with their corresponding densities. To solve the "infinite mass" problem we had from alpha release, Lumia splits into two bodies with different sizes at some levels (e.g. a Level 5 Lumia splits into a Level 2 and a Level 3). This will be made more clear and intuitive once we add size indicators during open beta.

### Spikes
The Red blocks on the bottom tile at Level 1 are the spikes. Every time Lumia collides with a spike, it looses mass (goes down a size level), until it goes below minimum size and dies. Since we added this a bit late, we did not had the chance to add spikes to the level design of more levels.

### Sticky Walls
The white velcro that lies on top of tiles are sticky walls that allows the player to aim and relaunch Lumia, in order to navigate through more complex level structures.

### Doors and Buttons
Doors and buttons are added as a pair on Level 4. While Lumia stays on the button, the corresponding door moves to another position. Since we don't have the corresponding assets, we are currently using the square blue block on the platform as the button and the blue block as the door it is controlling. 
