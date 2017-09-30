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

* write more tests (especially for blockSystem)
* install dbg.h macros in the solvers (and blocks?) to make them more user friendly
* write some "real" examples
* get visual studio projects setup (consider using visual studio code instead)
* pass "output" as an input to physics function
* add a few more comments
* (maybe) make a simple array type and convert everything to use them