#include "connector.h"
#include "builder.h"
#include "factory.h"




begin_gtl_namespace
begin_gdb_namespace

BuilderSharedPtr Builder::newBuilder(int signal){
	switch(signal){
	case Factory::FACTORYTYPE_FDB_BUILDER://�ļ����ݿ�
		{
			return ((FDBBuilderFactory*)Factory::getFactory(signal))->newBuilder();
		}
	case Factory::FACTORYTYPE_RDB_BUILDER://��ϵ���ݿ�
		{
			return ((RDBBuilderFactory*)Factory::getFactory(signal))->newBuilder();
		}
	case Factory::FACTORYTYPE_CDB_BUILDER://��Ⱥ���ݿ�
		{
			return ((CDBBuilderFactory*)Factory::getFactory(signal))->newBuilder();
		}
	case Factory::FACTORYTYPE_DDB_BUILDER://�ֲ�ʽ�������ݿ�
		{
			return ((DDBBuilderFactory*)Factory::getFactory(signal))->newBuilder();
		}
	}
	return BuilderSharedPtr();
}

Builder::Builder():_connector(""){
	_builderType=0;
	_quicktype = false; //Ĭ��Ϊfalse����Ҫ��ѯID���ж��Ƿ���ڣ�Ȼ������
}
Builder::~Builder(){}

/** @name ��������������  
*  @{
*/	
/**  ��ȡbuilder������
* @param [in] ��
* @return ����1Ϊ�ļ�ϵͳ,���򷵻�2Ϊ���ݿ�ϵͳ
*/
int Builder::getType(){
	return _builderType;
}

/**  ��ȡ��ʱ�������м�����¼
* @param [in] ��
* @return ������ʱ�����м�����¼TemporarySets
*/
TemporarySets & Builder::getTemporarySets(){ return _tempsets;}

/**  ��ȡѡ�񼯺�
* @param [in] ��
* @return ������ʱ�����м�����¼TemporarySets
*/
Selector & Builder::getSelector(){ return _selector;}

/**  ɾ��ѡ���е�Ҫ�ض���
* @return ����ɾ���ĸ��� 
*/
long Builder::eraseSelectedFeatures() {
	size_t r = _selector.size();
	for(size_t i =0; i<r;i++){
		FeatureSharedPtr p = _selector.removeAt(i);
		if(p)
			removeFeature(p->getID(),p->getFeatureClassID());
	}
	return r;
}

long Builder::mergeFeatureClass( const FCLSID & fclsid,vector<FCLSID> &fclsids)
{
	long iCountMerge = 0;
	FeatureClassSharedPtr mainFcsp;
	getProxy()->getFeatureClass(fclsid,mainFcsp);
	for(int i = 0 ;i < fclsids.size(); ++i)
	{
		FeatureClassSharedPtr fcsp;
		getProxy()->getFeatureClass(fclsids[i],fcsp);
		//���Ҫ����ṹһ�£�����Ժϲ�
		if (mainFcsp->isEqual(*fcsp))
		{
			vector<FeatureSharedPtr> objs;
			FeatureSharedPtr obj;
			getProxy()->queryByFeatureClassID(fclsids[i],objs,true);
			for(size_t j = 0; j < objs.size(); ++j)
			{
				objs[j]->setFeatureClass(mainFcsp);
				addFeature(objs[j]);
				addLOD(objs[j]);
			}
			removeFeatureClass(fclsids[i]);
			iCountMerge++;
		}
	}
	return iCountMerge;
}

/**  �����ݿ���вü����õ��Ľ�����浽resultBuilder��
* @param [in,out] resultBuilder 
* @return ִ�гɹ�����true�����򷵻�false
*/
bool Builder::clip( const Envelope3dRef bound, Builder * resultBuilder) {

	vector<FeatureClassSharedPtr> fclass_set;
	vector<FCLSID> fclassid_set;
	_proxySptr->getAllFeatureClass(fclass_set);
	vector<FeatureClassSharedPtr>::iterator it1=fclass_set.begin();
	while(it1!=fclass_set.end())	{
		fclassid_set.push_back((*it1)->getID());
		it1++;
	}

	std::vector<FeatureSharedPtr> fobj_set;
	_proxySptr->queryByBox(bound,fclassid_set,fobj_set);//����Χ���Ҫ�ض���
	_proxySptr->loadLOD(fobj_set);//��ȡҪ�ض����LOD��Ϣ

	//����
	if (fobj_set.size()>0)	{
		//��ȡ���ϲ�ѯ������Ҫ�ض����Ҫ����id
		FCLSID temfclsid;
		vector<FCLSID> vecFclsIdinCombox;
		vector<FeatureSharedPtr>::iterator it = fobj_set.begin();
		FeatureClass *pfcls = 0;
		this->setActive();
		for (unsigned int i = 0;i<fobj_set.size();i++)	{
			pfcls = (*it)->getFeatureClass().get();
			temfclsid = pfcls->getID();
			int flg = 0;
			for (unsigned int j =0;j< vecFclsIdinCombox.size();j++)	{
				if (temfclsid == vecFclsIdinCombox[j])	{
					flg = 1;
					break;
				}
			}

			if (!flg)	{
				vecFclsIdinCombox.push_back(temfclsid);
				vector<FeatureClassSharedPtr>::iterator itfcls=fclass_set.begin();
				while(itfcls!=fclass_set.end())
				{
					if ((*itfcls)->getID() == temfclsid)
					{
						resultBuilder->setActive();
						resultBuilder->addFeatureClass(*itfcls);
					}
					itfcls++;
				}
			}

			resultBuilder->setActive();
			resultBuilder->addFeature(*it);
			resultBuilder->addLOD(*it);
			this->setActive();
			it++;
		}

		//����
		std::vector<MATID> sMatIDSet; //���ڴ�Ű����Ĳ���ID
		std::vector<TEXID> sTexIDSet; //���ڴ�Ű���������ID
		std::vector<MDLID> sMdlIDSet; //���ڴ�Ű����Ĺ���ģ��ID

		vector<MaterialSharedPtr> aMaterials;//����������Ϣ
		vector<TextureSharedPtr> aTextures;//����������Ϣ
		vector<SharedModelSharedPtr> aModels;//����������Ϣ

		//��ȡFeature�����а������ʡ�����͹���ģ��ID
		for(vector<FeatureSharedPtr>::iterator fit= fobj_set.begin();fit!=fobj_set.end();fit++)
		{
			GeometrySharedPtr pGeom = (*fit)->getGeometry();
			/*SharedMdlFileUtils::getFileUtils().getSharedModelIDFromGeometry(pGeom,sMdlIDSet);
			MaterialFileUtils::getFileUtils().getMaterilIDFromGeometry(pGeom,sMatIDSet);*/
			_proxySptr->getSharedModelIDs(pGeom,sMdlIDSet);
			_proxySptr->getMaterilIDs(pGeom,sMatIDSet);
		}


		//�ӹ���ģ�Ϳ��в���ģ��
		for(vector<MDLID>::iterator MdlidIT = sMdlIDSet.begin(); MdlidIT != sMdlIDSet.end();MdlidIT++)
		{
			this->setActive();
			SharedModelSharedPtr pMdl = _proxySptr->getSharedModel(*MdlidIT);
			if (pMdl)
			{
				aModels.push_back(pMdl);
				resultBuilder->setActive();
				resultBuilder->addSharedModel(pMdl);
			}
		}

		//��ȡ����ģ���й����Ĳ���ID
		for(vector<SharedModelSharedPtr>::iterator mdlit = aModels.begin();mdlit!=aModels.end();mdlit++)		
		{
			vector<MATID> linkmatid = (*mdlit)->getMaterialIDs();

			long num=linkmatid.size();
			for (long i=0;i<num;i++)
			{
				MATID nMatID = linkmatid[i];
				sMatIDSet.push_back(nMatID);
			}
		}
		//�Ӳ��ʿ��в��Ҳ���
		for(vector<MATID>::iterator MatidIT = sMatIDSet.begin(); MatidIT != sMatIDSet.end();MatidIT++)	{
			this->setActive();
			MaterialSharedPtr pMat = _proxySptr->getMaterial(*MatidIT);
			if (pMat)	{
				aMaterials.push_back(pMat);
				resultBuilder->setActive();
				resultBuilder->addMaterial(pMat);
			}
		}


		//�ֲ������ͻ�ȡ����ID
		for(vector<MaterialSharedPtr>::iterator matit = aMaterials.begin();matit!=aMaterials.end();matit++)	{
			int MatType = (*matit)->getType();
			if(MatType == 0)
			{
				SimpleMaterial * psimple = dynamic_cast<SimpleMaterial *>((*matit).get());
				TEXID TextureID = psimple->getTextureID();
				sTexIDSet.push_back(TextureID);
			}
			else if(MatType == 1)
			{
				ComplexMaterial * pcomplex = dynamic_cast<ComplexMaterial *>((*matit).get());
				std::vector<COMMAT_ITEM> &MaterialItemList = pcomplex->getMaterialItems();
				long Num = MaterialItemList.size();
				for(long i=0; i<Num; i++)
				{
					TEXID texid = MaterialItemList[i].textureID;
					sTexIDSet.push_back(texid);
				}
			}
		}

		//��������в�������
		for(vector<TEXID>::iterator TexidIT = sTexIDSet.begin(); TexidIT != sTexIDSet.end();TexidIT++)
		{
			this->setActive();
			TextureSharedPtr pTex = _proxySptr->getTexture(*TexidIT);
			if (pTex)
			{
				aTextures.push_back(pTex);
				resultBuilder->setActive();
				resultBuilder->addTexture(pTex);
			}
		}

		//���˽ڵ�
		this->setActive();
		vector<TopoItemSharedPtr> topo_node;
		_proxySptr->getAllTopos(topo_node);
		resultBuilder->setActive();
		resultBuilder->addTopos(topo_node);
		topo_node.clear();

		//��������
		this->setActive();
		vector<TopoInfoSharedPtr> topo_info;
		_proxySptr->getAllTopos(topo_info);
		resultBuilder->setActive();
		resultBuilder->addTopos(topo_info);
		topo_info.clear();

		//�ͷ�
		resultBuilder->save();
		this->setActive();
		return true;
	}
	else
		return false;
}

bool Builder::exclude( const Envelope3dRef  bound)
{
	return false;
}		
/**  ��fromBuilder�е����ݵ���������ݿ���,������DEM��DOM����
* @param [in] fromBuilder 
* @return ִ�гɹ�����true�����򷵻�false
*/
bool Builder::import( Builder * fromBuilder) {
	vector<FeatureClassSharedPtr> fcls_orgset;
	_proxySptr->getAllFeatureClass(fcls_orgset); //��õ�ǰ���̵�Ҫ����

	//��ȡ�������ݿ��е�����
	fromBuilder->setActive();
	ProxySharedPtr proxy_from = fromBuilder->getProxy();

	//����Ҫ���༰Ҫ�ض�����Ϣ
	vector<FeatureClassSharedPtr> fcls_newset;
	proxy_from->getAllFeatureClass(fcls_newset);//���Ҫ���빤�̵�Ҫ����

	vector<FeatureClassSharedPtr>::iterator it1=fcls_newset.begin();
	while(it1!=fcls_newset.end())	{
		bool bfind=false;
		//�ж���Ҫ����ID�뵱ǰ���Ƿ�һ��
		vector<FeatureClassSharedPtr>::iterator it2=fcls_orgset.begin();
		while(it2!=fcls_orgset.end()){
			if ((*it1)->getID()==(*it2)->getID()){
				bfind=true;
				break;
			}
			it2++;
		}
		if (bfind==false){//��ID��һ���������Ҫ����			
			//���Ҫ����
			this->setActive();
			this->addFeatureClass(*it1);
		}
		//ͨ������Ҫ�����ȡ����Ҫ�ض�����ӵ�Ҫ������
		fromBuilder->setActive();
		//[2010-07-26 add by liuyanting �޸Ļ�ȡҪ�ض���ʽ��ԭ������������ʱ��������]
		vector<FID> fidset;
		//��ȡҪ�ض���ID����
		FCLSID fclsid = (*it1)->getID();
		proxy_from->queryFeatureIDsByFeatureClassID(fclsid,fidset);
		vector<FeatureSharedPtr> fobjset;
		vector<FID>::iterator it3 = fidset.begin();
		while (it3!=fidset.end()){
			fromBuilder->setActive();
			FeatureSharedPtr fobj;
			//��ȡҪ�ض���
			proxy_from->queryByFeatureID((*it3),fclsid,fobj);
			//��ȡLOD��Ϣ

			fobjset.push_back(fobj);
			proxy_from->loadLOD(fobjset);//��ȡLOD��Ϣ

			this->setActive();
			this->addFeature(fobj);//���Ҫ�ض���
			this->addLOD(fobj);//���LOD��Ϣ

			fobjset.clear();
			it3++;
		}
		it1++;
	}

	//����������ݡ��������ݡ�����ģ������
	//��������
	fromBuilder->setActive();
	vector<MATID> mater_id; 
	proxy_from->getAllMaterialID(mater_id);
	vector<MATID>::iterator matit=mater_id.begin();
	while (matit!=mater_id.end()){
		fromBuilder->setActive();
		MaterialSharedPtr pMat = proxy_from->getMaterial(*matit);
		this->setActive();
		this->addMaterial(pMat);
		matit++;			
	}
	mater_id.clear();

	//��������
	fromBuilder->setActive();
	vector<TEXID> tex_id;
	proxy_from->getAllTextureID(tex_id);
	vector<TEXID>::iterator texit=tex_id.begin();
	while (texit!=tex_id.end())	{
		fromBuilder->setActive();
		TextureSharedPtr pTex = proxy_from->getTexture(*texit);
		this->setActive();
		this->addTexture(pTex);
		texit++;
	}
	tex_id.clear();

	//����ģ������
	fromBuilder->setActive();
	vector<MDLID> mdl_id;
	proxy_from->getAllSharedModelID(mdl_id);
	vector<MDLID>::iterator mdlit=mdl_id.begin();
	while (mdlit!=mdl_id.end()) {
		fromBuilder->setActive();
		SharedModelSharedPtr pMdl = proxy_from->getSharedModel(*mdlit);
		this->setActive();
		this->addSharedModel(pMdl);
		mdlit++;		
	}
	mdl_id.clear();

	//���˽ڵ�
	fromBuilder->setActive();
	vector<TopoItemSharedPtr> topo_node;
	proxy_from->getAllTopos(topo_node);
	this->setActive();
	this->addTopos(topo_node);
	topo_node.clear();

	//��������
	fromBuilder->setActive();
	vector<TopoInfoSharedPtr> topo_info;
	proxy_from->getAllTopos(topo_info);
	this->setActive();
	this->addTopos(topo_info);
	topo_info.clear();

	//�������
	this->save();	
	return true;
}


BuilderSharedPtr Builder::clone(BuilderSharedPtr gFBuildr ){
	////�½�һ��Builder������
	//GfileBuilder * gFBuildr= new GfileBuilder;
	//gFBuildr->create(szNewPath);
	//gFBuildr->open(szNewPath);

	//����Ҫ�����ͺ�Ҫ����Ϣ
	std::map<FCLSID,FeatureClassSharedPtr> & fclsMap = getFeatureClassMap();
	for(std::map<FCLSID,FeatureClassSharedPtr>::iterator mit = fclsMap.begin();mit!=fclsMap.end();mit++)
	{
	
		gFBuildr->addFeatureClass( mit->second->clone() );
		this->setActive();
		FCLSID fclsid = mit->first;
		vector<FID> fids;
		getProxy()->queryFeatureIDsByFeatureClassID(fclsid,fids);

		vector<FeatureSharedPtr> fobjset;
		for (vector<FID>::iterator it_fid = fids.begin();it_fid != fids.end();it_fid++)
		{
			this->setActive();
			FeatureSharedPtr fobj;
			_proxySptr->queryByFeatureID((*it_fid),fclsid,fobj);
			fobjset.push_back(fobj);

			//����Ϊ��ȡfeature��lod��Ϣһ�ַ�ʽ  by liyingying
			/*LODInfo lodinfo = (*fobj).get()->getLODInfo();
			vector<Info> infos =  lodinfo.getLodS();
			for (vector<Info>::iterator it_info = infos.begin();it_info != infos.end();it_info++)
			{
			int lodLevel = (*it_info).first.lodLevel;
			proxy->loadLOD((*fobj).get()->getID(),lodLevel,(*it).get()->getID(),*fobj);
			}*/

			//[2010-07-28 modify by liuyanting �Ȼ�ȡlod��Ϣ���ٽ�feature���룬ʡȥaddlodʱ����feature�Ĳ���]
			_proxySptr->loadLOD(fobjset);//��ȡLOD��Ϣ
			gFBuildr->setActive();
			gFBuildr->addFeature(fobj);
			gFBuildr->addLOD(fobj);
			fobjset.clear();
			gFBuildr->save();
		}
	}

	//����������Ϣ
	this->setActive();
	std::vector<TEXID> texid_set;
	_proxySptr->getAllTextureID(texid_set);
	TextureSharedPtr  ptexture;
	for (vector<TEXID>::iterator it = texid_set.begin();it!=texid_set.end();it++)
	{
		this->setActive();
		ptexture =_proxySptr->getTexture(*it);
		gFBuildr->setActive();
		gFBuildr->addTexture(ptexture);
	}
	gFBuildr->save();

	//���ƹ���ģ����Ϣ
	this->setActive();
	std::vector<MDLID> mdlid_set;
	_proxySptr->getAllSharedModelID(mdlid_set);
	SharedModelSharedPtr psharemodel;
	for (vector<MDLID>::iterator it = mdlid_set.begin();it!= mdlid_set.end();it++)
	{		
		this->setActive();
		psharemodel = _proxySptr->getSharedModel(*it);
		gFBuildr->setActive();
		gFBuildr->addSharedModel(psharemodel);
	}
	gFBuildr->save();

	//���Ʋ�����Ϣ
	this->setActive();
	std::vector<MATID> matid_set;
	_proxySptr->getAllMaterialID(matid_set);
	MaterialSharedPtr pmat;
	for (vector<MATID>::iterator it = matid_set.begin();it != matid_set.end();it++)
	{
		this->setActive();
		pmat = _proxySptr->getMaterial(*it);
		gFBuildr->setActive();
		gFBuildr->addMaterial(pmat);
	}
	gFBuildr->save();

	//����������Ϣ
	this->setActive();
	std::vector<TopoItemSharedPtr> topo_nodes;
	std::vector<TopoInfoSharedPtr> topo_infos;
	_proxySptr->getAllTopos(topo_nodes);
	gFBuildr->setActive();
	gFBuildr->addTopos(topo_nodes);
	topo_nodes.clear();

	_proxySptr->getAllTopos(topo_infos);
	gFBuildr->setActive();
	gFBuildr->addTopos(topo_infos);
	topo_infos.clear();


	gFBuildr->save();
	return gFBuildr;
}


/**  ������ƽ��(dx,dy,dz)
* @param dx
* @param dy
* @param dz
* @return ��
*/
void Builder::translate(double dx, double dy , double dz) {
	//ͨ�����潻�����ƽ���������趨
	Matrix4x4 m4x4 = Matrix4x4::IDENTITY4X4;
	Matrix4x4 m_matrixTransForm;//����ƽ����ת���ŵı任�ۼƾ���
	Vertex3d offset;
	vector<FeatureSharedPtr> fobjs;//Ҫ�ض���
	std::map<FCLSID,FeatureClassSharedPtr> & fclsMap = getFeatureClassMap();
	for(std::map<FCLSID,FeatureClassSharedPtr>::iterator mit = fclsMap.begin();mit!=fclsMap.end();mit++)
	{

		fobjs.clear();
		getProxy()->queryByFeatureClassID(mit->first,fobjs);

		offset.x = dx;
		offset.y = dy;
		offset.z = dz;

		m4x4.buildTranslateMatrix(offset);
		for (vector<FeatureSharedPtr>::iterator it3 = fobjs.begin();it3 != fobjs.end();it3++ ){
			//(*it3)->getGeometry()->TranslateFrom(m4x4);
			// ���Geometry���ڣ���ִ��ƽ�� [�￨ 2010-8-19]
			GeometrySharedPtr pGeom = (*it3)->getGeometry();
			if (pGeom)
			{								
				if (pGeom->getType()==GEOMTYPE_3D_REFERENCE)
				{
					Reference3d *pRef =0;
					if(pGeom->queryInterface(GEOMTYPE_3D_REFERENCE,(void**)&pRef)){
						MDLID mdlID = pRef->getSharedModelID();
						SharedModelSharedPtr pLinkModel = _proxySptr->getSharedModel(mdlID);
						pRef->setSharedModel(pLinkModel);
					}					
				}
				else if (pGeom->getType()==GEOMTYPE_3D_GROUP)
				{
					// ֻ�������һ�㣬������������ˣ��������ú�������������Ϊref�ǲ�����Ϊgroup��Ա�� [�￨ 2010-8-20]
					Group3d  *pGroup = 0;

					if(pGeom->queryInterface(GEOMTYPE_3D_GROUP,(void**)&pGroup)){
						vector<Geometry*> geoCollection ;
						pGroup->getElements(geoCollection);
						for (vector<Geometry*>::iterator itr = geoCollection.begin();itr!=geoCollection.end();itr++){
							if ((*itr)->getType()==GEOMTYPE_3D_REFERENCE)
							{
								Reference3d *pRef = 0; 
								if((*itr)->queryInterface(GEOMTYPE_3D_REFERENCE,(void**)&pRef)){
									MDLID mdlID = pRef->getSharedModelID();
									SharedModelSharedPtr pLinkModel = _proxySptr->getSharedModel(mdlID);
									pRef->setSharedModel(pLinkModel);
								}								
							}
						}
					}
				}
				pGeom->translateFrom(m4x4);
			}
			modifyFeature(*it3);

			//ƽ�Ƽ���������Ϣ
			TopoInfoSharedPtr tip;
			getProxy()->queryTopo((*it3)->getID(),TOPOINFOTYPE_SEMATIC_GEOMETRY,tip);
			SemanticGeometryTopoInfo * sgi = dynamic_cast<SemanticGeometryTopoInfo*>(tip.get());
			if(sgi){
				GeometrySharedPtr pGeom = sgi->getGeometry();
				if (pGeom)
				{								
					if (pGeom->getType()==GEOMTYPE_3D_REFERENCE)
					{
						Reference3d *pRef =0;
						if(pGeom->queryInterface(GEOMTYPE_3D_REFERENCE,(void**)&pRef)){
							MDLID mdlID = pRef->getSharedModelID();
							SharedModelSharedPtr pLinkModel = _proxySptr->getSharedModel(mdlID);
							pRef->setSharedModel(pLinkModel);
						}					
					}
					else if (pGeom->getType()==GEOMTYPE_3D_GROUP)
					{
						// ֻ�������һ�㣬������������ˣ��������ú�������������Ϊref�ǲ�����Ϊgroup��Ա�� [�￨ 2010-8-20]
						Group3d *pGroup =0;// (C3DGroup*)pGeom;
						if(pGeom->queryInterface(GEOMTYPE_3D_GROUP,(void**)&pGroup)){
							vector<Geometry*> geoCollection ;
							pGroup->getElements(geoCollection);
							for (vector<Geometry*>::iterator itr = geoCollection.begin();itr!=geoCollection.end();itr++){
								if ((*itr)->getType()==GEOMTYPE_3D_REFERENCE)
								{
									Reference3d *pRef = 0;//(C3DReference*)(*itr);
									if((*itr)->queryInterface(GEOMTYPE_3D_REFERENCE,(void**)&pRef)){
										MDLID mdlID = pRef->getSharedModelID();
										SharedModelSharedPtr pLinkModel = _proxySptr->getSharedModel(mdlID);
										pRef->setSharedModel(pLinkModel);
									}
								}
							}
						}
					}
					pGeom->translateFrom(m4x4);
				}
				this->deleteTopo((*it3)->getID(),TOPOINFOTYPE_SEMATIC_GEOMETRY);
				this->addTopo(tip);
			}
		}
		save();
	}
	save();
}

end_gdb_namespace
end_gtl_namespace

