# csim2
csim2 is a very simple time domain simulation tool. It is architected to enable
implementing dynamic systems as block diagrams (similar to simulink). Individual
blocks can be written and then be composed together to create more complex
systems. One of the objectives is to create a repository of truly general and
re-usable blocks for composing dynamic simulations.

## blocks
blocks are the part of block diagrams that contain all of the dynamics. Connections
between blocks define where the inputs to a block come from and where the outputs go.
Here is a simple block diagram:
`
            ______       ______  
           |      |     |      |  
input ---->|u B1 y|---->|u B2 y|----> output  
           |______|     |______|  
`


Organization:
- csim2/
  - contains the most important bits
- mex*/
  - various matlab mex file functions enable using matlab as the front end
- demo/
  - .dll file intended to beused with matlab and the mex functions
- msim2/
  - a matlab implementation of csim2/ mainly for testing/prototyping
  
Currently implemented blocks:
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

The primary interface that blocks must implement in csim2/StrictlyProperBlock.c
