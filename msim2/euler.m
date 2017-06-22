function [output, state, dstate] = euler(...
block,...
dt,...
time,...
Xi,...
input
)
numSteps = numel(time);

output = zeros(numSteps,block.numOutputs);
state = zeros(numSteps,block.numStates);
dstate = state;

state(1,:) = Xi;
for i = 1:(numSteps-1)
		[output(i,:), block.storage] = block.h(block.numStates, block.numOutputs, time(i), state(i,:), block.storage);
		[state(i+1,:), dstate(i,:), block.storage] = euler_f_step(block.numStates, block.numInputs, dt, time(i) state(i,:), input(i,:), block.f, block.storage);
end
i = numSteps;
[output(i,:), block.storage] = block.h(block.numStates, block.numOutputs, time(i), state(i,:), block.storage);
[dstate(i,:), block.storage] = block.f(block.numStates, block.numInputs, time(i), state(i,:), input(i,:), block.storage);
end