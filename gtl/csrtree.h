/*
* Geosciences Template Library
*
* Copyright (c) 2008
* Zhenwen He (zwhe@hotmail.com)
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Zhenwen He  makes no  representations
* about the suitability of this software for any purpose. 
* It is provided "as is" without express or implied warranty.
*/

#pragma once
#include <cstdlib>			// standard C++ includes
#include <math.h>			// math routines
#include "rtree.h"
#include "matrix.h"
#include "sort.h"
#include "envelope.h"
#include "../kmeans/KMlocal.h"

begin_gtl_namespace

template<
	typename OBJTYPE, 
	typename ELEMTYPE, 
	int NUMDIMS,
	typename ELEMTYPEREAL = ELEMTYPE,
	typename  NodeVisitorType=RTreeNodeVisitor<OBJTYPE>,
	int TMAXNODES = 8, 
	int TMINNODES = TMAXNODES / 2,
	// template paramenters for Object class 
	typename T=char,
	typename TRAITS=std::char_traits<T>,
	typename ALLOC=std::allocator<T> >
class CSRTree:public RTree<OBJTYPE,ELEMTYPE,NUMDIMS,ELEMTYPEREAL,NodeVisitorType,TMAXNODES, TMINNODES,T,TRAITS,ALLOC>
{
public:
	virtual std::basic_string<T,TRAITS,ALLOC> getClassName(){
		return "CSRTree";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
public:
	typedef Envelope<ELEMTYPE,NUMDIMS> BoundRect;

	typedef RTree<OBJTYPE,ELEMTYPE,NUMDIMS,ELEMTYPEREAL,NodeVisitorType,TMAXNODES, TMINNODES> BaseClass;
	typedef std::pair<OBJTYPE,BoundRect> Item;//�ռ����͸ö���ı߽����,��Ϊ�ռ�ʵ����
	typedef std::vector<Item*> Items;//�ռ�ʵ�����,��ʾ����������ͬ��ռ����
	typedef std::vector<std::vector<int>>     ItemsIndexVector;//���������Ľ��,vector��Ԫ�ظ������Ǿ����������
protected:
	Items       items;//�������ǰ�����ݼ���
	ItemsIndexVector itemsvector;//��������Ľ��,���а���Items�ĸ�����Ϊ��������
	ELEMTYPE    * centers;//ÿ������ĵ�,��СΪ[/*count*NUMDIMS*/]
protected:
	//��������xyzSort����,��Ҫʵ�ֶԽڵ��XYZɨ������,����������index_orders��
	void xyz_sort(const int count, const ELEMTYPE cts[/*count*NUMDIMS*/],std::vector<int>& index_orders)
	{
		int ** iosv = new int* [NUMDIMS];
		int s = count;
		ELEMTYPE * temp = new ELEMTYPE [count];
		ELEMTYPE * sumv  = new ELEMTYPE [NUMDIMS];
		for(int i=0;i<NUMDIMS;i++){
			iosv[i]= new int [count];
			for(int j=0;j<count;j++){
				temp[j]=cts[j*NUMDIMS+i];
				iosv[i][j]=j;
			}
			//��temp�е����ݽ�������,����������isov[i]��
			IndexSorter<ELEMTYPE,int,void*> sorter(temp,iosv[i],count);
			sorter.sortAsce();
			sorter.getResult(iosv[i],&s);
			sumv[i] =0;
			for(int j=0;j<s-1;j++){
				sumv[i]+=(temp[iosv[i][j+1]]-temp[iosv[i][j]]);
			}
		}
		//�Ƚ�sumv�ĸ���������С,ȥ��С��
		ELEMTYPE t = sumv[0];
		s=0;
		for(int i=1;i<NUMDIMS;i++){
			if(t>sumv[i]){
				s=i;
				t=sumv[i];
			}
		}
		//s Ϊ�ۼӺ���С��ά������,���������������Ӧ����isov[s]
		index_orders.resize(count);
		std::copy(iosv[s],iosv[s]+count,index_orders.begin());

		delete [] temp;
		delete [] sumv;
		for(int i=0;i<NUMDIMS;i++){
			delete [] iosv[i];
		}
		delete [] iosv;
	}

	//���ݷ����Ľ��ItemsIndexVector,����������ײ�,�����ɵĸ��ڵ�����parents��
	void construct_tree_leaves(const ItemsIndexVector & data, std::vector<Node*> & children){
		for(int i=0;i<data.size();i++){
			int s = data[i].size();
			int node_count  = (s+MAXNODES)/MAXNODES;
			std::vector<Node*> nodes(node_count);
			//�����0-node_count-2�Ľڵ�����
			for(int j=0;j<node_count-1;j++){
				nodes[j] = new Node;
				nodes[j]->m_count=MAXNODES;
				nodes[j]->m_level=0;
				//����Branch������
				for(int m = 0; m<nodes[j]->m_count;m++){
					nodes[j]->m_branch[m].m_data=data[i][j*MAXNODES+m];
					BoundRect & br = items[data[i][j*MAXNODES+m]]->second;
					std::copy(br.getMinVector(),br.getMinVector()+NUMDIMS,nodes[j]->m_branch[m].m_rect.m_min);
					std::copy(br.getMaxVector(),br.getMaxVector()+NUMDIMS,nodes[j]->m_branch[m].m_rect.m_max);	
				}
			}
			//�����node_count-1�Ľڵ�����
			nodes[node_count-1] = new Node;
			nodes[node_count-1]->m_count=s-(node_count-1)*MAXNODES;
			nodes[node_count-1]->m_level=0;
			//���õ�node_count-1�Ľڵ��Branch������
			for(int m = 0; m<nodes[node_count-1]->m_count;m++){
				nodes[node_count-1]->m_branch[m].m_data=data[i][(node_count-1)*MAXNODES+m];
				BoundRect & br = items[data[i][(node_count-1)*MAXNODES+m]]->second;
				std::copy(br.getMinVector(),br.getMinVector()+NUMDIMS,nodes[(node_count-1)]->m_branch[m].m_rect.m_min);
				std::copy(br.getMaxVector(),br.getMaxVector()+NUMDIMS,nodes[(node_count-1)]->m_branch[m].m_rect.m_max);	
			}
			//���ڵ���ӵ����б���
			for(int n =0;n<node_count;n++)
				children.push_back(nodes[n]);
		}
	}
	//ͨ��construct_tree_leaves����õ����ӽڵ��б�����,���ظ��ڵ�
	void construct_tree(std::vector<Node*> & children, std::vector<Node*> & newNodes){
		int oldnodes_count=children.size();
		int newnodes_count = (oldnodes_count+MAXNODES)/MAXNODES;
		std::vector<Node*> nodes(newnodes_count);
		//�����0-newnodes_count-2�Ľڵ�����
		for(int j=0;j<newnodes_count-1;j++){
			nodes[j] = new Node;
			nodes[j]->m_count=MAXNODES;
			nodes[j]->m_level=1;
			//����Branch������
			for(int m = 0; m<nodes[j]->m_count;m++){
				nodes[j]->m_branch[m].m_child=children[j*MAXNODES+m];
				nodes[j]->m_branch[m].m_rect=NodeCover(children[j*MAXNODES+m]);
			}
		}
		//�����node_count-1�Ľڵ�����
		nodes[newnodes_count-1] = new Node;
		nodes[newnodes_count-1]->m_count=oldnodes_count-(newnodes_count-1)*MAXNODES;
		nodes[newnodes_count-1]->m_level=1;
		//���õ�newnodes_count-1�Ľڵ��Branch������
		for(int m = 0; m<nodes[newnodes_count-1]->m_count;m++){
			nodes[newnodes_count-1]->m_branch[m].m_child=children[(newnodes_count-1)*MAXNODES+m];
			nodes[newnodes_count-1]->m_branch[m].m_rect=NodeCover(children[(newnodes_count-1)*MAXNODES+m]);
		}
		//���ڵ���ӵ����б���
		for(int n =0;n<newnodes_count;n++)
			newNodes.push_back(nodes[n]);

		if(newnodes_count>1)
		{
			children.clear();
			children.resize(newNodes.size());
			std::copy(newNodes.begin(),newNodes.end(),children.begin());
			newNodes.clear();
			construct_tree(children,newNodes);
		}
		else{
			m_root= newNodes[0];
			return;
		}
	}

public:
	
	//���ڲ���������ԭʼ��������
	void randomBounds(int ncount){
		clearItems(items);
		ELEMTYPE d[NUMDIMS];
		std::vector<ELEMTYPE> rs(NUMDIMS);
		for(int i=0;i<ncount;i++){
			for(int k=0;k<NUMDIMS;k++){
				d[k]=rand()%10000;
				rs[k]=rand()%10;
			}
			Item * t= new Item;
			t->first=i;
			t->second=BoundRect(d,rs);
			items.push_back(t);
		}
	}
	void addRandomBounds(const int ncount,BoundRect& br ){
		ELEMTYPE d[NUMDIMS];
		ELEMTYPE d2[NUMDIMS];
		for(int i=0;i<ncount;i++){
			for(int k=0;k<NUMDIMS;k++){
				do{
					d[k]=rand();//%((long)((br.getMaxVector())[k]));
				}while(d[k]<(br.getMinVector())[k] || d[k]>(br.getMaxVector())[k]);
				do{
					d2[k]=rand();//%((long)((br.getMaxVector())[k]));
				}while(d2[k]<(br.getMinVector())[k] || d2[k]>(br.getMaxVector())[k]);

				d[k]=min(d[k],d2[k]);
				d2[k]=max(d[k],d2[k]);
			}
			Item * t= new Item;
			t->first=i;
			t->second=BoundRect(d,d2);
			items.push_back(t);
		}
	}
	BoundRect randomBound(){
		ELEMTYPE d[NUMDIMS];
		std::vector<ELEMTYPE> rs(NUMDIMS);
		
		for(int k=0;k<NUMDIMS;k++){
			d[k]=rand()%10000;
			rs[k]=rand()%500;
		}
		return BoundRect(d,rs);
	}

	Items & getOriginalData(){return items;}

	void clearItems(Items & v){
		for(Items::iterator it = v.begin();it!=v.end();it++){
			delete (*it);//���ٵײ��Item����
		}
		v.clear();
	}
	
	void resizeCenters(int n ){
		if(centers){
			delete [] centers;
			centers=0;
		}
		centers = new ELEMTYPE[n*NUMDIMS];
	}
	
	int getCenters(std::vector<int> & indexs,ELEMTYPE ** cts){
		int s = indexs.size();
		* cts = new ELEMTYPE[s*NUMDIMS];
		for(int i =0;i<s;i++){
			items[indexs[i]]->second.getCenter(*cts+i*NUMDIMS);
		}
		return indexs.size();
	}
	

public:
	CSRTree():RTree(){
		centers=0;
	}
	~CSRTree(){
		if(centers) delete [] centers;
		clearItems(items);
	}
public:	
	/* ͨ��K-Means������������ݷ�Ϊn��,���ؾ�������� */
	int kmeansClustering(const int k, const Items & dataIn, ItemsIndexVector &  dataOut,ELEMTYPE cts[/*k*NUMDIMS*/] ){


		int dim =NUMDIMS;
		int nptns=dataIn.size();

		KMterm term(500,0,0,0,
			0.1,0.1,3,
			0.5,10,0.95);
		
		KMdata data(dim,nptns);
		//kmUniformPts(data.getPts(),nptns,dim);
		for(int i=0;i<nptns;i++){
			((BoundRect&)(dataIn[i]->second)).getCenter(data[i]);
		}
		data.buildKcTree();
		KMfilterCenters fc(k,data);
		KMlocalLloyds kl(fc,term);
		fc=kl.execute();
		//fc.print();
		//��ȡÿ������ĵ�
		KMcenterArray ca = fc.getCtrPts();
		for(int i=0;i<k;i++){
			for(int j=0;j<NUMDIMS;j++){
				cts[i*NUMDIMS+j]=ca[i][j];
			}
		}
		//��ȡÿ����������һ�����Ϣ
		KMctrIdxArray closeCtr = new KMctrIdx[data.getNPts()];
		double* sqDist = new double[data.getNPts()];
		fc.getAssignments(closeCtr, sqDist);

		
		dataOut.resize(k);
		for(int i=0;i<k;i++){
			dataOut[i].clear();
		}
		for(int i=0;i<data.getNPts();i++){
			dataOut[closeCtr[i]].push_back(i);
		}

		delete [] closeCtr;
		delete [] sqDist;
		
		return 0;
	}
	/* ��ItemsVector �е�ÿ�����ݽ���XYZ����*/
	void xyzSort(ItemsIndexVector &  dataInOut ){
		//����ÿ������ĵ�����
		std::vector<int> iv;
		int k= dataInOut.size();
		xyz_sort(k,centers,iv);
		ItemsIndexVector iiv(k);
		for(int i=0;i<k;i++){
			iiv[i].resize(dataInOut[iv[i]].size());
			std::copy(dataInOut[iv[i]].begin(),dataInOut[iv[i]].end(),iiv[i].begin());
		}
		//��ÿ���еĽڵ��������,Ҳ����iiv[i]�еĽڵ��������
		iv.clear();
		for(int i=0;i<k;i++){
			int s = iiv[i].size();//��ȡ��i��Ľڵ����
			// �������ĵ�Ϊ����
			ELEMTYPE * cts = 0;
			getCenters(iiv[i],&cts);
			xyz_sort(s,cts,iv);
			delete [] cts;
			std::vector<int> t(s);
			for(int j=0;j<s;j++){
				t[j]=iiv[i][iv[j]];
			}
			std::copy(t.begin(),t.end(),iiv[i].begin());
		}
		//��iiv������dataInOut��
		for(int i=0;i<iiv.size();i++){
			int s = iiv[i].size();
			dataInOut[i].resize(s);
			std::copy(iiv[i].begin(),iiv[i].end(),dataInOut[i].begin());
		}
	}
	/*	ͨ�������Ľ����������R��	,KΪ��������*/
	void constructTree(int k){	

		resizeCenters(k);
		kmeansClustering(k,items,itemsvector,centers);
		//printClusteringResults();
		xyzSort(itemsvector);
		//printSortResults();
		std::vector<Node*> children,newNodes;
		construct_tree_leaves(itemsvector,children);
		construct_tree(children,newNodes);
	}

	void printClusteringResults(){
		std::cout<<"\n"<<"begin print clustering results:\n";
		int k = itemsvector.size();
		for (int i = 0; i < k; i++) {
			for(int j=0;j<itemsvector[i].size();j++)
			{
				ELEMTYPE * mi=items[itemsvector[i][j]]->second.getMinVector();
				ELEMTYPE * ma=items[itemsvector[i][j]]->second.getMaxVector();
				std::cout	<<i<<" (";
				for(int m=0;m<NUMDIMS;m++){
					std::cout<<centers[i*NUMDIMS+m]<<" ";
				}
				std::cout<< ") "<< items[itemsvector[i][j]]->first<< "" ;

				std::cout<<" ( ";
				for(int m=0;m<NUMDIMS;m++){
					std::cout<<mi[m]<<" ";
					std::cout<<ma[m]<<" ";
				}
				std::cout<< ") "<<endl;

			}
			std::cout<<"\n";
		}
		std::cout<<"\n"<<"end print clustering results!\n";
		getchar();
	}
	void printSortResults(){
		std::cout<<"\n"<<"begin print sort results:\n";
		int k = itemsvector.size();
		ELEMTYPE * cts =new ELEMTYPE[NUMDIMS];
		for (int i = 0; i < k; i++) {
			for(int j=0;j<itemsvector[i].size();j++)
			{
				ELEMTYPE * mi=items[itemsvector[i][j]]->second.getMinVector();
				ELEMTYPE * ma=items[itemsvector[i][j]]->second.getMaxVector();
				items[itemsvector[i][j]]->second.getCenter(cts);
				std::cout	<<i<<" (";
				for(int m=0;m<NUMDIMS;m++){
					std::cout<<cts[m]<<" ";
				}
				std::cout<< ") "<< items[itemsvector[i][j]]->first<< "" ;

				std::cout<<" ( ";
				for(int m=0;m<NUMDIMS;m++){
					std::cout<<mi[m]<<" ";
					std::cout<<ma[m]<<" ";
				}
				std::cout<< ") "<<endl;

			}
			std::cout<<"\n";
		}
		delete [] cts;
		std::cout<<"\n"<<"end print sort results!\n";

	}
};


///*	TreeGrid�ĵ�Ԫ����ģ���ࣻ
//OBJTYPE�ǿռ����Ķ�������,ELEMTYPE�Ǳ߽���ε�Ԫ������,
//NUMDIMS�Ǳ߽���ε�ά����ռ�����ά��,ELEMTYPEREAL������ֵ������õ���������,
//NodeVisitorType�����ڵ���ʶ�������,TMAXNODES��ʾһ���ڵ������ķ�֧��,
//TMINNODES��ʾһ���ڵ������ٱ�����TMINNODES����֧��*/
//template<class OBJTYPE, class ELEMTYPE, int NUMDIMS,class ELEMTYPEREAL = ELEMTYPE,
//class  NodeVisitorType=RTreeNodeVisitor<OBJTYPE>,int TMAXNODES = 8, int TMINNODES = TMAXNODES/2>
//class GridCell:public Object{
//public:
//	typedef Envelope<ELEMTYPE,NUMDIMS> BoundRect;
//protected:
//	BoundRect bound;//�����ı߽����
//	//������Ӧ��CR-Tree��������
//	CSRTree<OBJTYPE,ELEMTYPE,NUMDIMS,ELEMTYPEREAL,NodeVisitorType,TMAXNODES, TMINNODES> crtree;
//	std::string  name;//������Ӧ������ģ���ļ����ƻ������ݿ��е���Ӧ�����������
//public:
//
//};
///*	TreeGrid�Ӿ�����̳�,��֤Ԫ��ΪGridCell��
//OBJTYPE�ǿռ����Ķ�������,ELEMTYPE�Ǳ߽���ε�Ԫ������,
//NUMDIMS�Ǳ߽���ε�ά����ռ�����ά��,ELEMTYPEREAL������ֵ������õ���������,
//NodeVisitorType�����ڵ���ʶ�������,TMAXNODES��ʾһ���ڵ������ķ�֧��,
//TMINNODES��ʾһ���ڵ������ٱ�����TMINNODES����֧��*/
//template<class OBJTYPE, class ELEMTYPE, int NUMDIMS,class ELEMTYPEREAL = ELEMTYPE,
//class  NodeVisitorType=RTreeNodeVisitor<OBJTYPE>,int TMAXNODES = 8, int TMINNODES = TMAXNODES/2>
//class TreeGrid:public Matrix<GridCell<OBJTYPE,ELEMTYPE,NUMDIMS,ELEMTYPEREAL,NodeVisitorType,TMAXNODES, TMINNODES>>
//{
//};
//

end_gtl_namespace