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
for i = 1:(numel(time)-1)
		[output(i,:), block.storage] = block.h(time(i), state(i,:), block.storage);
		[state(i+1,:), dstate(i,:), block.storage] = euler_f_step(dt, time(i) state(i,:), input(i,:), block.f, block.storage);
end
[output(i,:), block.storage] = block.h(time(i), state(i,:), block.storage);
[dstate(i,:), block.storage] = block.f(time(i), state(i,:), input(i,:), block.storage);
end