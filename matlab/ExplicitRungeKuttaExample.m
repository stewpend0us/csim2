function [t, results, names] = ExplicitRungeKuttaExample(wn, dt, tfinal, X0)

%sample problem 1/(tau*s + 1)
    function dX = physics(t, X)
        zeta = .1;
        x = X(1);
        v = X(2);
        a = X(3);
        dx = v;
        dv = a;
        da = wn*wn*(1 - v*2*zeta/wn - x - a);
        dX = [dx dv da];
    end

[t, results, names] = CompareAllRungeKuttaMethods(@physics, dt, tfinal, X0);

if ~nargout
    clear t results names
end
end