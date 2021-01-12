# MINESWEEPER

### Kelley Neubauer

Minesweeper for your terminal with ncurses GUI interface!

<img src="/img/minesweeper_gameplay.png" width="600">

[Play on repli.it!](https://repl.it/@kelleyneubauer/minesweeper)

---

**Running the game:**

1. Navigate to src folder `cd src`
2. Compile using `make` && 
3. Run with `./minesweeper`
4. Clean up using `make clean`


*Note: The terminal window must be large enough to display the menu and the game. There is no error checking for console size at this time. If output is cut off, stop the game, resize your window, and try again.*

---

**How to play:**

- Navigate with arrow or vim `h` `j` `k` `l` keys
- `g` - reveal selected
- `r` - reveal random
- `f` - flag selected
- `v` - verify that flags present valid solution
  - *checks validity, not correctness of flag placements*
- `c` - clear all flags
- `s` - solve puzzle
  - *does not guarantee a winning solution*
- `?` - help menu
- `q` - quit

---

**Gameplay & AI Solver demo:**

<img src="/img/minesweeper_gameplay_ai_demo.gif" width="400">

---

**Notes:**
* There may be some minor formatting issues on different systems: 
  * The console window must be wide enough to display the game. If the game is cut off, stop the repl, resize the window, and try again.
  * If the help menu message is cut off and stuck, hit enter to return to the game.
* It was a fun multithreading exercise to get the concurrent timer working but ncurses is *not* thread safe. I've attempted to make it so by moderating access to display output, but there may be some minor glitches. 

---

**What's next**
- [ ] improve solver algorithm to work for multi-cell solutions
- [ ] save high score (fastest time for each level)
- [ ] keep session and all-time win/ loss stats 
- [x] add multithreading for timer

