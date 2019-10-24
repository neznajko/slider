### *In*
**T**his was a tough problem, it's the first one from *1990*, in the
[doc](https://ioinformatics.org/files/ioi1990round1.pdf) there are
*6* problems, zo I'm **dumping** it here:
```C
////////q///////f///////3///////4///////z///////.///////-///////
// Program the process transforming figure arrangement in A into
// that in B. A figure in a cell can be transferred into any one
// of neighbouring empty cells; in the case of such transformati-
// ons the cell previously occupied by the figure becomes empty.
// +---+---+---+---+                   z   +---+---+---+---+   *
// | 7 | 3 | 5 | e |                    z  | 1 | 2 | 3 | 4 |   `
// +---+---+---+---+                  ,    +---+---+---+---+   .
// |   | 4 | 9 | d |                .      | 5 | 6 | 7 | 8 |   o
// +---+---+---+---+ --------------------> +---+---+---+---+   '
// | 1 |   | 2 | a |             _*        | 9 | a | b | c |   .
// +---+---+---+---+        /. ,`          +---+---+---+---+   _
// | b | 8 | c | 6 | A     /==|            | d | e |   |   | B *
// +---+---+---+---+      /====.           +---+---+---+---+   `
////////0///////g///////-/======?------->///////,```````________
#define _GNU_SOURCE//     |    |               
#include <stdio.h> //     |    |                          oo  ..
//^^^^^^^^^^^^^^^^^^^^^---|_n__|________________________________
#include <stdint.h> //--3-------4-------5-------6-------7-------
```
***Brute*** *force* searching won't work, for example consider
a position which differs from the final one by swapping the last
two figures. Then in the **ckout** all positions *searching* approach
it will start messing already *ordered* figures. I don't know what
is the *best* solution *(obviously)*, but I took more or less a
straightforward approach: The first *8* figures are escorted in the
usual manner as you and I would normally do if we have to solve that
puzzle. Than ***depth search*** is used on the restricted area of
the lower half.

### *Yan*
*Okay* the code is a bit messy and inconsistent so I'm not
discussing any technical issues. Only few words about the escorting
algorithm. Empty squares (called zeroes) can move easily, figures
need to have a cleared path before them. To guard figures from being
pushed around the program uses a *lock* stack. Having a destination
square, zeroes find their way through the maze of locked squares by a
depth search. The first zero is placed on the figure target square while
second zero is clearing the figure path towards the square just
below the target when there is a one more swap to finish the journey.