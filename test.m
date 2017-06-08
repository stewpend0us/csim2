
DebugOrRelease = 'Release';
path = fullfile('x64',DebugOrRelease,'mex');
euler = 'mexeuler.mexw64';
rk4 = 'mexrk4.mexw64';
dllpath = fullfile('x64',DebugOrRelease,'demo.dll');
options = '3';
dt = .5;
t = 10:dt:30;

copyfile(fullfile(path,euler), cd);
copyfile(fullfile(path,rk4), cd);

[y1,x1,dx1]=  mexrk4(dllpath,'FirstOrderLag1',dt,t,0,ones(1,numel(t)),ones(1,numel(t)),options);
[y2,x2,dx2]=  mexrk4(dllpath,'FirstOrderLag2',dt,t,0,ones(1,numel(t)),ones(1,numel(t)),options);
[y3,x3,dx3]=mexeuler(dllpath,'FirstOrderLag1',dt,t,0,ones(1,numel(t)),options);
[y4,x4,dx4]=mexeuler(dllpath,'FirstOrderLag2',dt,t,0,ones(1,numel(t)),options);
plot(t,y1,'s',t,y2,'o',t,y3,'x',t,y4,'+');
grid on;

delete(euler);
delete(rk4);

clear mex;