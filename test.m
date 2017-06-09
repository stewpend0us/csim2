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
y11=  mexrk4(dllpath,'FirstOrderLag1',dt,t,0,ones(1,numel(t)),ones(1,numel(t)),options);
y21=  mexrk4(dllpath,'FirstOrderLag2',dt,t,0,ones(1,numel(t)),ones(1,numel(t)),options);
y31=mexeuler(dllpath,'FirstOrderLag1',dt,t,0,ones(1,numel(t)),options);
y41=mexeuler(dllpath,'FirstOrderLag2',dt,t,0,ones(1,numel(t)),options);
%makePlot(t,y11,y21,y31,y41,ti);
assert(all(y11==y21));
assert(all(y31==y41));

ti = 'two arg';
[y12,x12]=  mexrk4(dllpath,'FirstOrderLag1',dt,t,0,ones(1,numel(t)),ones(1,numel(t)),options);
[y22,x22]=  mexrk4(dllpath,'FirstOrderLag2',dt,t,0,ones(1,numel(t)),ones(1,numel(t)),options);
[y32,x32]=mexeuler(dllpath,'FirstOrderLag1',dt,t,0,ones(1,numel(t)),options);
[y42,x42]=mexeuler(dllpath,'FirstOrderLag2',dt,t,0,ones(1,numel(t)),options);
%makePlot(t,y11,y21,y31,y41,ti);
assert(all(y12==y22));
assert(all(x12==x22));
assert(all(y32==y42));
assert(all(x32==x42));

ti = 'three arg';
[y13,x13,dx13]=  mexrk4(dllpath,'FirstOrderLag1',dt,t,0,ones(1,numel(t)),ones(1,numel(t)),options);
[y23,x23,dx23]=  mexrk4(dllpath,'FirstOrderLag2',dt,t,0,ones(1,numel(t)),ones(1,numel(t)),options);
[y33,x33,dx33]=mexeuler(dllpath,'FirstOrderLag1',dt,t,0,ones(1,numel(t)),options);
[y43,x43,dx43]=mexeuler(dllpath,'FirstOrderLag2',dt,t,0,ones(1,numel(t)),options);
makePlot(t,y11,y21,y31,y41,ti);
assert(all(y13==y23));
assert(all(x13==x23));
assert(all(dx13==dx23));
assert(all(y33==y43));
assert(all(x33==x43));
assert(all(dx33==dx43));

assert(all(y11==y12));
assert(all(y31==y32));
assert(all(y12==y13));
assert(all(y32==y33));

assert(all(x12==x13));
assert(all(x32==x33));

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