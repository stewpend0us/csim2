# csim2
csim2 is a very simple time domain simulation tool. It is designed to enable
implementing dynamic systems as block diagrams (similar to simulink). Individual
blocks can be written and then be composed together to create more complex
systems. One of the objectives is to create a repository of truly general and
re-usable blocks and solvers for composing and running dynamic simulations.

## prerequisites
Before this tool will make sense you'll need to know something about differential
equations. I'll attempt to put it in a nutshell here but you may want to look around
as well. A differential equation is an eqation that is a function of a variable
and its derivatives. The classic (mechanical) example is a mass spring damper. The system
looks like this:

               |----> +x
                _______
    >|----K----|       |
    >|         |   M   |---->F
    >|----C----|_______|
           
*This is supposed to look like a wall on the left connected to a weight on the right with a spring
and a damper.*
Where:
- M is the mass (force due to acceleration = mass * acceleration)
- K is the spring constant (spring force = spring constant * position)
- C is the damping coefficient (damping force = damping coefficient * velocity)
- F is an externally applied force
- +x indicates the positive x direction

Summing the forces results in the following differential equation:

    M*xdotdot + C*xdot + K*x = F

I've used `dot` to indcate a time derivative of `x` so `xdot` is the first derivative of
position (velocity) and `xdotdot` is the seccond derivative of position (acceleration).

In csim2 the differential equation is defined by the *physics* function for a block.
The physics function calculates the state derivative as a function of state, input, and time.
What are the state derivative and state? The unsatisfying answer is: the state
derivative is the derivative of the state. Now we just need to figure out what the state is.

Back to the example. Let's solve our equation for the highest order derivative that shows up:

    xdotdot = (F - C*xdot - K*x)/M

I'm tempted to say that "x and all of its derivatives that show up on the right hand side are the state"
which is true in this case but they don't actually have to show up to be a part of the state. If C were
zero you could cross C\*xdot out of the equation but xdot would still need to be a part of the state. Another way
to say it might be "x and all of its derivatives less than the highest order derivative are the state".
This one is better and it would always work but it's not always nececary to keep x *and* all of its derivatives.
If K were zero and you weren't interested in knowing the position of the mass you don't have to keep x as part of the state.

In our example x and xdot show up so our system has two states (and two state derivatives). This could be
implemented in a physics function something like this:

    // deconstruct the state vector into each state
    double x = state[0];
    double xdot = state[1];
    // calculate the derivative of each state
    dstate[0] = xdot; // dx = xdot
    dstate[1] = (F - C*xdot - K*x)/M; // dxdot = xdotdot = (F - C*xdot - K*x)/M

I this example the first state derivative is trivial. x and xdot are provided in the state vector.
That means our state derivative will be dx (derivative of x) and dxdot (derivative of xdot). The 
derivative of x is just xdot and the derivative of xdot is the acceleration we solved for above.

One thing that helps me distinguish between the state derivative and the state is I always put the
word `dot` after the variable when I'm talking about the state and I always put the letter `d` before
the variable when I'm talking about the state derivative. It can be tempting to name both the state
and the state derivative `xdot` for example but this gets confusing quickly.

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

This is what a block really looks like:  

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

## strictly proper blocks
csim2 is __designed to only work with strictly proper blocks__. Strictly proper blocks look
the same as general blocks but the relatonship between inputs and outputs is different.

    dx = f(t,x,u)
    y  = h(t,x)

There is no direct connection between the outputs and inputs. This compromise is made to
drastically simplify composing blocks together (I'll talk about why this works more in 
the section on composing blocks).

## solvers `csim2/solvers.c`
Solvers only ever deal with an individual block (although the block may be composed of
several other blocks). Their job is to integrate the state derivative and produce the next state.
This is what the block and solver look like:

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

Here the laplace domain integrator `1/s` is used to represent the solver. The inputs to
the standard solver functions are:
- block to be solved
- time vector
- initial conditions
- input vector

The return values are:
- block output
- block state *(optional)*
- block dstate *(optional)*

## controller solvers
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
           '----|o         f|<---'
                |  control  |
    command---->|c          |
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
struct (`csim2/StrictlyProperBlock.h`).

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
own block so that it can be solved (remember the solvers only operate on one block).

## blockSystem `csim2/blockSystem.c`
The `blockSystem` block enables the composition of other blocks. Unlike implementing a new
block the `blockSystem` block is already implemented. Instead you have to construct a
`blockSystemStorage` struct and use that to construct a `blockSystem` block.

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

Here's a diagram to hopefully clear this up:

                 ________________________________________
                |           containing block             |
                |      ___________                       |
                |     | contained |      ____________    |
                |     |  blocks   |     | calcSystem |   |      system
                |  .->|u         y|--.--|   Output   |-->|----> output
                |  |  |___________|  |  |____________|   |
                |  |   ___________   |                   |
                |  |  | calcBlock |  |                   |
    system      |  '--|  Inputs   |<-'                   |
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

## file structure:
- csim2/
  - contains the most important bits
- cdemo/
  - contains example code and tests
- mex*/
  - various matlab mex file functions enable using matlab as the front end
- mexdemo/
  - a .dll intended to be used with matlab and the mex functions
- msim2/
  - a matlab implementation of csim2/ mainly for testing/prototyping
  
