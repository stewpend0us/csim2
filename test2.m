function test2(dt,t,dllpath,options)

arg1 = {dllpath,'SecondOrderSystem1',dt,t,[0 0],ones(1,numel(t)),ones(1,numel(t)),options};
arg2 = {dllpath,'SecondOrderSystem1',dt,t,[0 0],ones(1,numel(t)),options};

nlhsCheck(arg1,arg2);

end
