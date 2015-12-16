// gtl_test_dtree2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../../gtl/decompositiontree.h"

typedef gtl::DecompositionTree< gtl::Envelope<double, 4>, double, 4> DecompositionTree;
int _tmain(int argc, _TCHAR* argv[])
{
	DecompositionTree::envelope total;
	DecompositionTree::storage_manager * sm = new gtl::MemoryStorageManager<char>();
	DecompositionTree dtree(total, sm);

	dtree.insert(DecompositionTree::Entry());
	std::vector<DecompositionTree::Entry> results;
	dtree.search(total, results);
	delete sm;
	return 0;
}

