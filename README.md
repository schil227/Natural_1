# Natural_1
Teaching myself C through designing a Game

3/5/15: This is the initial commit for the project. The "game" at this point is just a window with a few dialog boxes, but it's the start of something magical. I am actually quite impressed with how C handles creating GUIs, especially with the .rc files; it all seems very, dare I say, intuitive? This coming from a background in swing (which I've often gone out of my way to ignore). The difficulty I'm encountering is finding a good tutorial, fortunately http://www.winprog.org/ has guided me thus far, leaving out bits and pieces for me to learn on my own. In short, I plan on going through the tutorial a little more, and once I have a good understanding of what's going on, I'll begin the design of the actual game with a more agile approach. 

Because I Know I'm going to need this...

Eclipse Build Steps:
First, the .rc files must be compiled. In eclips:
Project -> Properties, then 
C/C++ Build -> Settings.
Click the Build Steps tab. This will compile the .rc files before the other stuff.
Under Pre-build Steps, enter 'windres ../x.rc -o ../window_icon.o'*
Click back on the Tool Settings tab
drill into MinGW C Linker -> Miscellaneous
Under Other objects, add 'window_icon.o'
Under Linker flags, enter '-mwindows'

*awful file names, will change in the future
