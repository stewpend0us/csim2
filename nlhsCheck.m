function nlhsCheck(arg1, arg2)

ti = 'one arg';
tic;
y11=  mexrk4(arg1{:});
toc;
tic;
y21=  mexrk4(arg1{:});
toc;
tic;
y31=mexeuler(arg2{:});
toc;
tic;
y41=mexeuler(arg2{:});
toc;
%makePlot(t,y11,y21,y31,y41,ti);
assert(all(y11(:)==y21(:)));
assert(all(y31(:)==y41(:)));

ti = 'two arg';
tic;
[y12,x12]=  mexrk4(arg1{:});
toc;
tic;
[y22,x22]=  mexrk4(arg1{:});
toc;
tic;
[y32,x32]=mexeuler(arg2{:});
toc;
tic;
[y42,x42]=mexeuler(arg2{:});
toc;
%makePlot(t,y11,y21,y31,y41,ti);
assert(all(y12(:)==y22(:)));
assert(all(x12(:)==x22(:)));
assert(all(y32(:)==y42(:)));
assert(all(x32(:)==x42(:)));

ti = 'three arg';
tic;
[y13,x13,dx13]=  mexrk4(arg1{:});
toc;
tic;
[y23,x23,dx23]=  mexrk4(arg1{:});
toc;
tic;
[y33,x33,dx33]=mexeuler(arg2{:});
toc;
tic;
[y43,x43,dx43]=mexeuler(arg2{:});
toc;

makePlot(arg1{4},y11,y21,y31,y41,ti);
assert(all(y13(:)==y23(:)));
assert(all(x13(:)==x23(:)));
assert(all(dx13(:)==dx23(:)));
assert(all(y33(:)==y43(:)));
assert(all(x33(:)==x43(:)));
assert(all(dx33(:)==dx43(:)));

assert(all(y11(:)==y12(:)));
assert(all(y31(:)==y32(:)));
assert(all(y12(:)==y13(:)));
assert(all(y32(:)==y33(:)));

assert(all(x12(:)==x13(:)));
assert(all(x32(:)==x33(:)));
end

function makePlot(t, y1, y2, y3, y4, ti)
figure;
plot(t,y1,'s',t,y2,'o',t,y3,'x',t,y4,'+');
grid on;
title(ti);
end