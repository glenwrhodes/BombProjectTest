# "Blast It" - Project Bomb Test
#### _Bomberman_ Remake test in Unreal Engine 4.18

This is simple remake of BomberMan in Unreal Engine 4.18

# Features
- Procedurally generated map - Automatically generates a bomberman-style map, using different 'tiles' - grass, unbreakable and breakable pieces. Procedural map also regenerates the navigation mesh for future AI.
- Local 2 players - just spheres for now.  Overriding the Player viewport client class required to make 2 players use the same keyboard. As outlined here: https://wiki.unrealengine.com/Local_Multiplayer_Tips
- Left player WSAD and Left-Shift for bomb.  Right player Arrow keys, and Right shift for bomb.
- Bombs planted explode left / right and up / down only, according to a range which can be powered up with pickups.
- Powerups - Extra bomb, increase explosion range, increase player speed, remote controlled bombs. 
- Remote controlled bombs are triggered with the same key that is used to plant them. This effect lasts 10 seconds.
- Smart camera that pans and zooms to keep all relevant items on screen - players, powerups and bombs.

# Third Party Assets:
- 3 textures from purchased assets - Painterly terrain textures, and Landscape stone. https://www.assetstore.unity3d.com/en/#!/content/15334 
- Pickup assets from purchased asset pack: https://assetstore.unity.com/packages/3d/props/items-for-mobile-7767
