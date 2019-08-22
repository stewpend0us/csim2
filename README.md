# csim2
csim2 is a very simple time domain simulation tool. It is designed to enable
implementing dynamic systems as block diagrams (similar to simulink). Individual
blocks can be written and then be composed together to create more complex
systems.

## prerequisite
Before this tool will make sense you'll need to know something about differential
equations. I'll attempt to put it in a nutshell here but you may want to study up
yourself. If you are comfortable with differential equations and/or state space you
can probably skip this section.

A differential equation is just an eqation that is a function of a variable and its
derivatives. The classic (mechanical) example is a mass spring damper. Int this case the
variable is position and its derivatives are velocity and acceleration. The system looks
like this:

               |----> +x
                _______
    >|----K----|       |
    >|         |   M   |---->F
    >|----C----|_______|
           
*This is supposed to look like a wall on the left (>|) connected to a weight on the right (M)
via a spring (K) and damper (C). An external force (F) is applied to the mass. The positive X
direction is also indicated (+x).*

Where:
- M is the mass (force due to acceleration = mass * acceleration)
- C is the damping coefficient (damping force = damping coefficient * velocity)
- K is the spring constant (spring force = spring constant * position)
- F is an externally applied force
- +x indicates the positive x direction

Summing the forces results in the following differential equation:

    M*xdotdot + C*xdot + K*x = F

I've used `dot` to indcate a time derivative so `xdot` is the first derivative of position
dx/dt (velocity) and `xdotdot` is the seccond derivative of position d^2x/dt^2 (acceleration).

In csim2 the differential equation is defined by the *physics_function* for a block (see src/block.h).
The physics function calculates the state derivative as a function of state, input, and time.

What are the state derivative and state? Start by identifying the highest and lowest
order derivatives of x that appear in the differential equation. In our case the highest
is `xdotdot` and the lowest is `x`. That means that `xdotdot` is definitely a state derivative
and `x` is definitely a state. Everything in-between are both state
derivatives *and* states. In our case that's `xdot`. 

      state
    derivative        state
    [ xdotdot ]  ->  [ xdot ]
    [ xdot    ]  ->  [ x    ]

Clearly the state derivative is called the state derivative because it's simply the derivative
of the state.

Notice how `xdot` appears in both the state and state derivative. This isn't a problem but
calling them the same thing can get messy. I fix this by prefixing the state derivatives with
a `d` instead of a suffix of `dot`. They have the same meaning but it allows me to easily
identify the state derivative versus the state.

     state
    derivative        state
    [ dxdot   ]  ->  [ xdot ]
    [ dx      ]  ->  [ x    ]

Now back to the example. In the physics function we need to calculate the state derivative as a function
of state:

    [ dxdot   ]  =  [ (F - C*xdot - K*x)/M ]
    [ dx      ]  =  [ xdot                 ]

See `examples/mass_spring_damper_example.c` for a working demo of this example.

## blocks
For the purposes of this library a block is basically just the physics function for a given system.
See `src/block.h` for the definition of `physics_function` and `struct block`. The physics function
is the differential equation that converts input and state into state derivative. The library includes
the definition of some basic blocks (see `src/block_basic.c`) but the more interesting blocks will
be the ones that `you` implement. The blocks that represent the physics that you care about.

## solvers
Solvers only ever deal with a single block (although the block may be composed of several
other blocks by using a `block_system`). Their job is to integrate the state derivative and
produce the next state. See `src/block_solver.c`.

## composing blocks
`src/block_system.c` is intended to simplify the composition of multiple blocks into a new block.
Unlike a standard block, block systems do not have a physics function or state (at least not one that you need
to worry about). Instead the state is made up of all of the child block states and you have to implement
an `update_child_inputs_function` (see `src/block_system.h). The update child inputs function is where
the wiring between blocks happens. The following block diagram is implemented in `example/block_system.c`:

              _____________________________________________________
             |                     block system                    |
             |    _______      __________      __________          |
    system   |   |       |    |          |    |          |         |
    input ---|-->| scale |--->| block[0] |--->| block[1] |         |
             |   |_______|    |__________|    |__________|         |
             |                      |               |     _____    |
             |                      |               |    |     |   |
             |                       ---------------)--->|dx[0]|   |
             |                                      |    |     |   |  block system
             |                                       --->|dx[1]|-->|----> dx
             |                                           |dx[2]|   |
             |                                           |_____|   |
             |_____________________________________________________|

*Note: that block[0] has a single input and a single state while block[1] has a single input and two states.
The block system itself has a single input. If you were to inspect the `num_states` and `num_inputs` properties
of the struct block representing this block system you would find that it has 3 states (sum of the number of
child states) and 3 inputs (sum of the number of child inputs plus the number of system inputs). This is an
important detail when it comes time to allocate memory for the solver*

In this example the system input is scaled (multiplied by some constant) before being sent on to block[0].
Then the first (and only) state of block[0] is used as the input into block[1]. Hopefully this is fairly
obvious after looking at the example `update_c_inputs` implementation. The rest of the diagram is 
showing what the block system is doing for you. Essentially each blocks physics function is called
in turn and the results are stacked in the block system output.
