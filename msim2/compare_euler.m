function time_diff = compare_euler( debug, obj_name, c_options, m_object, dt, t, Xi, input )
[unload, demopath] = loadc(debug);
time_diff = NaN;
try
    tic;
    [m_output, m_state, m_dstate] = euler(m_object,dt,t,Xi,input);
    m_time = toc;
    
    [c_output] = mexeuler(demopath,obj_name,dt,t,Xi,input', c_options);
    c_output = c_output';
    assert(all(m_output(:)==c_output(:)), 'failed one arg');
    
    [c_output, c_state] = mexeuler(demopath,obj_name,dt,t,Xi,input', c_options);
    c_output = c_output';
    c_state = c_state';
    assert(all(m_output(:)==c_output(:)) && all(m_state(:)==c_state(:)), 'failed two arg');
    
    tic;
    [c_output, c_state, c_dstate] = mexeuler(demopath,obj_name,dt,t,Xi,input', c_options);
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

