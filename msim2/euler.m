function [output, state, dstate] = euler(...
block,...
dt,...
time,...
Xi,...
input...
)
time = time(:);
numSteps = numel(time);
assert(all(size(input) == [numSteps block.numInputs]), 'input wrong size');
assert(numel(Xi) == block.numStates, 'initial conditions wrong size');

output = zeros(numSteps,block.numOutputs);
state = zeros(numSteps,block.numStates);
dstate = state;

state(1,:) = Xi;
for i = 1:(numSteps-1)
		[output(i,:), block.storage] = block.h(block.numStates, block.numOutputs, time(i), state(i,:), block.storage);
		[state(i+1,:), dstate(i,:), block.storage] = euler_f_step(block.numStates, block.numInputs, dt, time(i), state(i,:), input(i,:), block.f, block.storage);
end
i = numSteps;
[output(i,:), block.storage] = block.h(block.numStates, block.numOutputs, time(i), state(i,:), block.storage);
[dstate(i,:), block.storage] = block.f(block.numStates, block.numInputs, time(i), state(i,:), input(i,:), block.storage);
end