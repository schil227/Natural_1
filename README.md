# Natural_1
Teaching myself C through designing a Game

3/24/15: I feel that I've got somewhat of a grip on the win32 API at this point, so I'm beginning the first iteration of this project! I have basically 3 main goals:
1.) Create a 'character' object which will be used as the parent class for the player/npc/etc.
2.) Create a 'field' object which will act as the main playing field container
3.) Make it so the field moves with the player / player moves with field



3/5/15: This is the initial commit for the project. The "game" at this point is just a window with a few dialog boxes, but it's the start of something magical. I am actually quite impressed with how C handles creating GUIs, especially with the .rc files; it all seems very, dare I say, intuitive? This coming from a background in swing (which I've often gone out of my way to ignore). The difficulty I'm encountering is finding a good tutorial, fortunately http://www.winprog.org/ has guided me thus far, leaving out bits and pieces for me to learn on my own. In short, I plan on going through the tutorial a little more, and once I have a good understanding of what's going on, I'll begin the design of the actual game with a more agile approach. 

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
