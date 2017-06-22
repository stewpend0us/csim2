struct StrictlyProperBlock blockSystem(...
	numSystemInputs,...
	numSystemOutputs,...
    blocks,...
    calcBlockInputs,... [blockInputs, systemStorage] = @( numBlocks, blocks, time, blockOutputs, numSystemInputs, systemInputs, systemStorage )
    calcSystemOutput,... [systemOutputs, systemStorage] = @( numSystemOutputs, time, blockOutputs, systemStorage )
	systemStorage...
)
    storage.numBlocks = numel(blocks);
	storage.blocks = blocks;
	storage.blockInputs = arrayfun(@(x) zeros(x.numInputs,1), blocks, 'uniformoutput', false);
	storage.blockOutputs = arrayfun(@(x) zeros(x.numOutputs,1), blocks, 'uniformoutput', false);
	storage.calcBlockInputs = calcBlockInputs;
	storage.calcSystemOutput = calcSystemOutput;
	storage.systemStorage = systemStorage;

    totalBlockStates = sum([blocks.numStates]);
	b = struct;
	b.numStates = totalBlockStates;
	b.numInputs = numSystemInputs;
	b.numOutputs = numSystemOutputs;
	b.f = @physics;
	b.h = @output;
	b.storage = storage;
end

// BlockSystem
function [dState, storage] = physics(...
	numStates,...
	numInputs,...
	time,...
	state,...
	input,...
	storage...
)
    blocks = storage.blocks;
	// update all of the block outputs based on the latest state
	xi = 0;
	for i = 1:storage.numBlocks
        statei = xi + (1:blocks(i).numStates);
		[storage.blockOutputs{i}, blocks(i).storage] = blocks(i).h(blocks(i).numStates, blocks(i).numOutputs, time, state(statei), blocks(i).storage);
		xi = xi + blocks(i).numStates;
	end
	// calculate the block inputs from the updated block outputs
    [storage.blockInputs, storage.systemStorage] = storage.calcBlockInputs( storage.numBlocks, storage.blocks, time, storage.blockOutputs, numInputs, input, storage.systemStorage );
	// finally calculate the block dstate from the updated block inputs
	xi = 0;
	for i = 1:storage.numBlocks
        statei = xi + (1:blocks(i).numStates);
		[dState(statei), blocks(i).storage] = blocks(i).f(blocks(i).numStates, blocks(i).numInputs, time, state(statei), storage.blockInputs{i}, blocks(i).storage);
		xi = xi + blocks(i).numStates;
	end
    storage.blocks = blocks;
end

function [output, storage] = output(...
	numStates,...
	numOutputs,...
	time,...
	state,...
	storage...
)
    blocks = storage.blocks;
	xi = 0;
	for i = 1:storage.numBlocks
        statei = xi + (1:blocks(i).numStates)
		[storage.blockOutputs{i}, blocks(i).storage] = blocks(i).h(blocks(i).numStates, blocks(i).numOutputs, time, state(statei), blocks(i).storage);
		xi = xi + blocks(i).numStates;
    end
    
    [storage.systemOutputs, storage.systemStorage] = storage.calcSystemOutput( numOutputs, time, storage.blockOutputs, storage.systemStorage );
    storage.blocks = blocks;
end


