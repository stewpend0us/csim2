function rk = getExplicitRungeKutta(f, h, bt)
% f  differential equation function handle dy/dt = f(t,y)
% h  step size h>0
% bt is an Explicit Butcher tableau struct (see constructExplicitButcherTableau)
%
% References:
% https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods#Explicit_Runge.E2.80.93Kutta_methods

assert(isa(f,'function_handle'), 'f must be a function handel dy/dt = f(t,y)');
assert(h>0, 'h must be greater than zero');
%assume bt is valid (that it came from constructExplicitButcherTableau)

a = bt.a;
b = bt.b;
c = bt.c;
s = bt.s;

    function y2 = generalRungeKutta(t1, y1)
        % t1 current time
        % y1 current state
        % y2 next state
        
        k = zeros(s,numel(y1));
        bk = 0;
        
        for i = 1:s
            yn = y1;
            tn = t1 + c(i)*h;
            for j = 1:(i-1)
                %note this code isn't hit when i=1
                yn = yn + h*a(i,j)*k(j,:);
            end
            k(i,:) = f(tn,yn);
            bk = bk + b(i)*k(i,:);
        end
        y2 = y1 + h*bk;
    end

rk = @generalRungeKutta;
end