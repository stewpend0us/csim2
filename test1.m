function test1(dt,t,dllpath,options)

arg1 = {dllpath,'FirstOrderLag1',dt,t,0,ones(1,numel(t)),ones(1,numel(t)),options};
arg2 = {dllpath,'FirstOrderLag1',dt,t,0,ones(1,numel(t)),options};

nlhsCheck(arg1,arg2);

end