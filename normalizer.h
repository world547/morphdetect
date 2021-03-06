#ifndef __NORMALIZER_H
#define __NORMALIZER_H
#include <unordered_set>

namespace detect_similar
{

using namespace std;

class BlockInfo;

class Normalizer
{
public:
	Normalizer(BlockInfo*);
	void remember(BlockInfo*);
	void forget(BlockInfo*);
	void normalize();
	void changeRoot(BlockInfo*);
	const unordered_set <BlockInfo*> *known();
private:
	bool _running;
	unordered_set <BlockInfo*> _known;
	BlockInfo* _root;
};

} //namespace detect_similar

#endif //__NORMALIZER_H
