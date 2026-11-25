# The Last Train - Programming Fundamentals Project (Procedural / No Structs)

CS-1002: Programming Fundamentals (AI & DS)

## Group Members

- Abdul Ahad 26i-0091
- Abdullah Tahir 26i-0060

## Architecture Overview

This project uses only procedural C++. There are no structs and no classes. All state is stored in `int`, `char` and `bool` variables and in multi-dimensional arrays.

- `include/`: header files (`config.h`, `state.h`, `level.h`, `simulator.h`, `physics.h`, `switches.h`, `logger.h`, `util.h`, `viewer_iface.h`).
- `src/core/`: implementation files.
- `data/`: building map (`building.txt`), initial state (`initial-state.txt`) and input test scripts.
- `out/`: save file, game summary and the determinism proof (`out/proof/`).
- `docs/`: `Project_Diagram.jpg` (architecture), `Flowchart.png` (game flow) and `ConsoleUI.png` (screenshot).
- `tests/`: small test programs, one for each part of the code.

## Build Instructions (g++ C++17)

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/core/*.cpp -o last_train
```

The build prints no warnings. Run the game from the project folder:

```bash
./last_train        # Linux / Mac
.\last_train        # Windows PowerShell
```

## How to Play

The game first asks:

1. `n` for a new game, `r` to resume the saved game in `out/savegame.txt`, or `quit`. `N` and `R` also work. Any other input is rejected and the game asks again.
2. For a new game, the starting room as two numbers, for example `5 2` (floor 5, room 2).

Commands during the game:

| Command                 | What it does                                      |
| ----------------------- | ------------------------------------------------- |
| `move <floor> <room>` | go to a connected room, e.g. `move 1 2`         |
| `inspect`             | search the room (finds clue digits)               |
| `collect <item>`      | pick up an item by its symbol, e.g. `collect <` |
| `use <item>`          | use an item you carry, e.g. `use *`             |
| `unlock <code>`       | enter the cabinet code in F5-R1                   |
| `board`               | board the train at F5-R2 when ready               |
| `objectives`          | show the checklist                                |
| `save`                | save the game to `out/savegame.txt`             |
| `help`                | show the command list                             |
| `quit`                | leave the game                                    |

The goal: rescue the passenger and the cat, open the cabinet with the code to get the Train Key, and board the train at F5-R2 before Energy or Time reaches 0.

## Game Rules in Brief

- The player starts with Energy 100 and Time 50. Both are limited to 0-100.
- The only Time gain is the Observation Token (+15, F5-R4). Two Energy Packs (+25 each) are in F1-R4.
- Lift 1 connects floors 1 and 2 (rooms 2 and 3 only). Lift 2 connects F2-R4, F4-R4 and F5-R4. It works after the Fuse is installed and the Lift Pass is held, or by using the 3 trips given by the Emergency Access Card.
- The player chooses the starting room. The game can be won from 6 of the 20 starting rooms: F4-R1, F4-R2 and all four rooms on floor 5. From the other rooms the game is playable but cannot be won within 50 Time.

## Running the Sample Scripts

Each script in `data/` is a list of commands, one per line. The game reads them instead of the keyboard. Every script starts with `n` (new game) and then the starting room.

```bash
./last_train < data/test-input-win.txt                         # Linux / Mac
Get-Content data\test-input-win.txt | .\last_train             # Windows PowerShell
```

| Script                  | Expected ending                                                |
| ----------------------- | -------------------------------------------------------------- |
| `test-input-win.txt`  | `MISSION COMPLETE`, final score 480                          |
| `test-input-f5r4.txt` | `MISSION COMPLETE`, final score 480 (hardest winnable start) |
| `test-input-lose.txt` | `MISSION FAILED: you ran out of Time`, final score 0         |
| `test-input-bad.txt`  | invalid-input messages, then `Game ended.`                   |

## Verifying Determinism

The same input must always give the same output.

1. Create the `out/proof` folder if it does not exist. Then run the win script twice and save the files each time:
   ```powershell
   Get-Content data\test-input-win.txt | .\last_train > out\proof\run1_screen.txt
   Copy-Item out\game_summary.txt out\proof\run1_summary.txt
   Copy-Item out\savegame.txt out\proof\run1_save.txt

   Get-Content data\test-input-win.txt | .\last_train > out\proof\run2_screen.txt
   Copy-Item out\game_summary.txt out\proof\run2_summary.txt
   Copy-Item out\savegame.txt out\proof\run2_save.txt
   ```
2. Compare each pair:
   ```powershell
   Compare-Object (Get-Content out\proof\run1_screen.txt) (Get-Content out\proof\run2_screen.txt)
   Compare-Object (Get-Content out\proof\run1_summary.txt) (Get-Content out\proof\run2_summary.txt)
   Compare-Object (Get-Content out\proof\run1_save.txt) (Get-Content out\proof\run2_save.txt)
   ```

   On Linux / Mac use `diff` on the same file pairs.
3. No output means the files are identical, so the game is deterministic. Our saved results are in `out/proof/`.

## Files

- `data/building.txt`: the ASCII map (never changed by the game)
- `data/initial-state.txt`: starting Energy, Time, score and flags
- `out/savegame.txt`: written by the `save` command; each save overwrites the previous one
- `out/game_summary.txt`: written when the game ends

## Tests

`tests/` holds small programs, each with its own `main`, that check one part of the code at a time. Example:

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude tests/test_cabinet.cpp src/core/simulator.cpp src/core/physics.cpp src/core/switches.cpp src/core/level.cpp src/core/util.cpp src/core/viewer_iface_core.cpp -o test_cabinet
./test_cabinet
```
