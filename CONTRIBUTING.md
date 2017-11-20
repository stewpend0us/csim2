# How to contribute

Honestly I'm not expecting much interest in this...but it'd be awesome if there were!
So here's me being optimistic:

## Project organization

* Branch `master` is always stable and release-ready.
* Branch `develop` is for development and merged into `master` when stable.

## Contributing

1. Fork the repo, or make sure you are synced with the latest changes on `develop`.
2. Program
3. **Rebase on `develop` branch and resolve any conflicts _before submitting a pull request!_**
4. Submit a pull request to the `develop` branch.

## TODO

* write "is valid block" test and run it on all block types
* write solver specific tests to verify that they...work? aka have some basic standard behaviors
* each solver type (euler, rk4) should have a "single step" function to simplify (or hopefully generalize) the creation of "custom" solver types (real-time, controller, known input)
* put all of the data in struct StrictlyProperBlock on it's own struct that can be passed into the functions rather than having the huge parameter lists
* add a "void util(time, input, output, state??)" function to all blocks
* install dbg.h macros in the solvers (and blocks?) to make them more user friendly
* write some "real" examples
* get visual studio projects setup (consider using visual studio code instead)

* add a few more comments