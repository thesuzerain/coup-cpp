# coup-cpp
A C++ implementation of the card game Coup with a simplified AI.

Coup is a 2-4 player game designed around bluffing and deception. Each player starts with two "influences" (cards) that are hidden from other players. They can perform certain actions depending on those cards, lying if they have to.

A simple ruleset can be viewed here: <https://upload.snakesandlattes.com/rules/c/CoupTheResistance.pdf> 

---

coup-cpp is an implementation of the game in C++.

To install:

1) Clone the repository to your local disk.

2) Import the project into your IDE (CMakeLists.txt is included to load it in cmake).

3) Use cmake to compile. 

```
./coup-cpp
```

---

This game does use colored text using ANSI/VT100 escape sequences: theser will display as black on most Windows machines.

---

Update 2022-10-26: Revisited this on a whim to refactor some of it without completely changing the way it is set up (and fix a couple small things).