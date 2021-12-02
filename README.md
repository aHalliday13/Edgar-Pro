# Vex64040Edgar
VexCode project for 64040's E.D.G.A.R. Robot

# IMPORTANT BRANCH INFO
The `main` branch now contains auton code. Any previous statments about it not containing auton code are null.


# Code Style Guide
1. Code should always be arranged from top to bottom:
```
#include ...

using namespace vex;
competition Competition;

vars
macros

functions

pre-auton

auton

user control

main()
```
2. Place a comment at the top of each section `// define vars and macros here`
3. `if`,`while`,`void`, etc. should all be written in the format:
```
void functionName() {
  code();
 }
 ```
 for voids and
 ```
 if(condition) {
  do();
 }
 ```
for if and while

4. Each section should have ONE line of space between it and the next section
5. Each function should have ONE line of space between it and the next function
6. One line per line. Please don't write like this: `code();otherFunction();someOtherFunction();` because it's a nightmare to read and understand
7. Place spacing inside of long functions. Group groups of lines based on thier intended function.
8. Only edit code inside the VexCode editor. Do not edit on GitHub, because there is no syntax checking and something WILL get messed up.
9. Do not EVER commit code to the `main` branch without testing. If you don't get a chance to test, create a new (apropriatley named) branch and commit your changes to that branch.
10. Always leave descriptive, but short commit messages. If nesscesary, use the description box. Don't ever make long titles, just use the description box.

# Auton Routine (Right)
- Drive forward (50 in) to first neutral with claw open
- Grab and lift mogo
- Reverse (12 in) and turn (45 degrees left) so back faces aliance goal
- Grab mogo with rear lift
- Place preload in rear mogo while driving forward (15 in)
- Drop both mogos

# Auton Routine (Left)
 - Start with ringle elevator output facing aliance mogo
 - Dump preloads in mogo
 - Spin (90 degrees right) to face neutral mogo
 - Drive forward to neutral mogo
 - Grab neutral mogo
 - Reverse holding neutral mogo
 - Drop neutral mogo
