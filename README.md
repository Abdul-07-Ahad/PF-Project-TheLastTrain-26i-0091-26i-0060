# The Last Train - Programming Fundamentals Project (Procedural / No Structs)

CS-1002: Programming Fundamentals (AI & DS)

## Architecture Overview

This project strictly follows primitive procedural programming concepts:

- **No Structs / No OOP**: All variables, game statuses, inventory, and building rooms are stored in primitive types (`int`, `char`, `bool`) and multi-dimensional parallel arrays.
- `include/`: Modular C++ header definitions (`config.h`, `state.h`, `level.h`, `simulator.h`, `physics.h`, `switches.h`, `logger.h`, `util.h`, `viewer_iface.h`).
- `src/core/`: Modular implementation files.
- `data/`: Building map (`building.txt`) and configuration (`initial-state.txt`).
- `out/`: Runtime save states (`savegame.txt`) and session summary reports (`game_summary.txt`).
- `docs/`: Blueprint diagrams (`Project_Diagram.jpg`) and documentation.

## Build Instructions (g++ C++17)

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/core/*.cpp -o last_train
./last_train
```


## Group Members

- Abdul Ahad 26i-0091
- Abdullah Tahir 26i-0060

## How to Play

Start the game with `./last_train` after you have compiled the game and named the executable file as last_train (Windows: `.\last_train`). The game first asks these when you start it:

1. `n` for a new game, `r` to resume the saved game in `out/savegame.txt`, or `quit`."(`N` and `R` are accepted too). Any other input is rejected and the game asks again."
2. For a new game, the starting room as two numbers, for example `5 2` (floor 5, room 2).

Commands during the game:

| Command                 | What it does                                          |
| ----------------------- | ----------------------------------------------------- |
| `move <floor> <room>` | go to a connected room, e.g. `move 1 2`             |
| `inspect`             | search the room (finds clue digits)                   |
| `collect <item>`      | pick up an item, using its symbol, e.g. `collect <` |
| `use <item>`          | use an item you carry, e.g. `use *`                 |
| `unlock <code>`       | enter the cabinet code in F5-R1                       |
| `board`               | board the train at F5-R2 when ready                   |
| `objectives`          | show the checklist                                    |
| `save`                | save the game to `out/savegame.txt`                 |
| `help`                | show the command list                                 |
| `quit`                | leave the game                                        |

The goal: rescue the passenger and the cat, open the cabinet with the code for the Train Key, and board the train at F5-R2 before Energy or Time reaches 0 . "The game is like a maze: you move between rooms and complete the objectives."

## Running the Sample Scripts

The folder `data/` holds three input scripts. Each one is fed to the game as if typed: Each script is a list of commands, one per line, that the game reads instead of the keyboard or the inputs that the game would get when we run it so each command is typed in next line

```bash
# Linux / Mac
./last_train < data/test-input-win.txt
# Windows PowerShell
Get-Content data\test-input-win.txt | .\last_train
```

| Script                  | Expected ending                                        |
| ----------------------- | ------------------------------------------------------ |
| `test-input-win.txt`  | `MISSION COMPLETE`, final score 480                  |
| `test-input-lose.txt` | `MISSION FAILED: you ran out of Time`, final score 0 |
| `test-input-bad.txt`  | invalid-input messages, then `Game ended.`           |

## Verifying Determinism

The same input must always give the same output.

1. Run the win script twice and save the output files each time:
   ```powershell
   Get-Content data\test-input-win.txt | .\last_train > out\proof\run1_screen.txt
   Copy-Item out\game_summary.txt out\proof\run1_summary.txt
   Copy-Item out\savegame.txt out\proof\run1_save.txt

   Get-Content data\test-input-win.txt | .\last_train > out\proof\run2_screen.txt
   Copy-Item out\game_summary.txt out\proof\run2_summary.txt
   Copy-Item out\savegame.txt out\proof\run2_save.txt
   ```
2. Compare each pair (create the `out/proof` folder first):
   ```powershell
   Compare-Object (Get-Content out\proof\run1_screen.txt) (Get-Content out\proof\run2_screen.txt)
   Compare-Object (Get-Content out\proof\run1_summary.txt) (Get-Content out\proof\run2_summary.txt)
   Compare-Object (Get-Content out\proof\run1_save.txt) (Get-Content out\proof\run2_save.txt)
   ```
3. No output means the files are identical, so the game is deterministic.

```bash
# Linux / Mac
diff run1_summary.txt run2_summary.txt
# Windows PowerShell
Compare-Object (Get-Content run1_summary.txt) (Get-Content run2_summary.txt)
```

No output means the files are identical. Our results are saved in `out/proof/`.

## Files

- `data/building.txt`: the ASCII map (never changed by the game)
- `data/initial-state.txt`: starting Energy, Time, score and flags
- `out/savegame.txt`: written by the `save` command or if existing previously than will be overwritten each time you save 
- `out/game_summary.txt`: written when the game ends

## Tests

`tests/` holds small programs that check one part at a time. It has those small files for the functions that we were working on so that they have a main function from where we could run and check them. For example:

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude tests/test_cabinet.cpp src/core/simulator.cpp src/core/physics.cpp src/core/switches.cpp src/core/level.cpp src/core/util.cpp src/core/viewer_iface_core.cpp -o test_cabinet
./test_cabinet
```
