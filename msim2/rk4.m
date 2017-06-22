function [output, state, dstate] = rk4(...
block,...
dt,...
time,...
Xi,...
input,...
input2
)
numSteps = numel(time);

output = zeros(numSteps,block.numOutputs);
state = zeros(numSteps,block.numStates);
dstate = zeros(numSteps,block.numStates);

state(1,:) = Xi;
for i = 1:(numSteps-1)
		[output(i,:), block.storage] = block.h(block.numStates, block.numOutputs, time(i), state(i,:), block.storage);
		[state(i+1,:), dstate(i,:), block.storage] = rk4_f_step(block.numStates, block.numInputs, dt, time(i) state(i,:), input(i,:), input2(i,:), input(i+1,:), block.f, block.storage);
end
i = numSteps;
[output(i,:), block.storage] = block.h(block.numStates, block.numOutputs, time(i), state(i,:), block.storage);
[dstate(i,:), block.storage] = block.f(block.numStates, block.numInputs, time(i), state(i,:), input(i,:), block.storage);
end