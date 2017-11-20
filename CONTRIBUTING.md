# How to contribute

Honestly I'm not expecting much interest in this...but it'd be awesome if there were!
So here's me being optimistic:

## Project organization

* Branch `master` is always stable and release-ready.
* Branch `develop` is for development and merged into `master` when stable.

## Contributing

1. Fork the repo
2. Make sure you are synced with the latest changes on `develop`.
3. Program
4. **Rebase on `develop` branch and resolve any conflicts _before submitting a pull request!_**
5. Submit a pull request to the `develop` branch.

## TODO

* write "is valid block" test and run it on all block types
* write solver specific tests to verify that they...work? aka have some basic standard behaviors
* each solver type (euler, rk4) should have a "take single step" function to simplify (or hopefully generalize) the creation of "custom" solver types (real-time, controller, known input)
* put all of the data in struct StrictlyProperBlock on it's own struct that can be passed into the functions rather than having the huge parameter lists
* add a "void util(time, input, output, state??)" function to all blocks
* install dbg.h macros in the solvers (and blocks?) to make them more user friendly
* get visual studio projects setup (consider using visual studio code instead)
* pass `output` as an input to physics function
* add a few more comments
* (maybe) make a simple array type and convert everything to use them
* implement `reducedSystem` block (so you can quickly zero out states and debug blocks)
* write `mass_spring_damper_demo` to show a simple end to end use of csim2
* implement a "real time" solver
* think about the solver architecture...is there a better way to organize things so there is less duplication and so it's easier to implement new types of solvers? (solver, solver_controller, solver_realtime etc.)
* implement a "linearize" function
* implement a "state space" block
* implement a "equilibrium" solver
