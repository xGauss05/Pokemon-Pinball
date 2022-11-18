# Pokémon Pinball (Sapphire table)

- [Repository LINK](https://github.com/xGauss05/Pokemon-Pinball)

## Disclaimer
We do NOT own any of the information or images of this Game. Pokémon Pinball (Sapphire table) and all of the content used for the Game is the property of Nintendo. 
Every piece of content is used and can be used by anyone for non-commercial projects. Created for educational purposes.

## Description
A tribute to Pokémon Pinball Ruby & Sapphire using SDL and Box2D. 

## Key Features
- Wailmer will spit the ball towards the trough, be careful!
- Spoink is our spring. Charge Spoink to impulse the ball towards the table.
- Shroomish are bouncy fellas. Whenever you touch them, they will push back the ball.
- Lots of combos around the table! The H,O,L,E combo gives you an extra life if you manage to obtain them!
- The top red dots upgrade the ball so you can have a better score multiplier!
- A maximum of 3 lives. You can not obtain more than 3.
- Shows the Current Score, they Previous Score and the High Score. (Only in one session)
- If you touch Pikachu's flag, he might save the ball...
- Debug mode by pressing F1.

## Gameplay controls
- DOWN: Compresses the Spoink (Spring)
- LEFT: Moves the left flipper.
- RIGHT: Moves the right flipper.
- F1: Opens/Closes the Debug Menu.

## Debug keys
When the Debug Menu (F1) is Open. Press BACKSPACE anytime to return to the previous menu.
Press 0 in order to teleport the ball on the mouse position.

1. Time Options
    - W: Increases target FPS by 10.
    - S: Decreases target FPS by 10.
2. Gravity Options
    - W: Increases gravity by 1.
    - S: Decreases gravity by 1.
3. Sprites Options
    - W: Increases ballMultiplier by 1 (changes ball texture).
    - S: Increases ballMultiplier by 1 (changes ball texture).
4. Coefficient Options (Increase by pressing W, decrease by pressing S)
    - 1: Ball restitution
    - 2: Bumpers force
    - 3: Slingshots force
    - 4: Spring force
    - 5: Flippers force
5. Show Colliders (shows the table and ball colliders)
    - LMB: While pointing to the ball, drag and move it.

## Developers
- Jonathan Cacay - [xGauss05](https://github.com/xGauss05)
- Ethan Martín - [Ethanm-0371](https://github.com/Ethanm-0371)
- Víctor Martín - [VicMarBall](https://github.com/VicMarBall)
- Ariadna Sevcik - [AriSevcik](https://github.com/AriSevcik)

## Development
- We had problems on the ball not rotating correctly. We tried to change the friction between the scenario and the ball in order to make it spin, but that was not
the problem. The funny thing is that by calling the body and reset their mass data, it also removes the Inertia. So... we added a value to the Inertia and it
rotates how it should.
- With the spring, we had problems on making the ball not bounce every time it touches the spring without charging it. The reason why was this happening it was
because it always had the maximum motor force, so in order to fix it we have put a bigger mass than the ball.
- We had problems with the TeleportTo() function. Everytime we called it, it would trigger an Assertion error. We asked the teacher why was that happening, and he
told us not to do any transformations to the body in the OnCollision() function and to it in the Update() or PreUpdate(). The reason was because by teleporting
to another spot, the body matrices would get the previous state from the TeleportTo() function and the values of the new spot, and that would end up having
NULLS, INF, and things that a computer can not compute.
- Sensors were kind of a headache; Sometimes the ball would pass through one and it would not trigger. Luckily, there was a class lesson that taught us how to
prevent this kind of "missing" collisions. We basically added the property "bullet" set to true for the ball to prevent mising collisions.
