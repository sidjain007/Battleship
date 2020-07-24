# Battleship

A terminal based version of the classic 2 player battleship game using C++17

## Battleship Rules
The official rules can be found [here](https://www.hasbro.com/common/instruct/Battleship.PDF).

## Playing

Clone the repo and cd into the directory. Then run the following commands:

```
$ g++ -std=c++17 board.cpp battleship.cpp main.cpp
$ ./a.out
```

Based on your terminal window size, consider increasing the value of ```lines``` in ```flushOut()``` in ```battleship.h``` to properly flush output before switching turns

