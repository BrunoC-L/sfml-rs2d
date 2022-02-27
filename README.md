# sfml-rs2d

Non commercial Runescape 2D version. Some assets are property of [Jagex](https://www.jagex.com/en-GB/).

[Dev vlogs](https://www.youtube.com/user/thelatestartosrs/videos).

To run the client and server:

1. Contact me if you need help, please use Visual Studio 2019
2. Setup ODBC connection (OPTIONAL, can instead uncomment the in memory database option in server/app/main.cpp)
    - Make sure you have SQL Server Running
    - look at example.txt in root for examples of connection strings
    - Add an environment variable `RS2D_HOME` pointing to the root of the repo without a '/' or '\' at the end <https://i.imgur.com/3xFRSLl.png>
3. unzip [dependencies](https://github.com/BrunoC-L/sfml-rs2d/tree/master/include)
4. build the server
5. run the server (if there is a DB error, it is most likely a problem with step 1! read the first few output lines for DB errors)
6. build the client
7. lauch as many clients as you want with `app.exe local` from inside the client debug folder, or just launch 1 in debug
8. type username/pw, click sign up, type again and login

TODO:

- Chat
- Inventory
- Bank
- Skill experience
- Monsters
- Basic quests

Long term:

- Instances (inferno/fight caves/raids/quest areas)
- Multiple Quests, monsters, NPCS, Shops
- Shops
- Skills content (No construction probably)
- Clue scrolls

Probably not:

- non 07 features
- diaries
- rest of the map
- My version of raids (how I imagined it before it was released)
- Different servers like DMM, fast xp, combat only, etc
