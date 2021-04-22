# sfml-rs2d
Non commercial Runescape 2D version Made with C++/SFML
Some assets are property of Jagex: https://www.jagex.com/en-GB/
Dev blogs on https://www.youtube.com/user/thelatestartosrs/videos

To run the game you must: (please contact me if you need help, I will make a tutorial on how to setup everything!)
1. run "python scripts/main.py" (changed chunks to reduce loaded area if you dont want to way 2 hours, lumbridge is {18, 13})
2. Setup SFML 2.5.1 (it should be setup like so already in the project https://www.sfml-dev.org/tutorials/2.5/start-vc.php)
   - Add an environment variable "SFML_RS2D_HOME" pointing to your sfml folder, ex: C:\Users\Bruno\Desktop\prog\SFML-2.5.1-windows-vc15-32-bit\SFML-2.5.1
   - copy the SFML DLLs to the debug folder containing the .exe created when compiling (in visual studio, debug or build (F5/F6), it will crash because of missing DLLs and create the debug folder)
3. Add an environment variable "ODBC_RS2D_HOME" pointing to a folder containing your DB connection string file, name that file "dbinfo.txt"
4. run an sql server and create a database named "rs2d"
6. run the server
7. run client(s), enter only a username in the console and press enter to send info to the server, no login system currently, it will just lookup the username in DB (sql insert it yourself for now)

If the info above is wrong, let me know!

TODO Short term:
- Add tests :/
- Make resources server side
- Make inventory server side
- Skill experience
- Add an NPC for dialogues
- Add a monster for combat
- Add a quest (4-5 steps)
- Add a bank

TODO Long term:
- Instances (inferno/fight caves/raids/quest areas)
- Multiple Quests, monsters, NPCS, Shops
- Interfaces: shop/bank/etc.
- Skills (No construction probably)
- Clue scrolls
- Quests

Not sure:
- non 07 features
- diaries
- rest of the map
- My version of raids (how I imagined it before it was released)
- Different servers like DMM, fast xp, combat only, etc
