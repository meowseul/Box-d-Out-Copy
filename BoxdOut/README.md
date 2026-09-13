# BoxdOut

### GUI Credits

We used the SFML library to create our GUI, a free software that has been put into the public domain.
SFML is free of use for creation, reproducting, selling, and other fun verbs.


### How to compile

Run "cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
Run "cmake --build build"
Run "./build/bin/main"

### Why isn't my program compiling?

SFML depends on having certain libraries installed, theres a good chance that these are libraries that you do not have.
Look into how to install the library given in the error message to your machine in order to fix the error.
This may help- 
Visual Studio: (https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio#installation).

### How to Play
Its chess! (For two players on one laptop)

Click the pieces to move them to a blue "valid move" square
If pieces are within +- 1 value point of each other they initiate a FIGHT!

In a fight:
 the white player uses A to move left, D to move right, and Space to punch
 the black player uses the arrow keys to move left and right and enter to punch

once a fight is over, the winning piece will either move to the spot it wanted to or remain where it was, gaining a value point
this value point means they will automatically win against weaker pieces, changing how chess strategy works

### Bugs/Balancing Issues
Currently the value system isn't really balanced out too well due to it not having been playtested by an outside audience.
This will get updated soon! Also at the time of writing this READ.ME update, checkmate and check aren't there which is a huge oversite lowk but its okay...
I also think visually we need to have indicators of a piece's health or value on the chess board because it's a lot to keep track of, or alternatively having health reset after a piece wins a battle. The value indicators on the pieces would help players be able to strategize better without having to keep track of all the moving memory pieces.
