function method = getExplicitButcherTableaus()
% Reference:
% https://en.wikipedia.org/wiki/List_of_Runge%E2%80%93Kutta_methods#3.2F8-rule_fourth-order_method

method{1}{1} = constructExplicitButcherTableau('Forward Euler', 1, 0, 1);
method{2}{1} = getExplicitButcherTableau2ndOrder(1/2, 'Midpoint');
method{2}{2} = getExplicitButcherTableau2ndOrder(1, 'Heun''s');
method{2}{3} = getExplicitButcherTableau2ndOrder(2/3, 'Ralston''s');
method{3}{1} = constructExplicitButcherTableau('Kutta''s third-order', 3, ...
    [
    0	0	0
    1/2	0	0
    -1	2	0
    ],...
    [
    1/6	2/3	1/6
    ]);
method{4}{1} = constructExplicitButcherTableau('RK4 classic', 4, ...
    [
    0	0	0	0
    1/2	0	0	0
    0	1/2	0	0
    0	0	1	0
    ],...
    [
    1/6	1/3	1/3	1/6
    ]);
method{4}{2} = constructExplicitButcherTableau('RK4 3/8-rule', 4, ...
    [
    0	0	0	0
    1/3	0	0	0
   -1/3	1	0	0
    1	-1	1	0
    ],...
    [
    1/8	3/8	3/8	1/8
    ]);
end