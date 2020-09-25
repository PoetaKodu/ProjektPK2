#include <Filter/App.h>
#include <Filter/PrefixTree.h>
#include <Filter/Filter.h>

#include <stdlib.h>
#include <assert.h>

/////////////////////////////////
bool run(LaunchConfig *launchCfg_)
{
	assert(launchCfg_ != NULL);

	PrefixTree const* prefixTree = buildPrefixTree(launchCfg_->prefixFilePath);
	if (!prefixTree)
		return false;

	bool result = filterInputFile(launchCfg_->inFilePath, launchCfg_->outFilePath, prefixTree);

	// Zwalnianie pamięci:
	destroyPrefixTree(prefixTree);
	free(prefixTree);

	return result;
}