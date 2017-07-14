# csim2
csim2 is a very simple time domain simulation tool. It is architected to enable
implementing dynamic systems as block diagrams (similar to simulink). Individual
blocks can be written and then be composed together to create more complex
systems. One of the objectives is to create a repository of truly general and
re-usable blocks and solvers for composing and running dynamic simulations.

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
csim2 is designed to only work with strictly proper blocks. Strictly proper blocks look
the same as general blocks but the relatonship between inputs and outputs is different.

    dx = f(t,x,u)
    y  = h(t,x)

There is no direct connection between the outputs and inputs. This compromise is made to
drastically simplify composing blocks together (I'll talk about why this works more in 
the section on composing blocks).

## solvers `csim2/solvers.c`
Solvers only ever deal with an individual block (although the block may be composed of
several other blocks). Their job is to integrate the state derivative. This is what the
block and solver look like:

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
- block state (optional)
- block dstate (optional)

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

## implementing blocks 
Blocks are implemented by writing a function that returns a `StrictlyProperBlock`
struct (`csim2/StrictlyProperBlock.h`). The struct contains the properties:
- `numStates` number of States (size of the state and dstate array)
- `numInputs` number of Inputs (size of the input array)
- `numOutputs` number of Outputs (size of the output array)
- `f` physics function pointer
- `h` output function pointer
- `storage` void pointer can be used for storing anything needed in the physics/output functions
  - Note that the storage values should remain constant. This isn't required but good practice.

The inputs to the physics function are:
- `numStates` same as above
- `numInputs` same as above
- `dState` state derivative array *output*
- `time` current time value
- `state` current state array
- `input` current input array
- `storage` same as above

The physics function updates the state derivative array (`dState`) as a function of the other inputs.
This is the __differential equation__ that represents whatever we are trying to simulate.

The inputs to the output function are:
- `numStates` same as above
- `numOutputs` same as above
- `output` output array *output*
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

In this example we've taken the trivial example from before and enclosed it in it's
own block so that it can be solved (remember the solvers only operate on one block).

## blockSystem `csim2/blockSystem.c`
The `blockSystem` block enables this composition. 



### Currently implemented blocks:
- csim2/integrator.c
  - 1/s
- csim2/firstOrderLag.c
  - 1/(tau*s + 1)
- csim2/secondOrderSystem.c
  - omega_n^2/(s^2 + 2\*zeta\*omega_n + omega_n^2)
- csim2/transferFunction.c
  - strictly proper transfer function general case
- csim2/blockSystem.c
  - this is used to compose a collection of other blocks together
  


## file structure:
- csim2/
  - contains the most important bits
- mex*/
  - various matlab mex file functions enable using matlab as the front end
- demo/
  - a .dll intended to beused with matlab and the mex functions
- msim2/
  - a matlab implementation of csim2/ mainly for testing/prototyping
  
