#include "vertexcollection2d.h"
#include "buffer.h"




begin_gtl_namespace
begin_gdb_namespace

 
/** ���캯��
* @param [in] ��
* @return
*/
VertexCollection2d::VertexCollection2d(void)
{
}

/** ��������
* @param [in] obj
* @return ��
*/
VertexCollection2d::VertexCollection2d(VertexCollection2d& obj)
{
	long lCount = obj.getCount();
	for (long i = 0; i < lCount; i++)
	{
		m_pointList.push_back(new Vertex2d(*(obj.getPointPtr(i))));
	}
}

/**��������
* @param [in]
* @return
*/
VertexCollection2d::~VertexCollection2d(void)
{
	clear();
}

/**���������Ϣд��������У������ļ���д�򻺳���д����
* @param  [in,out] f  std::ostream &����׼�������������
* @return �ɹ�����true�����򷵻�false
*/
void VertexCollection2d::write(std::ostream & f)
{
	size_t bufSize = sizeBuffer();
	Buffer buf;
	void * p = malloc(bufSize);
	buf.attach(p, bufSize);
	bool success = writeBuffer(buf);

	f.write((const char *)(&bufSize), sizeof(size_t));
	f.write((const char *)(&buf), bufSize);
	buf.clear();
}

/**�ӱ�׼����������ȡ�������Ϣ
* @param  [in,out] f  std::istream &����׼��������������
* @return �ɹ�����true�����򷵻�false
*/
void VertexCollection2d::read(std::istream & f)
{
	size_t bufSize = 0;
	f.read((char *)(&bufSize), sizeof(size_t));/*��ȡ����д�뵽Buffer����ռ�õ��ֽ���*/

	Buffer buf;
	void * p = malloc(bufSize);  /*�����ڴ�*/
	buf.attach(p, bufSize);      /*Attach��ָ��һ����Ч�ľ�������������*/
	f.read((char *)(&buf), bufSize);/*��������Ϣ��ȡ��buffer��*/
	bool success = readBuffer(buf);
	buf.clear();
}

/**��Buffer�ж�ȡ��Ϣ��䱾���ζ���
* @param  [in,out] buf Buffer & ����������������
* @return �ɹ�����true�����򷵻�false
*##ModelId=4D0B58C70264
*/
bool VertexCollection2d::writeBuffer(Buffer &buf)
{
	int nelementNum = m_pointList.size();
	buf.write(&nelementNum, sizeof(int));

	for (std::vector<Vertex2d *>::iterator it = m_pointList.begin(); it != m_pointList.end(); it++)
	{
		buf.write(&((*it)->x), sizeof(double));
		buf.write(&((*it)->y), sizeof(double));
		//double x = (*it)->x;
		//double y = (*it)->y;
		//buf.write(&x,sizeof(double));
		//buf.write(&y,sizeof(double));
	}

	// 	for (int i=0;i<nelementNum;i++)
	// 	{
	// 		buf.write(&(m_pointList[i]->x),sizeof(double));
	// 		buf.write(&(m_pointList[i]->y),sizeof(double));
	// 	}
	return true;
}

/**��Buffer�ж�ȡ��Ϣ��䱾���ζ���
* @param  [in,out] buf Buffer & ����������������
* @return �ɹ�����true�����򷵻�false
*##ModelId=4D0B58C70264
*/
bool VertexCollection2d::readBuffer(Buffer &buf)
{
	clear();
	int listSize = 0;
	buf.read(&listSize, sizeof(int));

	double x, y;
	for (long i = 0; i < listSize; i++)
	{
		buf.read(&x, sizeof(double));
		buf.read(&y, sizeof(double));
		m_pointList.push_back(new Vertex2d(x, y));
	}
	return true;
}


/** ��ȡ����д�뵽Buffer����ռ�õ��ֽ���
* @param ��
* @return ��ȡ����buffer�ֽ���
*/
size_t VertexCollection2d::sizeBuffer()
{
	return sizeof(int) + m_pointList.size() * 2 * sizeof(double);
}


/** ���Ԫ�ص�Ԫ�ؼ���
* ���before, after��Ϊ�գ�����ӵ�Ԫ�ؼ���ĩβ
* @param [in] pt��Ҫ�����Ԫ��
* @param [in] before���ڴ�����֮ǰ����
* @param [in] after���ڴ�����֮�����
* @return
*/
void VertexCollection2d::addPointPtr(Vertex2d* obj, int* before, int* after)
{
	if (after == NULL && before == NULL)
	{
		m_pointList.push_back(obj);
	}
	else
	{
		if (before == NULL)
		{
			m_pointList.insert(m_pointList.begin() + *after, obj);
		}
		else
		{
			m_pointList.insert(m_pointList.begin() + *before - 1, obj);
		}
	}
}


/** ���Ԫ�ص�Ԫ�ؼ���
* ���before, after��Ϊ�գ�����ӵ�Ԫ�ؼ���ĩβ
* @param [in] pt��Ҫ�����Ԫ��
* @param [in] before���ڴ�����֮ǰ����
* @param [in] after���ڴ�����֮�����
* @return
*/
void VertexCollection2d::addPoint(double x, double y, int* before, int* after)
{
	Vertex2d* obj = new Vertex2d(x, y);
	if (after == NULL && before == NULL)
	{
		m_pointList.push_back(obj);
	}
	else
	{
		if (before == NULL)
		{
			m_pointList.insert(m_pointList.begin() + *after, obj);
		}
		else
		{
			m_pointList.insert(m_pointList.begin() + *before - 1, obj);
		}
	}
}

/** ��Ŀ��Ԫ�ؼ��е�Ԫ����ӵ���ǰԪ�ؼ���ĩβ
* @param [in] newElements��Ŀ��Ԫ�ؼ�
* @return ����Ƿ�ɹ�
*/
bool VertexCollection2d::addCollection(VertexCollection2dSharedPtr& newElements)
{
	if (newElements == NULL) return false;
	long lCount = newElements->getCount();
	double x, y;
	for (long i = 0; i < lCount; i++)
	{
		newElements->getPoint(i, x, y);
		m_pointList.push_back(new Vertex2d(x, y));
	}
	return true;
}

/** ��Ŀ��Ԫ�ؼ��е�Ԫ�ز��뵽��ǰԪ�ؼ���
* @param [in] index������λ��
* @param [in] newElements��Ŀ��Ԫ�ؼ�
* @return ����Ƿ�ɹ�
*/
bool VertexCollection2d::insertCollection(long index, VertexCollection2dSharedPtr& newElements)
{
	if (newElements == NULL) return false;
	index = max(0, index);
	index = min(index, (long)m_pointList.size() - 1);

	long lCount = newElements->getCount();
	double x, y;
	for (long i = 0; i < lCount; i++)
	{
		newElements->getPoint(i, x, y);
		m_pointList.insert((m_pointList.begin() + index + i), new Vertex2d(x, y));
	}
	return true;
}

/** ��ȡָ��������Ԫ��
* @param [in] index: Ҫ��ȡ��Ԫ�ص�����
* @return: ��ȡ����Ԫ�ص�ָ�룬��ȡʧ���򷵻�NULL
*/
Vertex2d* VertexCollection2d::getPointPtr(long index)
{
	return m_pointList.at(index);
}

const Vertex2d & VertexCollection2d::getPoint(long index)
{
	return *(m_pointList.at(index));
}

void VertexCollection2d::getPoint(long index, Vertex2d & outPoint)
{
	outPoint.x = m_pointList[index]->x;
	outPoint.y = m_pointList[index]->y;
}

void VertexCollection2d::getPoint(long index, double &x, double &y)
{
	x = m_pointList[index]->x;
	y = m_pointList[index]->y;
}


/** ����ָ��������Ԫ������
* @param [in] index: Ҫָ����Ԫ�ص�����
* @return:
*/
void VertexCollection2d::setPoint(long index, const Vertex2d & outPoint)
{
	m_pointList[index]->x = outPoint.x;
	m_pointList[index]->y = outPoint.y;
}

void VertexCollection2d::setPoint(long index, double x, double y)
{
	m_pointList[index]->x = x;
	m_pointList[index]->y = y;
}


/** ��ȡԪ�صĸ���
* @return: Ԫ�صĸ���
*/
long VertexCollection2d::getCount()
{
	return (long)m_pointList.size();
}

/** ����Ԫ�ؼ��е�����Ԫ��
* @param [in] index1,index2: Ҫ������Ԫ���������е�����
* @return: �����ɹ�����true�����򷵻�false
*/
bool VertexCollection2d::swap(long index1, long index2)
{
	if (index1<0 || index1>(long)m_pointList.size() - 1)
	{
		return false;
	}
	if (index2<0 || index2>(long)m_pointList.size() - 1)
	{
		return false;
	}
	Vertex2d * ptemp = m_pointList[index1];
	m_pointList[index1] = m_pointList[index2];
	m_pointList[index2] = ptemp;
	return true;
}

/** ��Ԫ�ؼ����Ƴ�ָ��������Ԫ�أ��������Ƴ���Ԫ��
* @param [in] index: Ҫ�Ƴ���Ԫ�ص�����
* @return: �Ƴ���Ԫ�ص�ָ�룬ʧ���򷵻�NULL
*/
Vertex2d * VertexCollection2d::removePointPtr(long index)
{
	if (index<0 || index>(long)m_pointList.size() - 1)
	{
		return 0;
	}
	Vertex2d* p = m_pointList[index];
	m_pointList.erase(m_pointList.begin() + index);
	return p;
}

/** ��Ԫ�ؼ���ɾ��ָ��������Ԫ��
* @param [in] index: Ҫɾ����Ԫ�ص�����
* @return: ɾ��ʧ���򷵻�false
*/
bool VertexCollection2d::deletePoint(long index)
{
	if (index<0 || (index>(long)m_pointList.size() - 1))
	{
		return false;
	}
	Vertex2d* p = m_pointList[index];
	m_pointList.erase(m_pointList.begin() + index);
	delete p;
	return true;
}

/** ��Ԫ�ؼ���ɾ����Ч�ĵ㣨������ڵ�������ͬ����ɾ����
* @param [in]
* @return: �����Ч��󣬵���ĵ�ĸ���
*/
int VertexCollection2d::deleteInvalidPoint()
{
	Vertex2d * pt1 = 0;
	Vertex2d * pt2 = 0;
	for (int i = 1; i < (int)m_pointList.size(); i++)
	{
		pt1 = m_pointList[i];
		pt2 = m_pointList[i - 1];
		if (fabs(pt1->x - pt2->x) < SMALL_NUMBER &&
			fabs(pt1->y - pt2->y) < SMALL_NUMBER)
		{
			deletePoint(i);
			i--;
		}
	}
	return m_pointList.size();
}

/** ��յ�ǰԪ�ؼ�
* @return
*/
void VertexCollection2d::clear()
{
	long lCount = (long)m_pointList.size();
	for (long i = 0; i < lCount; i++)
	{
		delete m_pointList[i];
	}
	m_pointList.clear();
}

/** @ ��Ŀ������ g �еĿ����� ��ǰ������
* @param  [in] g , ElementCollection2d *, ָ��������������ָ��
* @return �Ƿ񿽱��ɹ�
*/
bool VertexCollection2d::copy(const VertexCollection2dSharedPtr& g)
{
	double x, y;
	for (long i = 0; i < g->getCount(); i++)
	{
		g->getPoint(i, x, y);
		m_pointList.push_back(new Vertex2d(x, y));
	}
	return true;
}

/** ��ȡ�߳���
* @param ��
* @return �߳���
*/
double VertexCollection2d::getLength()
{
	int nSize = (int)m_pointList.size();
	if (nSize < 2)return 0.0;

	double dsumLength = 0.0;
	Vertex2d * pCurPoint = NULL;
	Vertex2d * pNextPoint = NULL;
	for (int i = 0; i < nSize - 1; i++)
	{
		pCurPoint = m_pointList[i];
		pNextPoint = m_pointList[i + 1];
		dsumLength += sqrt((pCurPoint->x - pNextPoint->x)*(pCurPoint->x - pNextPoint->x)
			+ (pCurPoint->y - pNextPoint->y)*(pCurPoint->y - pNextPoint->y));
	}
	return dsumLength;
}


/** ��ȡ�����Χ��
* @param [out] envp
* @return
*/
void VertexCollection2d::getEnvelope(Envelope3d * envp)
{
	envp->minx = 1E20;
	envp->maxx = -1E20;
	envp->miny = 1E20;
	envp->maxy = -1E20;
	envp->minz = -100;
	envp->maxz = 100;
	int ncount = m_pointList.size();

	for (int i = 0; i < ncount; i++)
	{
		envp->minx = min(envp->minx, m_pointList[i]->x);
		envp->maxx = max(envp->maxx, m_pointList[i]->x);
		envp->miny = min(envp->miny, m_pointList[i]->y);
		envp->maxy = max(envp->maxy, m_pointList[i]->y);
	}
}

end_gdb_namespace
end_gtl_namespace


