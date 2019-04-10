# coup-cpp
A C++ implementation of the card game Coup with functional AI.



Coup is a 2-4 player game designed around bluffing and deception. Each player starts with two "influences" (cards) that are hidden from other players. They can perform certain actions depending on those cards, lying if they have to.

A simple ruleset can be viewed here: <https://upload.snakesandlattes.com/rules/c/CoupTheResistance.pdf> 

---

coup-cpp is an implementation of the game in C++.

To install:

1) Clone the repository to your local disk.

2) Import the project into your IDE (CMakeLists.txt is included to load it in cmake).

3) Use cmake to compile. 

For those having trouble with cmake or otherwise having compilation troubles, a "coup-cpp" executable has been included with the files that you can run using the command:

```
./coup-cpp
```

---

This game does use colored text using ANSI/VT100 escape sequences: it may have issues running on Win32. I added in some code to only apply the colored text to non Win32, but this issue may still apply. 