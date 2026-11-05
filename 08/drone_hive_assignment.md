# Practical Assignment — Drone-Hive Control System

Topics exercised: classes, inheritance, virtual functions, smart pointers.

---

## System Overview

1. There is a **Ground Station (GS)** that can operate multiple hives at once.
2. Each hive is controlled by a single **`BeeQueen`** drone. `BeeQueen` **is a** `Drone` (inherits from it) — she has her own position, can be armed/disarmed, and can move, in addition to her hive-management responsibilities.
3. GS communicates with a hive **exclusively through its `BeeQueen`** — GS never talks to a subordinate drone directly.
4. Each hive can contain only **one type** of subordinate drone: `Bomber`, `FPV`, or `Reconnaissance`.
5. GS assigns drones to a specific hive by calling a `BeeQueen` method, e.g. `beeQ.AddDrone(...)`.
6. `BeeQueen` **must** report an error if a drone of a different type is added to an already-typed hive. When the hive's type gets fixed is an open design decision — pick one and be ready to justify it:
   - fixed at `BeeQueen` construction time, or
   - determined dynamically by the **first** drone added, with subsequent additions checked via `dynamic_cast` against that type.
7. `BeeQueen` must be able to report, to GS: the **number of drones** in her hive and the **hive's type**.
8. Every drone has a **unique ID**, unique **within its own hive** (not required to be unique across different hives, since GS always addresses a drone through its specific queen). `BeeQueen` must report her subordinates' IDs and their statuses.
9. Drones can have (at least) the following statuses — the list is not exhaustive, add more if your design needs them:
   - `Operational`
   - `DEAD`
   - `Disarmed`
   - `FAIL`
10. GS commands specific drone(s) to attack (if possible) by ID, via a `BeeQueen` method that takes drone ID(s) and target coordinates.
11. `Reconnaissance` drones cannot attack directly — calling attack on one instead makes it **move** to the target location.
12. `FPV` drones die on attack: after attacking, their status becomes `DEAD` (they're expended on the target).
13. `Bomber` drones carry up to **6 bombs**. Each attack drops **1 bomb**. If a `Bomber` is out of bombs, it reports `FAIL` instead of attacking.
14. `Reconnaissance` drones report target coordinates as random `(x, y)` pairs, for simulation purposes.
15. The queen of a recon hive must have a method to launch a **"free hunt"** — subordinate recon drones search for and report target coordinates (see *Free Hunt Algorithm* below).
16. Every drone can only be armed, disarmed, or moved **through its `BeeQueen`**, in response to GS requests — never directly by GS or by the drone itself.
17. `BeeQueen` must be able to return to her starting point, which is recorded when she is armed.
18. **Return sequence for an FPV hive**: subordinate drones are never brought back charged. During normal operation, an FPV drone that has attacked is only *marked* `DEAD` (its object still exists). On return, the queen must **actually destroy** all subordinate drone objects (clear the container that owns them — `vector`, `map`, whatever structure you chose) rather than merely flagging their status.
19. **Return sequence for a Bomber hive**: before returning, the queen must force all subordinate drones to unload any remaining bombs.
20. **Return sequence for a Reconnaissance hive**: no additional steps — it returns as-is.

### Additional rules (apply across all drone types)

- **Attacking (or issuing any action to) a non-`Operational` drone must fail**: report `FAIL` and take no other effect. This includes drones already `DEAD`, `Disarmed`, or in `FAIL` status themselves.
- **No hive operations are possible while its `BeeQueen` is not armed.** If the queen herself is not armed, GS cannot arm/disarm/move/attack any of her subordinates, request target coordinates, or query hive status through her — all such requests must fail cleanly (report an error/`FAIL`, do not crash or silently no-op).

### Coordinate representation

Left to your design (a `struct`, a `std::pair`, etc.) — you'll be asked to explain your choice.

---

## Simulation Guide

1. Create a GS, one or more `BeeQueen`s, and drones of each type; assign each drone to the appropriate hive.
2. Place several targets on a field (e.g. 200×200, or any size convenient for your design).
3. Do not begin attacking until a recon hive has actually reported a target's coordinates — attacks must be driven by discovered targets, not by coordinates the student hard-codes from knowledge of where targets were placed.
4. FPV/Bomber hives do not need to be moved near a target before attacking — for this assignment, assume they can strike anywhere on the field regardless of the queen's or drones' current position. (This is a simplification; a future extension may add a "free hunt"-equivalent search/approach mode for attack hives too, and simulate running out of aircraft, interceptors, etc.)

### Free Hunt Algorithm (Reconnaissance hives)

Free hunt is a `BeeQueen` responsibility. The **search logic itself is your design task** — the description below is a constraint set and a rough shape of the loop, not a prescribed algorithm:

- Each recon drone has a **view distance of 1** (it can only detect a target at most 1 step away from its current position).
- The queen may only search up to **10 steps** away from her own position.
- Rough loop shape: launch drones to a coordinate → poll whether any of them currently sees a target → if yes, record and report those coordinates to GS → send drones to the next coordinate → repeat.
- **You decide the loop's exit condition and the search pattern.** As a hint: with a view distance of 1, a real search should aim to cover roughly a 20×20 area before the queen relocates and starts a fresh search from a new position — otherwise you risk moving on and missing a target that was actually nearby. How exactly you tile/step through that area is up to you.
- If, after exhausting the search within the 10-step queen radius, no target was found, all drones report empty coordinates, and the hive waits for GS to assign a new search location.

---

## What will be checked

- Correct use of inheritance and virtual dispatch for `Attack()` (and any other behavior that differs per drone type).
- Sensible ownership model using smart pointers (who owns whom, and why you picked the pointer type you did).
- Encapsulation: subordinate drones cannot be armed/disarmed/moved except through their `BeeQueen`.
- Correct handling of all status/edge cases above (non-operational drone actions, unarmed queen, out-of-bombs, etc.).
- Your justification for the open design points (hive-type assignment strategy, coordinate representation, free-hunt search pattern/exit condition).
