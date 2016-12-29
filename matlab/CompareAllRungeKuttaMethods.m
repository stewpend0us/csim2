function [t, results, names] = CompareAllRungeKuttaMethods(f, dt, tfinal, X0)
methods = getExplicitButcherTableaus();

t = (0:dt:tfinal)';
numstates = numel(X0);

count = 1;
sol = ode45(@(t,x) f(t,x)', [t(1) t(end)], X0);
results{count} = deval(sol,t)';
names{count} = 'matlab ode45';

for order = 1:numel(methods)
    for method = 1:numel(methods{order})
        RKsolver = getExplicitRungeKutta(f, dt, methods{order}{method});
        count = count + 1;
        names{count} = methods{order}{method}.name;
        results{count} = zeros(numel(t),numstates);
        % the following lines show how to use the solver
        results{count}(1,:) = X0; %set the initial state
        for j = 2:numel(t) %for all time
            results{count}(j,:) = RKsolver(t(j-1), results{count}(j-1,:)); %solve for the next state. function of the current state (and sometimes time)
        end
    end
end

for j = 1:numstates
    figure;
    axes;
    grid on;
    hold on;
    title(sprintf('state %i',j));
    for method = 1:numel(results)
        error = results{1}(:,j) - results{method}(:,j);
        leg{method} = sprintf('%20s %10.4f %10.4f',names{method},mean(error),std(error));
        plot(t,results{method}(:,j), '.-');
    end
    hold off;
    h=legend(leg,'Location','southeast');
    h.FontName = 'fixedwidth';
end


if ~nargout
    clear t results names
end
end