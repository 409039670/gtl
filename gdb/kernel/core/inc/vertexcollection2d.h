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
#include "assert.h"
#include <cmath>
#include "vertex2d.h" 


begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

class Buffer;
class VertexCollection2d;
typedef std::shared_ptr< VertexCollection2d >  VertexCollection2dSharedPtr;

/** @addtogroup vertex  VertexCollection2d-�㼯�࣬�������������Ԫ�ؼ�
*  @{
*/
class CORE_API VertexCollection2d
{
public:
	/** ���캯��
	* @param [in] ��
	* @return
	*/
	VertexCollection2d(void);

	/** ��������
	* @param [in] obj
	* @return ��
	*/
	VertexCollection2d(VertexCollection2d& obj);


	/**��������
	* @param [in]
	* @return
	*/
	~VertexCollection2d(void);

	/**���������Ϣд��������У������ļ���д�򻺳���д����
	* @param  [in,out] f  std::ostream &����׼�������������
	* @return �ɹ�����true�����򷵻�false
	*/
	void write(std::ostream & f);

	/**�ӱ�׼����������ȡ�������Ϣ
	* @param  [in,out] f  std::istream &����׼��������������
	* @return �ɹ�����true�����򷵻�false
	*/
	void read(std::istream & f);

	/*��Buffer�ж�ȡ��Ϣ��䱾���ζ���
	* @param  [in,out] buf Buffer & ����������������
	* @return �ɹ�����true�����򷵻�false
	*/
	bool writeBuffer(cn::edu::cug::gdb::Buffer &buf);

	/*��Buffer�ж�ȡ��Ϣ��䱾���ζ���
	* @param  [in,out] buf Buffer & ����������������
	* @return �ɹ�����true�����򷵻�false
	*/
	bool readBuffer(cn::edu::cug::gdb::Buffer &buf);

	/** ��ȡ����д�뵽Buffer����ռ�õ��ֽ���
	* @param ��
	* @return ��ȡ����buffer�ֽ���
	*/
	size_t sizeBuffer();

	/** ���Ԫ�ص�Ԫ�ؼ���
	* ���before, after��Ϊ�գ�����ӵ�Ԫ�ؼ���ĩβ
	* @param [in] pt��Ҫ�����Ԫ��
	* @param [in] before���ڴ�����֮ǰ����
	* @param [in] after���ڴ�����֮�����
	* @return
	*/
	void addPoint(double x, double y, int* before = NULL, int* after = NULL);
	void addPointPtr(Vertex2d* obj, int* before = NULL, int* after = NULL);

	/** ��Ŀ��Ԫ�ؼ��е�Ԫ����ӵ���ǰԪ�ؼ���ĩβ
	* @param [in] newElements��Ŀ��Ԫ�ؼ�
	* @return ����Ƿ�ɹ�
	*/
	bool addCollection(VertexCollection2dSharedPtr& newElements);

	/** ��Ŀ��Ԫ�ؼ��е�Ԫ�ز��뵽��ǰԪ�ؼ���
	* @param [in] index������λ��
	* @param [in] newElements��Ŀ��Ԫ�ؼ�
	* @return ����Ƿ�ɹ�
	*/
	bool insertCollection(long index, VertexCollection2dSharedPtr& newElements);

	/** ��ȡָ��������Ԫ��
	* @param [in] index: Ҫ��ȡ��Ԫ�ص�����
	* @return: ��ȡ����Ԫ�ص�ָ�룬��ȡʧ���򷵻�NULL
	*/
	Vertex2d* getPointPtr(long index);
	virtual const Vertex2d & getPoint(long index);
	virtual void getPoint(long index, Vertex2d & outPoint);
	virtual void getPoint(long index, double &x, double &y);

	/** ����ָ��������Ԫ������
	* @param [in] index: Ҫָ����Ԫ�ص�����
	* @return:
	*/
	virtual void setPoint(long index, const Vertex2d &pPoint);
	virtual void setPoint(long index, double x, double y);

	/** ��ȡԪ�صĸ���
	* @return: Ԫ�صĸ���
	*/
	long getCount();

	/** ����Ԫ�ؼ��е�����Ԫ��
	* @param [in] index1,index2: Ҫ������Ԫ���������е�����
	* @return: �����ɹ�����true�����򷵻�false
	*/
	bool swap(long index1, long index2);

	/** ��Ԫ�ؼ����Ƴ�ָ��������Ԫ�أ��������Ƴ���Ԫ��
	* @param [in] index: Ҫ�Ƴ���Ԫ�ص�����
	* @return: �Ƴ���Ԫ�ص�ָ�룬ʧ���򷵻�NULL
	*/
	Vertex2d * removePointPtr(long index);

	/** ��Ԫ�ؼ���ɾ��ָ��������Ԫ��
	* @param [in] index: Ҫɾ����Ԫ�ص�����
	* @return: ɾ��ʧ���򷵻�false
	*/
	bool deletePoint(long index);

	/** ��Ԫ�ؼ���ɾ����Ч�ĵ㣨������ڵ�������ͬ����ɾ����
	* @param [in]
	* @return: �����Ч��󣬵���ĵ�ĸ���
	*/
	int deleteInvalidPoint();

	/** ��յ�ǰԪ�ؼ�
	* @return
	*/
	void clear();

	/** @ ��Ŀ������ g �еĿ����� ��ǰ������
	* @param  [in] g , ElementCollection2d *, ָ��������������ָ��
	* @return �Ƿ񿽱��ɹ�
	*/
	bool copy(const VertexCollection2dSharedPtr& g);

	/** ��ȡ�߳���
	* @param ��
	* @return �߳���
	*/
	double getLength();

	/** ��ȡ�����Χ��
	* @param [out] envp
	* @return
	*/
	virtual void getEnvelope(cn::edu::cug::gdb::Envelope3d * envp);
protected:
	std::vector< Vertex2d  * > m_pointList;

};
/** @} */

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace

