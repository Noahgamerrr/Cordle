# Cordle
Wordle written in C

# How to play it
This project uses [my map implementation](https://github.com/Noahgamerrr/CMap), which is an opaque datatype, this if you want to run the game, you'll need to compile the main.c file together with the map.c file

### Compile with GCC
If you use gcc as a compiler, run the following command to compile the code in the source-folder:
```
gcc main.c map.c -o main.exe
```
Now you can run the code with:
```
.\main
```

### Compile with Clang
With clang, you'll need to:

Compile the files into object files with
```
clang -c main.c
clang -c map.c
```
Then link the object files to a binary file:
```
clang main.o map.o
```
Last but not least, run the binary file:
```
.\a.out
```

## Rules of the game
The rules are simple: You need to guess a word with five letters and you are given 6 attempts. If your guess contains a letter that is also part of the word which is being looked for, then it will be marked as yellow. If the letter is on the same position (say you guessed p***ri***c***e*** and the word you're looking for is b***ri***d***e***), then this letter will be marked as green. The markings will be seen on the guesses matrix as well as on the keyboard. If however, the colour of the letter in the matrix is green, but on the keyboard it's yellow, that means that this letter is inside the word at least one more time.

## Mentioning
You're free to use my project as you like, extend it, improve its functionality etc.. If however, you use it in your own project and you publish it, please mention me somewhere :\)

Have fun playing my game!
