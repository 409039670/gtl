// gtl_test_rlist.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../GTL/rlist.h"


int _tmain(int argc, _TCHAR* argv[])
{
	double mi[3],ma[3];


	//ʱ���¼
	time_t start,end; 

	std::vector<int> nodes;
	std::vector<float> rtimes;
	std::vector<float>csrtimes;
	std::vector<float>xtimes;

	std::vector<float> rtimes_c;
	std::vector<float>csrtimes_c;
	std::vector<float>xtimes_c;

	std::vector<int> rtree_node_numbs;
	std::vector<int> csrtree_node_numbs;
	std::vector<int> xlist_node_numbs;

	typedef gtl::CSRTree<int,double,3,double,gtl::RTreeNodeVisitor<int>,50,2> CSRTREE;
	typedef gtl::Rlists<int,double,3,50> RLISTS;

	for(int t=2;t<56;t+=1)
	{


		std::cout<<t<<std::endl;

		CSRTREE csr_tree;
		CSRTREE r_tree;
		RLISTS r_list;

		for(int n=0;n<t;n+=2){
			//����100*t���ڵ�����
			mi[0]=0+n*500;mi[1]=n*500;mi[2]=n*500;
			ma[0]=100+n*500;ma[1]=n*500+100;ma[2]=n*500+100;
			csr_tree.addRandomBounds(100,gtl::BoundRect<double,3>(mi,ma));
		}
		for(int n=1;n<t;n+=2){
			//����100*t���ڵ�����
			mi[0]=0+n*500;mi[1]=n*500;mi[2]=n*500;
			ma[0]=100+n*500;ma[1]=n*500+100;ma[2]=n*500+100;
			csr_tree.addRandomBounds(100,gtl::BoundRect<double,3>(mi,ma));
		}

		nodes.push_back(100*t);

		//���ڲ�ѯ����
		mi[0]=50;mi[1]=50;mi[2]=50;
		ma[0]=500*(t-1);ma[1]=500*(t-1);ma[2]=500*(t-1);
		gtl::BoundRect<double,3> br(mi,ma);


		CSRTREE::Items & data =csr_tree.getOriginalData();

		//����Rlist�Ĺ������������µ�X-Lists,ԭʼ����һ��	
		start=clock(); 
		for(int i =0 ;i<data.size();i++){
			r_list.insert(i,data[i]->second);
		}
		end=clock(); 
		xtimes_c.push_back((float)(end-start)/CLK_TCK);

		//R-LISTS����
		std::vector<int> vv;
		start=clock(); 
		for(int i=0;i<1000;i++)
			r_list.search(br,vv);
		end=clock(); 
		xtimes.push_back((float)(end-start)/CLK_TCK);


		//����CSRTRee�ľ�̬�������������������Ĺ����н���Щ�ڵ��Ϊ3��
		start=clock(); 
		csr_tree.constructTree(t);
		end=clock(); 
		csrtimes_c.push_back((float)(end-start)/CLK_TCK);



		//CSR-TREE����
		start=clock(); 
		for(int i=0;i<1000;i++)
			csr_tree.search(br);
		end=clock(); 
		csrtimes.push_back((float)(end-start)/CLK_TCK);

		//������ͨRTree�Ĺ������������µ�CSRTree,������ԭʼ����һ��	
		start=clock(); 
		for(int i =0 ;i<data.size();i++){
			r_tree.insert(data[i]->second,i);
		}
		end=clock(); 
		rtimes_c.push_back((float)(end-start)/CLK_TCK);

		//R-TREE����
		start=clock(); 
		for(int i=0;i<1000;i++)
			r_tree.search(br);
		end=clock(); 
		rtimes.push_back((float)(end-start)/CLK_TCK);



		/*rtree_node_numbs.push_back(r_tree.numbNodes());
		csrtree_node_numbs.push_back(csr_tree.numbNodes());
		xlist_node_numbs.push_back(r_list.numberNodes());*/


		rtree_node_numbs.push_back(r_tree.allocSize()/1024.0);
		csrtree_node_numbs.push_back(csr_tree.allocSize()/1024.0);
		xlist_node_numbs.push_back(r_list.allocSize()/1024.0);
	}

	ofstream ofs("d:\\data.txt");

	ofs<<"Test run time record begin:\n";

	for(int i=0;i<nodes.size();i++){
		ofs<<setw(5)<<nodes[i]<<setw(10)<<xtimes_c[i]<<setw(10)<<rtimes_c[i]<<setw(10)<< csrtimes_c[i]<<setw(10)<<xtimes[i]<<setw(10)<<rtimes[i]  <<setw(10)<< csrtimes[i]<<setw(10)<<xlist_node_numbs[i]<<setw(10)<<rtree_node_numbs[i]<<setw(10)<<csrtree_node_numbs[i]<<endl;
	}
	ofs<<"Test run time record ending:\n";

	ofs.close();

	return 0;
}

