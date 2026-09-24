# Super Mario PRO2

A 2D platformer engine built entirely from scratch in C++ — no third-party game engine or libraries — for the Programming II (PRO2) course. Component-based architecture, full local 2-player support, and enemies with real behavioral logic (a segmented Wiggler, a multi-state flying boss).

<img width="908" height="613" alt="Image" src="https://github.com/user-attachments/assets/c50c4853-655b-4fe2-8744-bf5416e659db" />

## Contents

- [Running the project](#running-the-project)
- [Controls](#controls)
- [Architecture](#architecture)
- [Entities & collectables](#entities--collectables)
- [HUD](#hud)
- [Enemies](#enemies)
- [Respawn system](#respawn-system)
- [Level end](#level-end)
- [2-player mode](#2-player-mode)
- [Random mushrooms](#random-mushrooms)

---

## Running the project

From the project root:

```bash
make
```

Run the executable:

```bash
./bin/mario_pro2
```


---

## Controls

### Player 1 (Mario)

| Key | Action |
|---|---|
| `←` | Move left |
| `→` | Move right |
| `↑` | Sprint |
| `↓` | Crouch |
| `Space` | Jump |
| `B` | Dance |

### Player 2 (Luigi)

| Key | Action |
|---|---|
| `A` | Move left |
| `D` | Move right |
| `W` | Jump |
| `S` | Crouch |
| `V` | Sprint |
| `Q` | Dance |

### Special functions

| Key | Action |
|---|---|
| `E` | Debug mode (shows colliders) |
| `U` | Immediate respawn |
| `P` | Pause / resume — **note:** there is no visual indicator that the game is paused |
| `2` | Spawn or remove Luigi |

---

## Architecture

The project follows a **component-based design**:

- **`BoxCollider`** — rectangular collision detection.
- **`Animator`** — manages sprites and frame-by-frame animation.

Every entity and collectable is built from these components, which keeps new entities fast to create and easy to maintain.

- **`ParticleSystemManager`** — a component that handles on-screen particle effects: respawn particles, enemy-defeat effects, jump and run dust, and similar.


---

## Entities & collectables

**Entities** — from Goombas and Koopas to the various mushrooms.

**Collectables** — traditional coins, and the **End Star**, a Super Mario Galaxy–style golden star.

---

## HUD

Handled by the `HUD.hh` class, showing on screen:

1. Coins collected
2. Time in the current run
3. Mushrooms collected

<img width="962" height="636" alt="Image" src="https://github.com/user-attachments/assets/010cd0e5-f0ed-4a14-a242-13e1c47c01cf" />


---

## Enemies

### Wiggler

Inspired by linked lists: each body segment is a node.

- Jumping on a segment splits it off, and it reappears as a new, independent Wiggler.
- Sinusoidal movement and a stepping animation, driven by the `Animator` component.

### Kamek

Flies around the screen with multiple states:

| State | Behavior |
|---|---|
| `Idle` | Floats in the air |
| `Move` | Picks a position on screen and flies toward it |
| `Summon` | Summons enemies |

<img width="817" height="437" alt="Image" src="https://github.com/user-attachments/assets/e9bbf5bc-eaa8-4c34-86a8-d947274d9c2a" />


---

## Respawn system

Flags are placed throughout the level. Touching one updates Mario's respawn point.

If Mario dies, he reappears at the last flag he activated.

---

## Level end

A special collectable, the **End Star** (inspired by Super Mario Galaxy 2). Collecting it triggers a short end-of-level sequence, and your run's stats are displayed on screen.

<!-- IMAGE HERE: end-of-level screenshot -->

---

## 2-player mode

Press `2` to spawn or remove Luigi, who appears alongside Mario. If Luigi leaves the screen, he's automatically removed.

<img width="571" height="396" alt="Image" src="https://github.com/user-attachments/assets/52511789-bb1f-469c-b0ea-b69cd59e6e5d" />

---

## Random mushrooms

Hitting a `?` block drops a random mushroom: Normal, Red, Green... and the Giant Mushroom.

<img width="249" height="165" alt="Image" src="https://github.com/user-attachments/assets/d5ee4e55-50f9-4f28-bb2e-fd2f4700a053" />
