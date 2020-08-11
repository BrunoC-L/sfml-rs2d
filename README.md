# sfml-rs2d
Runescape 2D version Made with C++/SFML - only asset used is the map, the rest is remade by myself

To run the game you must:
1: run "python scripts/main.py"
2: setup project like so https://www.sfml-dev.org/tutorials/2.5/start-vc.php
3: download SFML, place the SFML folder next to the sfml1 folder at the root of the repo (mine is gitignored)
4: copy the SFML DLLs to the debug folder with the .exe created when compiling (go in visual studio, try to debug, it will crash because of missing DLLs and create the debug folders and executable)

If the info above is wrong, let me know!

TODO Long term:
Multiplayer
-Instances? (inferno/fight caves/raids/quest bosses)
-Multiple Quests, monsters, NPCS, Shops
-Interfaces: shop/bank/misc. 
-Skills, xp, skill requirements, scaling (No construction probably)
-clue scrolls

TODO Short term:
-add an NPC (for dialogues)
-add a monster (for combat)

Not sure:
-diaries
-rest of the map
-non 07 mechanics (will likely try to make TOB though if i get there because it seems like a good challenge)
-My version of raids (how I imagined it before it was released)
-Different servers, some compatible and some like DMM that are separate
-Construction
