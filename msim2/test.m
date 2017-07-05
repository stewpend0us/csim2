function test
debug = false;
c = 0;

c = c+1;
run(c).dt = .1;
run(c).t = (0:run(c).dt:1000)';
run(c).Xi = [0 0];
run(c).input = ones(size(run(c).t));
run(c).input2 = ones(size(run(c).t));

storage = struct('zeta',1,'omega_n',1);
run(c).mobj = secondOrderSystem(storage);
run(c).c_options = sprintf('%i,%i',storage.zeta,storage.omega_n);
run(c).c_name = 'SecondOrderSystem1';

c = c+1;
run(c).dt = .001;
run(c).t = (5:run(c).dt:50)';
run(c).Xi = [.25 .8];
rng(2345544)
run(c).input = rand(size(run(c).t));
run(c).input2 = rand(size(run(c).t));

storage = struct('zeta',.5,'omega_n',2.3);
run(c).mobj = secondOrderSystem(storage);
run(c).c_options = sprintf('%f,%f',storage.zeta,storage.omega_n);
run(c).c_name = 'SecondOrderSystem1';

c = c+1;
run(c).dt = .001;
run(c).t = (5:run(c).dt:50)';
run(c).Xi = [500];
rng(756373)
run(c).input = rand(size(run(c).t));
run(c).input2 = rand(size(run(c).t));

storage = 3.2;
run(c).mobj = firstOrderLag(storage);
run(c).c_options = sprintf('%f',storage);
run(c).c_name = 'FirstOrderLag1';

c = c+1;
run(c) = run(c-1);
run(c).c_name = 'FirstOrderLag2';

c = c+1;
run(c).dt = .001;
run(c).t = (5:run(c).dt:50)';
numStates = 5;
rng(543345)
run(c).Xi = rand([1,numStates]);
run(c).input = rand([numel(run(c).t),numStates]);
run(c).input2 = rand([numel(run(c).t),numStates]);

run(c).mobj = integrator(numStates);
run(c).c_options = sprintf('%f',numStates);
run(c).c_name = 'Int1';

for i = 1:numel(run)
    fprintf('test %i',i);
    try
        e_time = compare_euler(debug,run(i).c_name,run(i).c_options,run(i).mobj,run(i).dt,run(i).t,run(i).Xi,run(i).input);
        rk_time = compare_rk4(debug,run(i).c_name,run(i).c_options,run(i).mobj,run(i).dt,run(i).t,run(i).Xi,run(i).input,run(i).input2);
    catch me
        fprintf(': FAIL (%s)\n', me.message);
        continue;
    end
    fprintf(': euler -> %8.2f rk4 -> %8.2f\n', e_time, rk_time);
end
end