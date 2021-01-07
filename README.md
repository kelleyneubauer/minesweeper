# MINESWEEPER

**Kelley Neubauer**\
*Summer 2020*

---

 Minesweeper clone for terminal with ncurses GUI interface

<img src="/img/minesweeper_menu.png" width="600">

<img src="/img/minesweeper_gameplay.png" width="600">


---

**Running minesweeper.c from cmd line:**
1. Navigate to minesweeper src folder using `cd minesweeper/src`
2. To compile, type `make`
3. To run, type `./minesweeper`
4. To clean up, type `make clean`.

---

**Gameplay:**

<img src="/img/minesweeper_gameplay_intermediate2.gif" width="600">

---

**How to play:**

- navigate with arrow or vim (hjkl) keys
- [g] - reveal selected
- [r] - reveal random
- [f] - flag selected
- [v] - verify that flags present valid solution\
      *does not check correctness*
- [c] - clear all flags
- [s] - solve puzzle\
      *does not guarantee a winning solution*
- [?] - help menu
- [q] - quit

---

**Minesweeper AI solver algorithm**

<img src="/img/minesweeper_solver.gif" width="600">

---

### What's next:
- [ ] improve solver algorithm to work for multi-cell solutions
- [ ] center menu and game to window. adjust when window is resized
- [ ] add multithreading where possible (ncurses is not thread safe)
- [ ] save high score (fastest time for each level)
- [ ] keep session and all-time win/ loss stats 
