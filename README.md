# Natural_1
Teaching myself C through designing a Game

7/23/16: So last iteration went fairly well - It's definitely looking more like a game! While I haven't checked for memoryleaks, I'll try to do some this iteration. Speaking of, I'll be working on the A.I. finally, and once this hurdle is done with, I think that it'll be (mostly) smooth sailing (I still have to come up with the story, of couse). Just another wall to break down - success is inevitable!

4/30/16: Hooray! Abilities are implemented - far and beyond my initial statements in my previous iteration wrap-up. Also, I've set a streak of coding 35 days in a row! While it has taken a while, I'm pleased with the progress I've made. It was also this iteration that I decided to make this github repository private - my long term goal is to publish this game on Steam. For this next iteration, before I begin working on the A.I., I think I need to flesh out a few more basics, such as sound and animation. Also, I'll do some QA testing work and look for some bugs, and patch any new memory leaks (Gotta make sure this application is C worthy).

2/5/16: Well, that wasn't as bad as I had anticipated. I found a way to implement npcs, dialogs, and events, and I indirectly made the game consistant as the player moves between fields and easier to save thanks to expanding on the global registry. It really does feel like the game is being pulled together, especially with this update. The next thing to implement (which has been a long time coming) is Stats and magic. There will be a lot of overhead with this iteration because creating the interface for creating spells could be difficult, especially with the level of complexity. I think I'll try to keep it simple, and just go for the standard fireball spell.

12/16/15: Here's the iteration I had been dreading - however, I feel rather optimistic : Implementing a dialog system and NPCs! While I havent committed this past week, I have been actively thinking about how I'm going to implement the dialog system. With this commit, I have made it so a dialog box pops up when looking at an item description - I've also laid out the dialog structures to handle messages, decisions, and events (dialog.h). Now it's a matter of assembling the whole thing.

11/18/15: Wow, that was quick! I'm pleased with the velocity that this project is having--between the first and second iteration: 133 days--second and third: 75-- third and forth: 31! Keeping in mind that the next thing coming up is NPC (which will likely be harder than this previous iteration), I feel that it's time to do some clean up. Now presenting: the mini iteration! I'm going to take some time to fix up things I missed, including: enemy movement animations, plugging memory leaks, fixing the console graphic, and any other little things that come to my attention.

10/18/15: Today marks the end of another iteration, having successfully implemented last iterations goals - including making the console twice! They way I figure it, there's probably about 4-5 iterations worth of content to yet implement (in order): Items and a Global registry, NPCs, Magic System, A.I., Story/world building. So next up comes creating items, an inventory/equip  system for individuals, inventory for field spaces, and a global registry system to keep track of what happens.

8/4/15: Wow, time bloody flies, huh? Haven't updated this in a while, guess I'll just make a note here saying work is steady and I've conquered a lot, and I'm modestly proud of what I have accomplished so far--however there is a stunning amount of work yet to do. My C-fu is getting better, and I've gotta say I have a new found appreciation for some higher-level languages, but at the same time they take all the fun out of it :) The goals below (and their subsequent surprises) have been finished for a while, I've moved on to my new set: implementing windows for displaying data to the user (soon to be revised to a single window), managing field transformations (going from one field to another) and camera-lock on the player as they move around... we'll see what surprises happen this time.

3/24/15: I feel that I've got somewhat of a grip on the win32 API at this point, so I'm beginning the first iteration of this project! I have basically 3 main goals:
1.) Create a 'character' object which will be used as the parent class for the player/npc/etc.
2.) Create a 'field' object which will act as the main playing field container
3.) Make it so the field moves with the player / player moves with field

3/5/15: This is the initial commit for the project. The "game" at this point is just a window with a few dialog boxes, but it's the start of something magical. I am actually quite impressed with how C handles creating GUIs, especially with the .rc files; it all seems very, dare I say, intuitive? This coming from a background in swing (which I've often gone out of my way to avoid). The difficulty I'm encountering is finding a good tutorial, fortunately http://www.winprog.org/ has guided me thus far, leaving out bits and pieces for me to learn on my own. In short, I plan on going through the tutorial a little more, and once I have a good understanding of what's going on, I'll begin the design of the actual game with a more agile approach. 

Because I Know I'm going to need this...

Eclipse Build Steps:
First, the .rc files must be compiled. In eclipse:
Project -> Properties, then 
C/C++ Build -> Settings.
Click the Build Steps tab. This will compile the .rc files before the other stuff.
Under Pre-build Steps, enter 'windres ../resources/window_resources/main_window_resources.rc -o ../window_resources.o'
Click back on the Tool Settings tab
drill into MinGW C Linker -> Miscellaneous
Under Other objects, add 'window_resources.o'
Under Linker flags, enter '-mwindows'
