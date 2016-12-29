function test1


    function [dX, Y] = physics(t, X)
        wn = 1;
        zeta = .1;
        x = X(1);
        v = X(2);
        dx = v;
        dv = wn*wn*(1 - v * 2 * zeta / wn - x);
        dX = [dx dv];
        Y=[];
    end

bt = getExplicitButcherTableaus();

RKsolver = getExplicitRungeKutta(@physics, 1, bt{4}{1});




t = (0:1:10)';
xi = [0 0];
f = @(t,x,u,tf) physics(t,x);
%check
[ dstate, output, state, input ] = rk4( f, t', xi, [] );

results = zeros(numel(t),2);
results(1,:) = xi;
%total = 0;
for j = 2:numel(t) %for all time
    %fprintf('%f %f %f\n',t(j-1),results(j-1,1),results(j-1,2));
    results(j,:) = RKsolver(t(j-1), results(j-1,:)); %solve for the next state. function of the current state (and sometimes time)
    %total = total + results(j-1,2);
end
disp([t results state]);
plot(t,results, t, state);
end