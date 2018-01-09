# "Blast It" - Project Bomb Test
#### _Bomberman_ Remake test in Unreal Engine 4.18

This is simple remake of BomberMan in Unreal Engine 4.18

## Code docs at:
https://glenwrhodes.github.io/BombProjectTest/

# Getting started
- Download the repo, and load in Unreal Engine.  This has been developed with 4.18.2, Preview 4
- Launch the editor and compile the C++ code
- Start the game with Play, from the MENU scene (GameLevel/Menu)
- Enjoy!

# Features
- Procedurally generated map - Automatically generates a bomberman-style map, using different 'tiles' - grass, unbreakable and breakable pieces. Procedural map also regenerates the navigation mesh for future AI.
- Local 2 players - just spheres for now.  Overriding the Player viewport client class required to make 2 players use the same keyboard. As outlined here: https://wiki.unrealengine.com/Local_Multiplayer_Tips
- Left player WSAD and Left-Shift for bomb.  Right player Arrow keys, and Right shift for bomb.
- Bombs planted explode left / right and up / down only, according to a range which can be powered up with pickups.
- Powerups - Extra bomb, increase explosion range, increase player speed, remote controlled bombs. 
- Remote controlled bombs are triggered with the same key that is used to plant them. This effect lasts 10 seconds.
- Smart camera that pans and zooms to keep all relevant items on screen - players, powerups and bombs.
- Simple AI that follows you and tries to bomb you, runs away, and goes after pickups as they appear.
- Powerups are driven by simple blueprints, making it easy for a game designer to create other powerups that hook into exposed properties without needing to write any C++.

# Third Party Assets:
- 3 textures from purchased assets - Painterly terrain textures, and Landscape stone. https://www.assetstore.unity3d.com/en/#!/content/15334 
- Pickup assets from purchased asset pack: https://assetstore.unity.com/packages/3d/props/items-for-mobile-7767

# Future improvements / changes
- Don't use GetAllActorsOfClass in the smart camera.  Keep a record of bombs, players and powerups via the Game Mode.
- More sound effects.
- Music.
- Better user interface.
- Network support for multiplayer online.
- Player graphics - replace spheres with characters.
- AI - smarter enemy player.
- More powerup types.
- Patrolling monsters.

# Notes
- Some of the C++ aspects might be better served in Blueprints, and vice versa. 
- Total development time, approximately 14.5 hours. 
- AI is strictly blueprints. With more time, it would be made into a combination of C++ and Behaviour Trees / Blackboards.
