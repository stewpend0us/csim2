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

### restructureInterface

1. ~~put all of the data in struct StrictlyProperBlock on it's own struct that can be passed into the functions rather than having the huge parameter lists~~
2. ~~install dbg.h macros in the solvers and blocks to make them more user friendly~~
3. ~~pass `output` as an input to physics function~~ Not going to do this. In rk4, for example, it would force a call to the output function every partial step wheather the result is used or not. You'll get the exact same perormance if you just call the output function from the physics function if you need it but the people who don't need it won't get the (probably negligable) performance hit.
4. add a `void utility(time, input, state, output)` function to all blocks and call it from the solvers

### tests

* write "is valid block" test and run it on all block types
* write solver specific tests to verify that they...work? aka have some basic standard behaviors

### solvers

* think about the solver architecture...is there a better way to organize things so there is less duplication and so it's easier to implement new types of solvers? (solver, solver_controller, solver_realtime etc.)
* ~~each solver type (euler, rk4) should have a "take single step" function to simplify (or hopefully generalize) the creation of "custom" solver types (real-time, controller, known input)~~
* implement a "real time" solver
* implement a "linearize" function
* implement a "equilibrium" solver

### blocks

* implement `reducedSystem` block (so you can quickly zero out states and debug blocks)
* implement a `stateSpace` block
* implement a parallel `blockSystem`

### examples

* write `mass_spring_damper_demo` to show a simple end to end use of csim2

### visual studio

* get visual studio projects setup (consider using visual studio code instead)
