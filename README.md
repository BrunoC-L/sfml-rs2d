# sfml-rs2d

Non commercial Runescape 2D version Made with C++/SFML
Some assets are property of Jagex: <https://www.jagex.com/en-GB/>
Dev vlogs on <https://www.youtube.com/user/thelatestartosrs/videos>

To run the client online: (server might be slow as HELL or even down)

1. Build the client, lauch as many as you want without arguments
2. type username/pw (switch with tab) and then click sign up, or just login if you already have signed up
3. logout by clicking the bottom banner (yes... I know)

To run the client and server locally:

1. Contact me if you need help, eventually I will make a video tutorial on how to setup everything!
2. Setup ODBC connection
    - Make sure you have SQL Server Running
    - look at example.txt in root for examples of connection strings
    - add a new file called config.json in root, by copying example.json, replace the content with your preferences
    - Add an environment variable called "RS2D_HOME" pointing to the root of the repo without a '/' at the end <https://i.imgur.com/3xFRSLl.png>
3. build the server
4. run the server (if there is a DB error, it is most likely a problem with step 1! read the first few output lines for DB errors)
5. build the client
6. lauch as many clients as you want with "app.exe local" from inside the client debug folder
7. type username/pw (switch with tab) and then click sign up, or just login if you already have signed up

TODO Short term: (few months)

- Re-add resources properly server side, they used to exist client side when prototyping!
- Skill experience
- Add monsters
- Add basic quests
- Add banks

TODO Long term: (not this year)

- Instances (inferno/fight caves/raids/quest areas)
- Multiple Quests, monsters, NPCS, Shops
- Interfaces: shop/bank/etc.
- Skills content (No construction probably)
- Clue scrolls

Not sure:

- non 07 features
- diaries
- rest of the map
- My version of raids (how I imagined it before it was released)
- Different servers like DMM, fast xp, combat only, etc
- mode without a tickrate, just goes as FAST AS IT CAN
- Mode where everyone is maxed but everything is untradable other than pvm supplies
