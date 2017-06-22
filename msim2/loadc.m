function [f, demopath] = loadc(debug)
if debug
    DebugOrRelease = 'Debug';
else
    DebugOrRelease = 'Release';
end

euler = 'mexeuler.mexw64';
rk4 = 'mexrk4.mexw64';

%clean up incase they didn't call unload like they should have
clear mex;
if exist(euler,'file')
    delete(euler);
end
if exist(rk4,'file')
    delete(rk4);
end

%move the most recent files to the local directory
mex_path = fullfile('..','x64',DebugOrRelease,'mex');
copyfile(fullfile(mex_path,euler), cd);
copyfile(fullfile(mex_path,rk4), cd);

%provide a clean up function for them to use
    function unload()
        delete(euler);
        delete(rk4);
        clear mex;
    end

demopath = fullfile('..','x64',DebugOrRelease,'demo.dll');
f = @unload;
end