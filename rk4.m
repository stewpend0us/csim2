function [ output, input, state, dstate ] = rk4( f, t, Xi, U )
%RK4 Fourth order Runge-Kutta solver
%f -> system function handle [dX, Y] = f(t, X, U)
%t -> time vector (must have fixed time step)
%Xi-> initial state
%U -> input matrix where the first column is time and the remaining columns
%     are input 1:n. The matrix will be interpolated.
%     or
%     a function handle u = U(t)
%     or
%     the empty array []

assert(isfield(f,'numStates'))
assert(isfield(f,'numInputs'));
assert(isfield(f,'numOutputs'));
assert(isfield(f,'physics'));

assert(isa(f.physics,'function_handle'), '''f'' must be a function handle [dX, Y] = f(t, X, U)');
assert(isrow(Xi) && numel(Xi)==f.numStates, '''Xi'' must be a row vector with the correct number of states');
assert(isa(U,'function_handle') || (ismatrix(U) && size(U,2)==(f.numInputs + 1)), '''U'' must be a function handle u = U(t), or an array');
assert(iscolumn(t) && numel(t)>1, '''t'' must be a vector');
difft = diff(t);
assert(std(difft) < 1e-10, '''t'' must have a fixed time step.');

numpts = numel(t);
dt = mean(difft);
dt2 = dt/2;
t2 = t+dt2;

[input, input2] = getInput(U,t,t2);
isnaninput = isnan(input);
isnaninput2 = isnan(input2);
if any(any(isnaninput))
    [a,b] = find(isnaninput,1);
    warning('input %i contains %i NaNs starting at time %g', b, numel(find(isnaninput(:,b))), t(a));
elseif any(any(isnaninput2))
    [a,b] = find(isnaninput2,1);
    warning('input %i contains %i NaNs starting at time %g', b, numel(find(isnaninput2(:,b))), t2(a));
end

state = zeros(numpts, f.numStates);
dstate = state;
output = zeros(numpts, f.numOutputs);
state(1,:) = Xi;

beenWarnedState = false;
for i = 1:numpts-1
    [A, output(i,:)] = ...
        f.physics(t(i),  state(i,:),         input(i,:));
    B = f.physics(t2(i), state(i,:) + A*dt2, input2(i,:));
    C = f.physics(t2(i), state(i,:) + B*dt2, input2(i,:));
    D = f.physics(t(i+1),state(i,:) + C*dt,  input(i+1,:));
    
    state(i+1,:) = state(i,:) + dt*(A + 2*B + 2*C + D)/6;
    dstate(i,:) = A;
    
    if any(isnan(state(i,:))) && ~beenWarnedState
        warning('state contains NaN at time %g', t(i));
        beenWarnedState = true;
    end
end
i = numpts;
[dstate(numpts,:), output(i,:)] = f.physics(t(i), state(i,:), input(i,:));

method = 'decomposeState';
if isfield(f,method)
    state = f.(method)(state);
    dstate = f.(method)(dstate);
end

method = 'decomposeInput';
if isfield(f,method)
    input = f.(method)(input);
end

method = 'decomposeOutput';
if isfield(f,method)
    output = f.(method)(output);
end
end

function [input, input2] = getInput(U,t,t2)
numt = numel(t);

if isa(U,'function_handle')
    try
        input = U(t);
        assert(size(input,1) == size(t,1));
        input2 = U(t2);
    catch e
        warning('%s\n%s','Inital call to U failed. Using slower fallback.',e.message);
        numout = numel(U(t(1)));
        input = zeros(numt,numout);
        input2 = input;
        for i = 1:numt
            input(i,:) = U(t(i));
            input2(i,:) = U(t2(i));
        end
    end
    
    return;
end

if isempty(U)
    input = zeros(numt,1);
    input2 = input;
    return;
end

input  = interp1(U(:,1), U(:,2:end), t, 'linear', 'extrap');
input2 = interp1(U(:,1), U(:,2:end), t2,'linear', 'extrap');
end