function runTest(test, dt, debug)
if exist('debug','var')
    DebugOrRelease = 'Debug';
else
    DebugOrRelease = 'Release';
end

path = fullfile('x64',DebugOrRelease,'mex');
euler = 'mexeuler.mexw64';
rk4 = 'mexrk4.mexw64';
dllpath = fullfile('x64',DebugOrRelease,'demo.dll');


t = 10:dt:30;

copyfile(fullfile(path,euler), cd);
copyfile(fullfile(path,rk4), cd);

close all;

try
    switch test
        case 1
            options = '3';
            test1(dt,t,dllpath,options);
        case 2
            options = '.1,1';
            test2(dt,t,dllpath,options);
    end
    
catch me
    delete(euler);
    delete(rk4);
    
    clear mex;
    rethrow(me)
end

delete(euler);
delete(rk4);

clear mex;
end