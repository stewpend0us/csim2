function [A, B, C, D] = linearize(block, state, input, t, stateStep, inputStep)

num_states = block.numStates;
num_inputs = block.numInputs;
num_outputs = block.numOutputs;
storage = block.storage;
f = block.f;
h = block.h;

assert(numel(state)==num_states, 'state is the wrong size.');
assert(numel(input)==num_inputs, 'input is the wrong size.');

state = state(:)';
input = input(:)';

if ~exist('stateStep','var')
    stateStep = .001;
end
if ~exist('inputStep','var')
    inputStep = stateStep;
end

stateStep = initStep(stateStep,num_states);
inputStep = initStep(inputStep,num_inputs);

output = h(num_states,num_inputs,t,state,storage);
dstate = f(num_states,num_inputs,t,state,input,storage);

    function [A, C] = perturbState(shift)
        A = zeros(num_states,	num_states);
        C = zeros(num_outputs,	num_states);
        
        for i = 1:num_states
            state_pert    = state;
            state_pert(i) = state_pert(i) + shift(i);
            
            output_pert = h(num_states,num_inputs,t,state_pert,storage);
            dstate_pert = f(num_states,num_inputs,t,state_pert,input,storage);
            
            delta_dx = dstate_pert - dstate;
            delta_y  = output_pert - output;
            
            A(:,i) = delta_dx/shift(i);
            C(:,i) = delta_y/shift(i);
        end
    end

    function [B, D] = perturbInput(shift)
        B = zeros(num_states,   num_inputs);
        D = zeros(num_outputs,	num_inputs);
        
        for i = 1:num_inputs
            input_pert    = input;
            input_pert(i) = input_pert(i) + shift(i);
            
            %output_pert = h(num_states,num_inputs,t,state,storage);
            dstate_pert = f(num_states,num_inputs,t,state,input_pert,storage);
            
            delta_dx = dstate_pert - dstate;
            %delta_y  = output_pert - output;
            
            B(:,i) = delta_dx/shift(i);
            %D(:,i) = delta_y/shift(i);
        end
    end

[A1, C1] = perturbState(stateStep);
[A2, C2] = perturbState(-stateStep);
[B1, D1] = perturbInput(inputStep);
[B2, D2] = perturbInput(-inputStep);

A = (A1 + A2)/2;
B = (B1 + B2)/2;
C = (C1 + C2)/2;
D = (D1 + D2)/2;

end

function s = initStep(s,count)
if isscalar(s)
    s = s * ones(1,count);
    return;
end
assert(numel(s)==count,'step is the wrong size');
s = s(:)';
end