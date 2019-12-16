



## DodgeQuest


For a thousand years Bob the Ravager and his foul magics  have caused much suffering throughout the lands of Alabala.Many brave warriors have tried defeating him and all of them have failed.Yet,if the rumors are true,there is one way to do so.Should enough time pass,Bob's impenetrable shield will dissipate,leaving him vulnerable to attacks.
   
So, what do you think young adventurer?Do you belive you can succeed when so many have not?Will you pass this test of perception and endurance?


**How it plays:**
As sugested by the title,the game revolves around dodging the attacks the antagonist will throw at you.You will do so by moving vertically,horizontally or diagonally.Movement is acomplished using the joystick.

The game's objective is to survive all 4 levels and get as high a score as possible.The maximum score is 2046.

Each level has a countdown that tracks how much time has to pass untill the either the next level begins or the game ends in a victory.

The countdown as well as the level you are on and the number of lives you have are displayed on an lcd screen.The game itself is displayed on an 8x8 LED matrix.

Your character is represented by a blinking red dot on the screen,in contrast to the other red dots that do not blink which represent an enemy attack.

If your avatar gets hit by one of the attacks,you will lose a life.Lose all 3 and you lose the game.

After being hit,you will be invulnerable for 1 second.You will also be invulnerable for the first second upon entering the game.

You can replenish your lives by completing a level without losing any of the ones remaining.One of your lives will be replenished by doing so.

The feature above is set by default but it can be disabled in the settings menu(also displayed on the lcd) in exchange for double the score.

In the settings menu you can also choose what level to start on. Do note however that you will get less points for not completing the previous levels and if the starting level is 4 you will not get your score double if you disabled bonus lives(would be cheating).

**How the score is calculated:**
Each time a level is completed  (4^level)*nr_of_lives _left is added to the score.The ammount added is doubled if bonus lives are disabled(unless in the case above).

**Types of attacks:**

  ***Arrows***:  
  <pre>                     !                       </pre>
  <pre>                     v                 -- >   </pre>
 
They can be refered to as the game's basic attack.They move along a straight path,be it a line or a column.

There are 2 types of arrows: vertical and lateral.

The vertical one always comes from the top of the screen whereas the lateral one will come from the side closest to you.

The vertical one is introduced from the very beggining while the lateral one starts appearing from level 2 onwards. The vertical arrow is not present in the last level.
 
Other than that (and obviously their trajectory) they function the same way.They have the same speed and the same shape(line made of two points).

Both will 'spawn' based on the column or line you are curently standing on.


***Spikes:*** 
 <pre>                                                                         A     A     A      </pre>                                  <pre>                                             A     A     A               !     !     !      </pre>
 <pre>                      A    A    A            !     !     !               !     !     !      </pre>                                

Spikes will slowly rise from and return to the ground as shown above.Their main aim is to discourage the player from staying on the bottom lines to better avoid the vertical arrows.However they still allow the player to use those rows if careful.

They will only spawn if the player enters the 4 bottom lines.

They are introduced from the very beggining.


***Fireball Cluster:***
<pre>                 o         o                    o                     o       </pre>
<pre>                      o                                    o                  </pre>      

Also refered to as the Wave in the code(doue to its inital shape),the Cluster will split in 3 when the player moves out of its path.

The Cluster appears in the 4th level,replacing the top arrow.

When split,the ball from the center will go straight down while the others will go down diagonally,one to the left and one to the right.

The best way to deal with Clusters is to keep moving.The earlier they split,the less dangerous they should become.


***Meteor:***
<pre>                                                            #              </pre>
<pre>                        #                           #              #            </pre>
<pre>     #           #             #             #                            #          </pre>
<pre>                        #                           #              #            </pre>
<pre>                                                            #              </pre>

The Meteor acts as a shockwave-type ability,starting from a single point and spreading outwards as shown above.

The Meteor appears direactly near the player(but only diagonally in regards to the avatar)once every 5 seconds if he is in the middle(between rows and columns 3 and 6).

When a Meteor is about to appear and while it is active the Arrow attacks won't spawn.This is a good indicator of when it will strike.

It only appears in level 3.


**Require Hardware:**

-Arduino Uno;

-8x8 LED matrix;

-MAX7219 Driver;

-2x16 LCD screen;

-Joystick;

**Links:**
Picture of my setup:

Gameplay footage:
