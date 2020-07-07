#include <Filter/App.h>
#include <Filter/PrefixTree.h>
#include <Filter/Filter.h>

#include <assert.h>

bool run(LaunchConfig *launchCfg)
{
	assert(launchCfg != NULL);

	PrefixTree const* prefixTree = buildPrefixTree(launchCfg->prefixFilePath);
	if (!prefixTree)
		return false;

	bool result = filterInputFile(launchCfg->inFilePath, launchCfg->outFilePath, prefixTree);
	
	free(prefixTree);
	
	return result;
}