# sfml-rs2d
Runescape 2D version Made with C++/SFML - only asset used is the map, the rest is remade by myself
https://www.youtube.com/user/thelatestartosrs/videos

To run the game you must:
1. run "python scripts/main.py" (changed chunks to reduce loaded area if you dont want to way 2 hours, lumbridge is {18, 13})
2. setup projects (server/client) like so https://www.sfml-dev.org/tutorials/2.5/start-vc.php
   - add the additional include directory in C/C++ settings
   - add the additional libraries in linker settings
   - add additional dependencies in input settings
   - copy the SFML DLLs to the debug folder with the .exe created when compiling (go in visual studio, try to debug, it will crash because of missing DLLs and create the debug folders and executable)
3. run an sql server, change the odbc connection string to match the db, link odbc in visual c++ settings
4. add credentials in db for a user TODO: add db schema somewhere
5. run the server
6. run clients, enter username in console and press enter to send info to server

If the info above is wrong, let me know!

TODO Long term:
- Instances? (inferno/fight caves/raids/quest bosses)
- Multiple Quests, monsters, NPCS, Shops
- Interfaces: shop/bank/etc.
- Skills (No construction probably)
- clue scrolls

TODO Short term:
- multiplayer
- add an NPC (for dialogues)
- add a monster (for combat)

Not sure:
- diaries
- rest of the map
- non 07 features
- My version of raids (how I imagined it before it was released)
- Different servers like DMM
