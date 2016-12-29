function bt = constructExplicitButcherTableau(name, s, a, b)
% s Runge-Kutta order, s>=1
% a Runge-Kutta matrix, size s by s
% b Runge-Kutta weights, size s, sum(b)==1
%
% Explicit Butcher tableau:
% 0   |   0   0   0   0   0   0      0
% c2  |   a21 0   0   0   0   0      0
% c3  |   a31 a32 0   0   0   0      0
% .   |   .   .   .   0   0   0      0
% .   |   .   .   .   .   0   0      0
% .   |   .   .   .   .   .   0      0
% cs  |   as1 as2 .   .   .   as,s-1 0
% ----------------------------------
%     |   b1  b2  .   .   .   .      bs
%
% References:
% https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods#Explicit_Runge.E2.80.93Kutta_methods
% https://en.wikipedia.org/wiki/List_of_Runge%E2%80%93Kutta_methods

assert(ischar(name), 'name must be a string');
assert(mod(s,1)==0, 's must be an integer');
assert(s>=1, 's must be greater than or equal to 1');
assert(size(a,1)==s, 'a must have s rows');
assert(size(a,2)==s, 'a must have s columns');
assert(~any(any(triu(a))), 'a must be lower triangular with zeros on the diagonal');
assert(numel(b)==s, 'b must have s elements');
assert(abs(sum(b)-1)<10*eps, 'the sum of the elements in b must equal 1');

bt.name = name;
bt.s = s;
bt.a = a;
bt.b = b;
bt.c = sum(a,2); %calculate Runge-Kutta nodes = sum of the rows of a
end