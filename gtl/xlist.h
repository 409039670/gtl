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
#include <stack> 
#include "envelope.h"
begin_gtl_namespace

/** @defgroup   Xlist  ��������б�                     
    *  @{
*/

template<
typename  OBJECTTYPE ,/**�ռ��������*/  
typename  VALUETYPE, /*��С�߽���ε�ֵ����*/  
int NUMDIMS ,/*�ռ����ά��*/
int MAXELEMS ,/*ÿ���ӱ����Ԫ�ظ���*/ 
typename T=char,
typename TRAITS=std::char_traits<T>,
typename ALLOC=std::allocator<T>
>
class  Xlist : public Object<T,TRAITS,ALLOC>{
public:
	virtual std::basic_string<T,TRAITS,ALLOC> getClassName(){
		return "Xlist";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
    virtual Object<T,TRAITS,ALLOC> * clone() {
		return 0;
	}
public:
	struct INDEXSTATS{
		unsigned long long objects;// spatiotemopral objects
		unsigned long long nodes;// total nodes in the index
		unsigned long long innernodes;// inner nodes in the index
		unsigned long long leafnodes;// inner nodes in the index
		unsigned long long splits;// the times of splitting operations
		unsigned long long keycomparisons;// the key comparison times;
		unsigned long long reads;// the reading io times;
		unsigned long long writes;//the writing io times;
		unsigned long long depth;// the depth of index tree or list
	} ;

	INDEXSTATS m_stats;
public:
	typedef Envelope<VALUETYPE,NUMDIMS> BoundingBox; /** @brief ��С�߽���� */
	
	struct Element{
		/** @brief ָ��ǰһ��ͬ������Ԫ�� */
		Element * prev; 
		/** @brief ָ���һ��ͬ������Ԫ�� */
		Element * next; 
		/** @brief ָ���ϼ��б�Ԫ�� */
		//Element * parent; 
		/** @brief �б�Ԫ������ 0��ʾΪԭ��Ԫ�أ�1��ʾΪ���б� */
		unsigned char type; 
		/** @brief ��type==0�ǣ�objptr��Ч������sublist��Ч */
		union {
			OBJECTTYPE   obj;
			Element * sublist;
		};
		/** @brief Ԫ�ص���С�߽���� */
		BoundingBox   bound;
	};

	Xlist(){
		_head=0;
		memset(&m_stats,0,sizeof(INDEXSTATS));
	}
	~Xlist(){
		clear();
		_head=0;
	}
	
protected:
	/** @brief �����б��ͷ */
	Element  * _head;
	/** @brief �����б��BoundingBox */
	BoundingBox   _bound;
protected:	
	
	/** ͳ�� �б���Ԫ�ظ���
		* @param  [in] h �б�ͷָ��
		* @return ����ͷ�ڵ�ͬ��������Ԫ�ظ���
		*/
	static int numberElements(Element  * h){
		Element * p = h;
		if(!h) return 0;
		int i=1;
		while(p->prev) p=p->prev;

		while (p->next) {
			i++;
			p=p->next;
		}
		return i;
	}
	/** ��ָ����Ԫ��֮ǰ����һ��Ԫ�� 
		* @param  [in,out] pos ����Ԫ��
		* @param  [in,out] e �������Ԫ��
		* @return ���ִ�гɹ��򷵻�true,���򷵻�false
		*/
	static bool insertBefore(Element* pos, Element  * e){
		if(!pos || !e) return false;
		if(!pos->prev) { /*���pos��ͷ */
			e->next = pos;
			pos->prev=e;
		}
		else{
			Element * p = pos->prev;
			p->next=e;
			e->prev=p;
			pos->prev=e;
			e->next=pos;
		}
		return true;
	}
	/** ��ָ����Ԫ��֮�����һ��Ԫ�� 
	* @param  [in,out] pos ����Ԫ��
	* @param  [in,out] e �������Ԫ��
	* @return ���ִ�гɹ��򷵻�true,���򷵻�false
	*/
	static bool insertAfter(Element* pos, Element  * e){
		if(!pos || !e) return false;
		if(!pos->next) { /*���pos��β*/
			e->prev = pos;
			pos->next=e;
		}
		else{
			Element * p = pos->next;
			pos->next=e;
			e->prev=pos;
			p->prev=e;
			e->next=p;
		}
		return true;
	}
	/** ��ȡ�б��е����һ��Ԫ��
	* @param  [in,out] h �б��е���һ��Ԫ��
	* @return ����β��Ԫ��
	*/
	static Element * getTail(Element* h){
		Element * e = h;
		while (e->next)
			e=e->next;
		return e;
	}
	/** ��ȡ�б��е���ǰһ��Ԫ��
	* @param  [in,out] h �б��е���һ��Ԫ��
	* @return ����ͷ��Ԫ��
	*/
	static Element * getHead(Element* h){
		Element * e = h;
		while (e->prev)
			e=e->prev;
		return e;
	}
	/** ��β������һ��Ԫ�� 
	* @param  [in,out] h �б��е���һ��Ԫ��
	* @param  [in,out] e �������Ԫ��
	* @return ���ִ�гɹ��򷵻�true,���򷵻�false
	*/
	static bool addTail(Element* h,Element  * e){
		if(!h || !e) return false;
		Element * pos = getTail(h);
		pos->next=e;
		e->prev=pos;
		return true;
	}
	/** ��ͷ������һ��Ԫ�� 
	* @param  [in,out] h �б��е���һ��Ԫ��
	* @param  [in,out] e �������Ԫ��
	* @return ���ִ�гɹ��򷵻�true,���򷵻�false
	*/
	static bool addHead(Element* h,Element  * e){
		if(!h || !e) return false;
		Element * pos = getHead(h);
		pos->prev=e;
		e->next=pos;
		return true;
	}
	/** �½�һ��ԭ�����͵��б�Ԫ�� 
		* @param  [in] obj �ռ����
		* @param  [in] b ����BoundingBox
		* @return ������Ԫ��
		*/
	static Element * newElement(OBJECTTYPE obj, BoundingBox & b){
		Element * e = new Element;
		e->bound=b;
		e->obj=obj;
		e->type=0;
		e->prev=0;
		e->next=0;
		return e;
	}
	/** �½�һ���ӱ����͵��б�Ԫ�� 
	* @param  [in] obj �ռ����
	* @param  [in] b ����BoundingBox
	* @return ������Ԫ��
	*/
	static Element * newElement(Element * c,bool isElement){
		Element * e = new Element;
		if(isElement){//���c��ʾ���ǵ�Ԫ��		
			e->bound=c->bound;
			e->sublist=c;
			e->type=1;		
			e->prev=0;
			e->next=0;

			//�������ӹ�ϵ
			if(c->prev){
				c->prev->next=e;
				e->prev=c->prev;
			}
			if(c->next){
				c->next->prev=e;
				e->next=c->next;
			}
			c->next=0;
			c->prev=0;
		}
		else{//���c�������һ������
			Element * h = getHead(c);

			e->sublist=h;
			e->type=1;		
			e->prev=0;
			e->next=0;

			while(h){
				e->bound +=h->bound;
				h=h->next;
			}
		}
		
		return e;
	}
	/** ����hΪͷ���б��У���ѯ��b�ཻ����Ԫ��
		* @param  [in] h �б�ͷԪ��
		* @param  [in] b ��С�߽����
		* @return ��b�ཻ����Ԫ��ָ��
		*/
	static Element * findOverlap(Element * h,BoundingBox & b){
		Element * e = h;
		while(e){
			if(e->bound.overlaps(b))
				return e;
			else
				e=e->next;
		}
		return e;
	}
	/** ��obj�滻��headΪͷ�ڵ�������ĳ��Ԫ�أ�ʹ��head�����е�����Ԫ�صİ�Χ����С 
		* @param  [in,out] head ����ͷ�ڵ�
		* @param  [in,out] obj Ҫ�滻��ȥ��Ԫ��
		* @return ���ִ�гɹ��򷵻ر��滻������Ԫ��,���򷵻�0
		*/
	static Element * replace(Element * & head,Element * obj){
		BoundingBox b ,bt;
		Element * h = head;
		Element * p = head;
		Element * p_min=0;

		while (p){
			b+=p->bound;
			p=p->next;
		}

		while(h){
			p=head;
			bt.reset();
			while(p){
				if(p==h){
					p=p->next;
					continue;
				}
				else{
					bt+=p->bound;
					p=p->next;
				}				
			}

			bt+=obj->bound;
			if(bt.volume()<b.volume()){
				b=bt;
				p_min=h;
			}
			h=h->next;
		}
		if(p_min){//��������滻��������obj��p_min�Ի�
			obj->next=p_min->next;
			obj->prev=p_min->prev;
			if (p_min->prev)
				p_min->prev->next=obj;
			if(p_min->next)
				p_min->next->prev=obj;

			p_min->prev=0;
			p_min->next=0;

			
			head = getHead(obj);
		}
		return p_min;
	}
protected:
	/** ��һ���ռ��������б���
		* @param  [in,out] head �б�ͷԪ��
		* @param  [in,out] bound �б��Ӧ��BoundingBox
		* @param  [in] head_parent headԪ�صĸ��ڵ�
		* @param  [in] obj �ռ����
		* @param  [in] b �ռ�����Ӧ�ı߽����
		* @return ���ִ�гɹ��򷵻�true,���򷵻�false
		*/
	static bool insert(Element* & head, BoundingBox & bound,Element*  head_parent, OBJECTTYPE obj, BoundingBox & b) {
		//����ǵ�һ��Ԫ�أ�ֱ�����
		if (!head)	{
			head = newElement(obj,b);
			bound=b;
			head->bound=b;
			return true;
		}
		/*********************************************************************** 
		��ȡRi�����ȼ���Ri��C�Ƿ��ཻ��
		1��	������ཻ������k<m-1����Ri��ΪC�ı�βԪ����ӣ�
		2��	������ཻ������k=m-1����C��Ϊ��ͷ��Ri��Ϊ��β�������µı����¼�ΪC��
		3��	����ཻ�����Ҵ���Cj��Ri�ཻ����Ri��Cj�ϲ����µ�Cj��
		4��	����ཻ�����Ҳ�����Cj��Ri�ཻ����k<m-1����Ri��ΪC�ı�βԪ����ӣ���k=m-1����Ri��Ck(C�ı�βԪ��)�ϲ����µ�Ck��
		***********************************************************************/
		if( bound.overlaps(b)){//������ܱ��ཻ
			Element * e = findOverlap(head,b);
			if(e){//���ܱ��е�Ԫ���ཻ
				if(e->type==0) {//�����ԭ��Ԫ��
					if(head_parent){
						int k = numberElements(e);
						if(k<MAXELEMS) {
							addTail(e,newElement(obj,b));
							head_parent->bound.extend(b);
							bound.extend(b);
						}
						else{
							Element * sub = newElement(e,true);
							addTail(e,newElement(obj,b));
							sub->bound.extend(b);
							bound.extend(b);
						}						
					}
					else{
						Element * sub = newElement(e,true);
						
						addTail(e,newElement(obj,b));
						sub->bound.extend(b);
						bound.extend(b);
					}
										
				}
				else{//�������ԭ��Ԫ��
					bound.extend(b);
					insert(e->sublist,e->bound,e,obj,b);
					
				}				
			}
			else{//�����ܱ��е��κ�Ԫ���ཻ
				int k = numberElements(head);
				
				if(k<MAXELEMS){
					e=newElement(obj,b);
					addTail(head,e);
					bound.extend(b);
				}
				else{//��Ҫ����
					Element * e= newElement(obj,b);
					Element* t = replace(head,e);
					if(t){//�滻������Ԫ�ز�Ϊ��
						Element * h = newElement(head,false);
						while(head){
							h->bound += head->bound;
							head=head->next;
						}						
						addTail(h,t);
						bound.extend(b);
						head=h;
					}
					else{
						Element * h = newElement(head,false);
						h->bound=bound;
						addTail(h,e);
						bound.extend(b);
						head=h;
					}									
				}
			}
		}
		else{//������ܱ��ཻ
			int k = numberElements(head);
			if(k<MAXELEMS){
				Element * e = newElement(obj,b);
				addTail(head,e);
				bound.extend(b);
			}
			else{
				Element * h = newElement(head,false);
				h->bound=bound;
				Element * e= newElement(obj,b);
				addTail(h,e);
				bound.extend(b);
				
				head=h;
			}
		}
		return true;
	}
	/** ������Һ��� 
		* @param  [in] head ��ѯ�б��ͷָ��
		* @param  [in] lists_bound  ��ѯ�б�ı߽����
		* @param  [in] search_bound  ��ѯ�߽����
		* @param  [in,out] results  ���ز�ѯ���Ľ��
		* @return ���ִ�гɹ��򷵻ز�ѯ���Ķ������,���򷵻�0
		*/
	static int search(Element * head,BoundingBox & lists_bound, BoundingBox & search_bound,std::vector<OBJECTTYPE> & results, INDEXSTATS & stats){
		if(!lists_bound.overlaps(search_bound))
			return 0;

		Element * p = head;
		while (p){
#ifdef  _GTL_DEBUG_
			stats.keycomparisons+=1;
#endif //_GTL_DEBUG_
			if(p->bound.overlaps(search_bound)){
				if(p->type==0){
					results.push_back(p->obj);
					p=p->next;

				}
				else{
					search(p->sublist,p->bound,search_bound,results,stats);
#ifdef  _GTL_DEBUG_
					++ (stats.reads);
#endif //_GTL_DEBUG_
					p=p->next;

				}
			}
			else
			{
				p=p->next;
			}
		}
		return results.size();
	}
	/** ��head��ʾ���б��в���obj�����Ԫ�ؽڵ� 
		* @param  [in] obj �ռ����
		* @param  [in] head �б�ͷ�ڵ�
		* @param  [in] head_parent �б�ͷ�ڵ�ĸ��ڵ�ָ��
		* @param  [in,out] pe ����ҵ���ͨ��pe����
		* @param  [in,out] pe_parent ����pe�ĸ��ڵ�
		* @return ���ִ�гɹ��򷵻�true,���򷵻�false
		*/
	static bool find(OBJECTTYPE obj,Element * head, Element * head_parent,Element *  &pe , Element  * & pe_parent){
		
		Element * h = head;
		Element * p = head_parent;
		while (h){
			if(h->type==0){
				if(h->obj==obj){
					pe = h;
					pe_parent=p;
					return true;
				}
				else{
					h=h->next;
				}
			}
			else{
				if(find(obj,h->sublist,h,pe,pe_parent))
					return true;
				else
					h=h->next;
			}
			
		}
		return false;		
	}
	/** �ж���h��ͷ���б�ʱ�������б�����з���true�����û�У�����false 
		* @param  [in] h ͷָ��
		* @return ���ִ�гɹ��򷵻�true,���򷵻�false
		*/
	static bool has_sublist(Element * h){
		if(!h) return false;
		Element * p = h;
		while (p){
			if(p->type!=0)
				return true;
			else
				p=p->next;
		}
		return false;
	}
	
	/** ��head��ʾ���б��в���obj����ĸ���·�� 
	* @param  [in] obj �ռ����
	* @param  [in] head �б�ͷ�ڵ�
	* @param  [in] head_parent �б�ͷ�ڵ�ĸ��ڵ�ָ��
	* @param  [in,out] parents ����ڵ�ջ
	* @return ���ִ�гɹ��򷵻�true,���򷵻�false
	*/
	static bool find(OBJECTTYPE obj,Element * head, Element * head_parent,std::stack<Element *> &parents){
		/*[find_parents]
		(1)	���head_parent��Ϊ�գ���head_parent��ջ��
		(2)	���head��ԭ�ӣ���h->obj����obj���򷵻�true��
		(3)	���head��ԭ�ӣ���h->obj������obj������h=h->next��ֱ��hΪ�ջ�h����ԭ�ӽڵ㣻
		(4)	���head��Ϊ�գ���ѭ��ִ�����沽�裻
		(5)	���head�ӱ���head_parent=head��head=head->sublist��
		(6)	�ݹ����find_parents��������ؽ��Ϊtrue����ֱ�ӷ��أ�
		(7)	����ݹ���÷��ؽ��Ϊfalse������head=head_parent��head_parent=ջ��Ԫ�أ�����ջ��
		��head=head->next���ݹ����find_parents���������Ϊtrue����ֱ�ӷ��أ��������Ϊfalse���ظ�ִ�в���(4)������(7)��*/

		
		Element * h = head;
		Element * p = head_parent;
		if(p) 
			parents.push(p);

		while(h){
			if(h->type==0){
				if(h->obj==obj)
					return true;
				h=h->next;
			}
			else
				break;			
		}

		while(h){
			p=h;		
			h=h->sublist;
			if(find(obj,h,p,parents)){				
				return true;
			}
			else{
				h=p;
				p=parents.top();
				parents.pop();
				h=h->next;
				if(find(obj,h,p,parents)){					
					return true;
				}
			}
		}

		parents.pop();
		return false;
	}
	/** ͳ���б����ܽ��ж��ٸ��ڵ� 
	* @return ���ִ�гɹ��򷵻ؽڵ����,���򷵻�-1
	*/
	static long numberNodes(Element * h){
		int i=0;
		while (h)
		{
			i++;
			if(h->type!=0)
				i+=numberNodes(h->sublist);
			h=h->next;
		}
		return i;
	}
	/** ����б� 
		* @param  [in] head �б�ͷ�ڵ�ָ��
		* @return ��
		*/
	static void clear(Element * head){
		while(head){
			if(head->type==0){//�����ԭ�ӣ���ֱ��ɾ���ýڵ㣬��������������
				Element *  n = head->next;				
				if(n){
					n->prev=0;
					delete head;
					head = n;
				}
				else
					break;
			}
			else{
				clear(head->sublist);
			}
			head = head->next;
		}
	}
public:
	/** ����б�����_bound����
		* @return ��
		*/
	void clear(){
		clear(_head);
		_bound.reset();
		_head=0;
	}

	/** ���������б�ռ�õ��ڴ�ռ��С
		* @return ���������б�ռ�õ��ڴ�ռ��С
		*/
	long allocSize(){
		return numberNodes()*sizeof(Element);
	}

	/** ͳ���б����ܽ��ж��ٸ��ڵ� 
	* @return ���ִ�гɹ��򷵻ؽڵ����,���򷵻�-1
	*/
	long numberNodes(){
		return numberNodes(_head);
	}
	/** ����һ��Ԫ�� 
	* @param  [in] obj �ռ����
	* @param  [in] b �ռ�����Ӧ��BoundingBox
	* @return ���ִ�гɹ��򷵻�true,���򷵻�false
	*/
	bool insert(OBJECTTYPE obj, BoundingBox b){
#ifdef  _GTL_DEBUG_
					++ m_stats.objects;
#endif //_GTL_DEBUG_
		return insert(_head,_bound,0,obj,b);
	}
	/** ������Һ��� 
		* @param  [in] search_bound ��ѯ������
		* @param  [in,out] results ��ѯ�Ľ��
		* @return ���ִ�гɹ��򷵻ز��ҵ��ĸ���,���򷵻�0
		*/
	int search(BoundingBox search_bound,std::vector<OBJECTTYPE> & results){
#ifdef  _GTL_DEBUG_
					++ m_stats.reads;
#endif //_GTL_DEBUG_
		return search(_head,_bound,search_bound,results,m_stats);
	}
	/** ��head��ʾ���б��в���obj�����Ԫ�ؽڵ� 
	* @param  [in] obj �ռ����
	* @param  [in,out] pe ����ҵ���ͨ��pe����
	* @param  [in,out] pe_parent ����pe�ĸ��ڵ�
	* @return ���ִ�гɹ��򷵻�true,���򷵻�false
	*/
	bool find(OBJECTTYPE obj,Element *  &pe , Element  * & pe_parent){
		return find(obj,_head,0,pe,pe_parent);
	}
	/** ��head��ʾ���б��в��Ҵ�head��obj�����·�� 
	* @param  [in] obj �ռ����
	* @param  [in] head �б�ͷ�ڵ�
	* @param  [in] head_parent �б�ͷ�ڵ�ĸ��ڵ�ָ��
	* @param  [in,out] parents ����ڵ�ջ
	* @return ���ִ�гɹ��򷵻�true,���򷵻�false
	*/
	bool find(OBJECTTYPE obj,std::stack<Element *> &parents){
		if(find(obj,_head,0,parents)){
				Element * p = parents.top()->sublist;
				while (p){
					if(p->obj!=obj)
						p=p->next;
					else{
						parents.push(p);
						return true;
					}
				}
		}
		return false;
	}

	/** ɾ���㷨 
	* @param  [in] obj �ռ����
	* @return ���ִ�гɹ��򷵻�ɾ��Ԫ��ָ��,���򷵻�0
	*/
	Element * remove(OBJECTTYPE obj){
		std::stack<Element *> path;
		if(!find(obj,path)) 
			return 0;

		//������ֽڵ㣬����·��
		Element * p_obj = path.top();
		path.pop();
		Element * p =p_obj;
		Element * h = p;
		if(path.empty()){//Ϊ�����Ԫ��
			if(!p->prev) {//ͷԪ��
				h = p->next;
				h->prev=0;
				p->next=0;
			}
			else if (!p->next){//βԪ��
				h = p->prev;
				p->prev=0;
				h->next=0;
				h = getHead(h);
			}
			else{//�м�Ԫ��
				h = p->prev;
				h->next=p->next;
				p->next->prev=h;
				p->prev=0;
				p->next=0;
				h = getHead(h);
			}		
			//���¼���_bound
			_bound.reset();
			while (h){
				_bound+=h->bound;
				h=h->next;
			}
		}
		else{//������Ƕ���Ԫ��
			Element * parent   = path.top();
			path.pop();
			if(!p->prev) {//ͷԪ��
				h = p->next;
				h->prev=0;
				p->next=0;

			}
			else if (!p->next){//βԪ��
				h = p->prev;
				p->prev=0;
				h->next=0;
				h = getHead(h);
			}
			else{//�м�Ԫ��
				h = p->prev;
				h->next=p->next;
				p->next->prev=h;
				p->prev=0;
				p->next=0;
				h = getHead(h);
			}		
			parent->sublist=h;

			if(!h->next){//���h���ڵ�ͬ��Ԫ��ֻ��1������h�ϲ����ϼ��б���
				
				h->prev=parent->prev;
				h->next=parent->next;

				if(parent->next)
					parent->next->prev=h;
				if(parent->prev)
					parent->prev->next=h;

				parent->prev=0;
				parent->next=0;
				parent->sublist=0;
				delete parent;

				while(!path.empty()){
					h = getHead(h);
					parent   = path.top();	
					path.pop();
					parent->bound.reset();
					while (h){
						parent->bound +=h->bound;
						h=h->next;
					}
				}

				//��������ı߽�
				_bound.reset();
				parent = getHead(parent);
				while (parent){
					_bound +=parent->bound;
					parent=parent->next;
				}
			}
			else{//���¼���_bound					
				do {
					h = getHead(h);
					parent->bound.reset();
					while (h){
						parent->bound +=h->bound;
						h=h->next;
					}
					h = parent;
					parent   = path.top();	
					path.pop();
				}while(!path.empty());

				//��������ı߽�
				_bound.reset();
				parent = getHead(parent);
				while (parent){
					_bound +=parent->bound;
					parent=parent->next;
				}
			}				
		}
		return p_obj;
		
	}
};


/** @}*/ 


end_gtl_namespace
