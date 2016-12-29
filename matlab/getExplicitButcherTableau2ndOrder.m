function m = getExplicitButcherTableau2ndOrder(x, name)
% 2nd order Butcher Tableau's are easy to generalize

if ~exist('name','var')
    name = sprintf('2nd order %.3f', x);
end
s = 2;
a = [
    0           0
    x           0
    ];
b = [
    1-1/2/x     1/2/x
    ];

m = constructExplicitButcherTableau(name, s, a, b);
end