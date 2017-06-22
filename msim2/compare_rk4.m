function time_diff = compare_rk4( debug, obj_name, c_options, m_object, dt, t, Xi, input, input2 )
[unload, demopath] = loadc(debug);
time_diff = NaN;
try
    tic;
    [m_output, m_state, m_dstate] = rk4(m_object,dt,t,Xi,input,input2);
    m_time = toc;
    
    [c_output] = mexrk4(demopath,obj_name,dt,t,Xi,input',input2', c_options);
    c_output = c_output';
    assert(all(m_output(:)==c_output(:)), 'failed one arg');
    
    [c_output, c_state] = mexrk4(demopath,obj_name,dt,t,Xi,input',input2', c_options);
    c_output = c_output';
    c_state = c_state';
    assert(all(m_output(:)==c_output(:)) && all(m_state(:)==c_state(:)), 'failed two arg');
    
    tic;
    [c_output, c_state, c_dstate] = mexrk4(demopath,obj_name,dt,t,Xi,input',input2', c_options);
    c_time = toc;
    c_output = c_output';
    c_state = c_state';
    c_dstate = c_dstate';
    assert(all(m_output(:)==c_output(:)) && all(m_state(:)==c_state(:)) && all(m_dstate(:)==c_dstate(:)), 'failed three arg');
    
    time_diff = m_time / c_time;
catch me
    unload();
    rethrow(me);
end
unload();
end

