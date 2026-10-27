# mogOS
A simple terminal-based folder system written in C++.

_mogOS is a small personal project created to learn C++, pointers, recursion, and basic file-system concepts._

## Features

- `pwd` Display the current path
- `mkdir` Create a new folder
- `cd` Navigate between folders (cd .. to move up)
- `rm` Remove a folder and its contents
- `ls` List folders in the current directory (or ls child)
- `clear` Clear the terminal
- `help` Show available commands
- `exit` Exit mogOS

### Compile

```
g++ main.cpp -o mogOS
```

### Run

```
./mogOS
```

## Future Updates

- [ ] `tree` command to visualize the folder structure
- [ ] `mv` to move and/or rename files
- [ ] Prevent duplicate folder names

### Maybe
- [ ] Custom fetch command to display system information
- [ ] Add support for creating and managing files!
- [ ] Implement saving and loading the folder structure
- [ ] Version control inspired by Git (init, add, status, commit, log)
