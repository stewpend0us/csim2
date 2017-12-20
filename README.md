# csim2
csim2 is a very simple time domain simulation tool. It is designed to enable
implementing dynamic systems as block diagrams (similar to simulink). Individual
blocks can be written and then be composed together to create more complex
systems. One of the objectives is to create a repository of truly general and
re-usable blocks and solvers for composing and running dynamic simulations.

## prerequisites
Before this tool will make sense you'll need to know something about differential
equations. I'll attempt to put it in a nutshell here but you may want to study up
yourself. If you are comfortable with differential equations and/or state space you
can probably skip this section.

A differential equation is an eqation that is a function of a variable and its
derivatives. The classic (mechanical) example is a mass spring damper.
The system looks like this:

               |----> +x
                _______
    >|----K----|       |
    >|         |   M   |---->F
    >|----C----|_______|
           
*This is supposed to look like a wall on the left connected to a weight on the right with
a spring and a damper.*

Where:
- M is the mass (force due to acceleration = mass * acceleration)
- K is the spring constant (spring force = spring constant * position)
- C is the damping coefficient (damping force = damping coefficient * velocity)
- F is an externally applied force
- +x indicates the positive x direction

Summing the forces results in the following differential equation:

    M*xdotdot + C*xdot + K*x = F

I've used `dot` to indcate a time derivative so `xdot` is the first derivative of position
dx/dt (velocity) and `xdotdot` is the seccond derivative of position d^2x/dt^2 (acceleration).

In csim2 the differential equation is defined by the *physics* function for a block.
The physics function calculates the state derivative as a function of state, input, and time.

What are the state derivative and state? Start by identifying the highest and lowest
order derivatives of x that appear in the differential equation. In our case the highest
is `xdotdot` and the lowest is `x`. That means that `xdotdot` is definitely a state derivative
and `x` is definitely a state. Everything in-between are both state
derivatives *and* states. In our case that's `xdotdot`. 

      state
    derivative        state
    [ xdotdot ]  ->  [ xdot ]
    [ xdot    ]  ->  [ x    ]

If it was confusing before hopefully now it's obvious why the state derivative is called the
state derivative. It's simply the derivative of the state!

Notice how `xdot` appears in both the state and state derivative. This isn't a problem but
calling them the same thing can get messy. I fix this by prefixing the state derivative with
a `d` instead of a suffix of `dot`. They have the same meaning but it allows me to easily
identify the state derivative versus the state.

     state
    derivative        state
    [ dxdot   ]  ->  [ xdot ]
    [ dx      ]  ->  [ x    ]

Now back to the example. In the physics function we need to calculate the state derivative.
So let solve our differential equation for the highest order derivative of `x`:

    xdotdot = (F - C*xdot - K*x)/M

All that's left is to implement the physics function. It will look something like this:

    // deconstruct the state vector into each state
    double xdot = state[0];
	double x =	  state[1];
	// deconstruct the state derivative vector into each state derivative
	double * dxdot = &dstate[0];
	double * dx =	 &dstate[1];
    // calculate each state derivative
    *dx = xdot; // this is why I use the naming convention otherwise both variables would have the same name
    *dxdot = (F - C*xdot - K*x) / M;

See `examples/mass_spring_damper_example.c` for a working demo.

## blocks
Blocks are the part of block diagrams that contain all of the dynamics. Connections
between blocks define where the inputs to a block come from and where the outputs go.
This is a simplified, trivial, block diagram:  

                ______       ______   
               |  B1  |     |  B2  |   
    input ---->|u    y|---->|u    y|----> output   
               |______|     |______|   

Here the input signal goes into block B1. Inside B1 it's processed into the output y
which is then passed into the input of block B2. The final output signal is just the
output of block B2.

This is a more complete representation of a block:  

          ___________
         |           |
    ---->|x        dx|---->
         |           |
    ---->|u         y|---->
         |___________|

Where __x__ and __dx__ are the block __state__ and __state derviative__, __u__ is the
block __input__, and __y__ is the block __output__. In the general case:

    dx = f(t,x,u)
    y  = h(t,x,u)

## strictly proper blocks `StrictlyProperBlock.h`
csim2 is __designed to only work with strictly proper blocks__. Strictly proper blocks look
the same as general blocks but the relatonship between inputs and outputs is different.

    dx = f(t,x,u)
    y  = h(t,x)

There is no direct connection between the outputs and inputs. This compromise is made to
drastically simplify composing blocks together (I'll talk about why this works more in 
the section on composing blocks). Typically *real* systems don't have a direct connection
between inputs and outputs anyway so this isn't a huge sacrafice. When a system does have
a direct feedthrough relationship it can usually be handled by making the connection by 
wrapping the block in a `blockSystem`.

## solvers
Solvers only ever deal with a single block (although the block may be composed of several
other blocks by using a `blockSystem`). Their job is to integrate the state derivative and
produce the next state. This is what the block and solver look like:

                 _______
                |       |
                |   1   |
         .------|  ---  |<-----.
         |      |   s   |      |
         |      |_______|      |
         |     ___________     |
         |    |           |    |
         '--->|x        dx|----'
              |           |
    input---->|u         y|---->output
              |___________|

Here the laplace domain integrator `1/s` is used to represent the solver. Currently there
are two solvers: `euler` and `rk4`.

## solvers with controller
Controller solvers do the same thing as standard solvers but instead of specifying the
block input directly they are specified via a controller function and command. The block
diagram looks like this:

                   _______
                  |       |
                  |   1   |
           .------|  ---  |<-----.
           |      |   s   |      |
           |      |_______|      |
           |     ___________     |
           |    |           |    |
           '--->|x        dx|----'
                |           |
           .--->|u         y|----.
           |    |___________|    |
          \      ___________      \
           |    |           |    |
           '----|c         f|<---'
                |  control  |
    command---->|i          |
                |___________|
              
In this case the solver functions also return the output of the controller.

The kink in the connections from y to f and o to u represent switches. That means
the signals are sampled rather than "continuous". This is convenient because the
controller is typically implemented on a digital computer. Right now the controller
function is called once per time step. So the time step used for the simulation
should be less than or equal to the rate that the controller code is expected to run.
If the time step is less you will have to manually check whether the controller
function should be run or not and then call it accordingly.

## implementing blocks 
Blocks are implemented by writing a function that returns a `StrictlyProperBlock`
struct (`StrictlyProperBlock.h`).

The struct contains the properties:
- `numStates` number of States (size of the state and dstate array)
- `numInputs` number of Inputs (size of the input array)
- `numOutputs` number of Outputs (size of the output array)
- `f` physics function pointer
  - Note that the physics function should be *pure*
- `h` output function pointer
  - Note that the output function should be *pure*
- `storage` void pointer can be used for storing anything needed in the physics/output functions
  - Note that the storage values should remain constant. This isn't required but good practice.

The inputs to the physics function are:
- `numStates` same as above
- `numInputs` same as above
- `dState` state derivative array *(output)*
- `time` current time value
- `state` current state array
- `input` current input array
- `storage` same as above

The physics function updates the state derivative array (`dState`) as a function of the other inputs.
This is the __differential equation__ that represents whatever we are trying to simulate. if you are
familiar with __state space__ it represents the A and B matrices.

The inputs to the output function are:
- `numStates` same as above
- `numOutputs` same as above
- `output` output array *(output)*
- `time` current time value
- `state` current state array
- `storage` same as above

The output function defines the output of our system. If you are familiar with __state space__
it represents the C matrix (there is no D matrix for strictly proper systems).

## composing blocks
Blocks can be composed together to form one larger block. Here's a simple example:

                 _________________________
                |     ____       ____     |
    system      |    |    |     |    |    |      system
     input ---->|--->| B1 |---->| B2 |--->|----> output
                |    |____|     |____|    |
                |_________________________|

Here we've taken the trivial example from above and enclosed it in it's
own block so that it can be solved (remember the solvers only operate on a single block).
To achieve this using csim2 you would construct a `blockSystem`

## blockSystem `blockSystem.c`
The `blockSystem` block enables the composition of other blocks. Unlike implementing a new
block the `blockSystem` block is already implemented. Instead you have to construct a
`blockSystemStorage` struct which contains functions for calculating the input to all of the
contained block and for calculating the output of the entire block system.

Here's a more complete diagram:

                 ________________________________________
                |           containing block             |
                |      ___________                       |
                |     | contained |      ____________    |
                |     |  blocks   |     | calcSystem |   |      system
                |  .->|u         y|--.--|   Output   |-->|----> output
                |  |  |___________|  |  |____________|   |
                |  |   ___________   |                   |
                |  |  | calcBlock |  |                   |
     system     |  '--|  Inputs   |<-'                   |
     input ---->|---->|___________|                      |
                |________________________________________|

This is where the distinction between general blocks and strictly proper blocks comes in.
If we were using general blocks and the output were a function of state and input `y = h(t,x,u)`
then there would be an algebraic loop between the contained blocks and the calcBlockInputs function.
Say we need to calculate the output of the block. First we will need the state which we have and the input
which we don't have. So we'd need to follow the signal back to the calcBlockInputs function which requires
the block output as input. Of cource the block output is what we were after in the first place so that's
not going to work without getting fancy. Thankfully we're using strictly proper blocks `y = h(t,x)` so 
the output of the block is guaranteed to be available and this problem goes away.

The blockSystemStorage struct contains:
- `numBlocks` number of blocks the blockSystem contains
- `blocks` array of strictlyProperBlock structs
- `blockInputs` array of input arrays (one for each block)
- `blockOutputs` array of output arrays (one for each block)
- `calcBlockInputs` function pointer for calculating the individual block inputs
  - Note that the calcBlockInputs function should be *pure*
- `calcSystemOutput` function pointer for calculating the blockSystem output
  - Note that the calcSystemOutput function should be *pure*
- `systemStorage` void pointer can be used for storing anything needed in the calcBlockInputs/calcSystemOutput functions
  - Note that the storage values should remain constant. This isn't required but good practice.

The inputs to the calcBlockInputs function are:
- `numBlocks` same as above
- `blocks` same as above
- `blockInputs` same as above *(output)*
- `time` current time value
- `blockOutputs` same as above
- `numSystemInputs` number of blockSystem inputs (size of the systemInputs array)
- `systemInputs` current input array
- `systemStorage` same as above

You can think of the calcBlockInputs function as making the connections from the the containing
block inputs and contained blocks outputs to the contained blocks inputs.

The inputs to the calcSystemOutput function are:
- `numSystemOutputs` number of blockSystem outputs (size of they systemOutputs array)
- `systemOutputs` current output array *(output)*
- `time` current time value
- `blockOutputs` same as above
- `systemStorage` same as above

The calcSystemOutput makes the connection from the contained blocks output to the blockSystem
output.