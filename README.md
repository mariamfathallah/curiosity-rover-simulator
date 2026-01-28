# Curiosity Rover Simulator (C)

University mini-project (Université Grenoble Alpes).

A simulator of the **Curiosity rover** moving on a 2D terrain and executing a
stack-based robot programming language.  
The project includes an interpreter, automated tests, random terrain generation,
performance evaluation, and runtime verification with an observer.

---

## Features

- Terrain loading and validation (free cells, obstacles, robot start)
- Robot movement and orientation handling
- Stack-based program interpreter (`.prg` language)
- Step-limited **test harness** for functional validation
- **Robustness tests** for invalid programs and error cases
- Random terrain generation and **performance evaluation**
- Runtime verification with an **observer**  
  *(property: a measure must be performed before moving forward)*

---

## Project structure

```text
.
├── src/                     # All C source and header files
├── tests_functional/        # Functional tests (.test)
├── tests_error_cases/       # Expected-failure tests
├── observer_tests/          # Programs for observer verification
├── terrains_valid/          # Valid terrain examples
├── terrains_invalid/        # Invalid terrain examples
├── programs/                # Sample robot programs (.prg)
├── performance_results/     # Example performance outputs (optional)
├── Makefile
├── README.md
└── LICENSE
```

---

## Build

### Requirements
- C compiler (`clang`)
- `make`
- Tested on **MSYS2 UCRT64 (Windows)**

### Compile everything
```bash
make
```
### Clean build files
```bash
make clean
```

## Executables

- `curiosity` — runs a program without step limit

- `curiosity-test` — runs a .test file (step-limited)

- `curiosity-perf` — performance evaluation with random terrains

- `curiosity-obs` — runs the program with runtime verification (observer)


## Validation
### 1️⃣ Functional correctness (recommended sanity check)

Run a functional test (step-limited):
```bash
./curiosity-test tests_functional/test_avancer.test
```

Run the full functional test suite:
```bash
for f in tests_functional/*.test; do
  ./curiosity-test "$f" || exit 1
done
echo "All functional tests passed."
```
---

### 2️⃣ Robustness tests (expected failures)

Some programs are intentionally invalid and must fail:
```bash
./curiosity-test tests_error_cases/testRob_erreur.test
```
Expected result: non-zero exit code.

---

### 3️⃣ Performance evaluation

Runs a robot program on randomly generated terrains with a maximum number of steps:
```bash
./curiosity-perf 1 11 9 0.4 42 programs/prog1.prg 200 out.txt
```

This validates:
- random terrain generation
- bounded execution of autonomous programs
- performance measurements

---

### 4️⃣ Observer mode 

Checks the runtime property:

> **A measure must be performed before any forward move.**

```bash
./curiosity-obs terrains_valid/terrain_11x9.txt programs/prog1.prg
``` 

The observer reports whether the property is respected or violated.

---

## Important note about termination

>The `curiosity` executable does **not** enforce a step limit.

Some autonomous robot programs are intentionally infinite and will not terminate.
This is **expected behavior**.

For bounded execution and validation, always use:

- `curiosity-test`
- `curiosity-perf`
- `curiosity-obs`

## Academic context

This project was developed as part of the INF304 course at
Université Grenoble Alpes and is shared for educational and portfolio purposes.

## License

MIT