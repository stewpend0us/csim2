function test(dt, debug)
if exist('debug','var')
    DebugOrRelease = 'Debug';
else
    DebugOrRelease = 'Release';
end

path = fullfile('x64',DebugOrRelease,'mex');
euler = 'mexeuler.mexw64';
rk4 = 'mexrk4.mexw64';
dllpath = fullfile('x64',DebugOrRelease,'demo.dll');
options = '3';
t = 10:dt:30;

copyfile(fullfile(path,euler), cd);
copyfile(fullfile(path,rk4), cd);

close all;

ti = 'one arg';
y1=  mexrk4(dllpath,'FirstOrderLag1',dt,t,0,ones(1,numel(t)),ones(1,numel(t)),options);
y2=  mexrk4(dllpath,'FirstOrderLag2',dt,t,0,ones(1,numel(t)),ones(1,numel(t)),options);
y3=mexeuler(dllpath,'FirstOrderLag1',dt,t,0,ones(1,numel(t)),options);
y4=mexeuler(dllpath,'FirstOrderLag2',dt,t,0,ones(1,numel(t)),options);
makePlot(t,y1,y2,y3,y4,ti);
ti = 'two arg';
[y1,x1]=  mexrk4(dllpath,'FirstOrderLag1',dt,t,0,ones(1,numel(t)),ones(1,numel(t)),options);
[y2,x2]=  mexrk4(dllpath,'FirstOrderLag2',dt,t,0,ones(1,numel(t)),ones(1,numel(t)),options);
[y3,x3]=mexeuler(dllpath,'FirstOrderLag1',dt,t,0,ones(1,numel(t)),options);
[y4,x4]=mexeuler(dllpath,'FirstOrderLag2',dt,t,0,ones(1,numel(t)),options);
makePlot(t,y1,y2,y3,y4,ti);
ti = 'three arg';
[y1,x1,dx1]=  mexrk4(dllpath,'FirstOrderLag1',dt,t,0,ones(1,numel(t)),ones(1,numel(t)),options);
[y2,x2,dx2]=  mexrk4(dllpath,'FirstOrderLag2',dt,t,0,ones(1,numel(t)),ones(1,numel(t)),options);
[y3,x3,dx3]=mexeuler(dllpath,'FirstOrderLag1',dt,t,0,ones(1,numel(t)),options);
[y4,x4,dx4]=mexeuler(dllpath,'FirstOrderLag2',dt,t,0,ones(1,numel(t)),options);
makePlot(t,y1,y2,y3,y4,ti);

delete(euler);
delete(rk4);

clear mex;
end

function makePlot(t, y1, y2, y3, y4, ti)
figure;
plot(t,y1,'s',t,y2,'o',t,y3,'x',t,y4,'+');
grid on;
title(ti);
end