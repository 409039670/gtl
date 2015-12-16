#pragma once

#include "config.h"
#include "buffer.h"


begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

/** ���û���ش�С,BYTE
* @param  [in] fpsize	Ҫ�ػ�����ֽ���
* @param  [in] mpsize	���ʻ�����ֽ���
* @param  [in] tpsize	��������ֽ���
* @param  [in] spsize	����ģ�ͻ�����ֽ���
*/
static std::vector<unsigned long long> cacheSizes;

string   Config::shaderPath = "h:\\dev\\sdk\\shader";

string   Config::imagePath = "h:\\dev\\sdk\\images";

Envelope3d Config::defaultQueryEnvelope=Envelope3d(-100,100,-100,100);
int  Config::textureFileModel=0;//	TextureReadModel //0����IO��С�����ļ���1����IO���������ļ���2�����ڴ�ӳ�������ļ�
int 	Config::featureFileModel=0; //1����IO����д��0����IO�ڴ�ӳ���ļ���д
double Config::unloadDistance =1000.0;
double Config::loadDistance =100.0;
double Config::fovy=45;
double Config::zNear=0.0001;
double Config::zFar=100000.0;

//ÿһ֡�ص����볡���е����Ҫ�ظ���
unsigned int  Config::numberOfAddFeaturesPreframe=5;
unsigned int  Config::cachedTexturesOfRender=3000;
unsigned int  Config::cachedSharedModelsOfRender=2000;

unsigned int Config::currentCommand =0;

unsigned int Config::versionFileDB=0;//�ļ����ݿ�汾
unsigned int Config::versionRDB=0;//��ϵ���ݿ�汾 

unsigned int Config::cachedBlockNumber=1000;// 

unsigned int Config::useSameBlockID4DomDem=0;// 

int Config::cameraLightStatus=1;

unsigned int Config::stereo = 0;
unsigned int Config::stereoMode = 0;

unsigned int Config::dispachMode = 1;

unsigned int  Config::sameID4DEMDOMBlock()
{
	return Config::useSameBlockID4DomDem;
}
unsigned int  Config::getFDBVersion()//��ȡ�ļ���汾
{
	return versionFileDB;
}
unsigned int  Config::getRDBVersion()//��ȡ���ݿ�汾
{
	return versionRDB;
}

unsigned int Config::getCurrentCommand(){
	return currentCommand;
}
void Config::setCurrentCommand(unsigned int cmd){
	currentCommand = cmd;
}
unsigned int  Config::beginPluginCommand()//��ȡ���в��������СCommandID
{
	return WM_USER+1000;
}
unsigned int  Config::endPluginCommand()//��ȡ���в���������CommandID
{
	return WM_USER+2000;;
}


int Config::getTextureFileModel(){
	return textureFileModel;
}
int Config::getFeatureFileModel(){
	return featureFileModel;
}
string   Config::getShaderPath() { return shaderPath;}

string   Config::getImagePath() { return imagePath;}

void   Config::setShaderPath(string sz ) {
	Config::shaderPath=sz;
}
void   Config::setImagePath(string  sz){
	Config::imagePath = sz;
}

Envelope3d&  Config::getDefaultQueryEnvelope(){
	return Config::defaultQueryEnvelope;
}
double Config::getUnloadDistance(){
	return Config::unloadDistance;
}
double Config::getLoadDistance()
{
	return Config::loadDistance;
}
unsigned int Config::getCachedTexturesOfRender() //��Ⱦ�����е��������С����λΪ�������
{
	return Config::cachedTexturesOfRender;
}
unsigned int Config::getCachedSharedModelsOfRender()//��Ⱦ�����еĹ���ģ�ͻ����С����λΪ�������
{
	return Config::cachedSharedModelsOfRender;
}
unsigned int Config::getCachedBlockNumber()//������������������
{
	return Config::cachedBlockNumber;
}
double Config::getFovy(){
	return Config::fovy;
}

double Config::getZNear()
{
	return Config::zNear;
}
double Config::getZFar()
{
	return Config::zFar;
}

int Config::getCameraLightStatus()
{
	return cameraLightStatus;
}

void Config::enableStereo(bool s){
	stereo = (s ? 1 : 0);
}
bool Config::isEnableStereo() {
	return stereo != 0;
}
unsigned int Config::getStereoMode()
{
	return stereoMode;
}
bool Config::isDynamicDispatching(){
	return dispachMode != 0;
}
Config::Config(){
	_loc = std::locale::global(std::locale(""));//���û���Ϊϵͳ����  
	char path[MAX_PATH];
	DWORD size = ::GetModuleFileName(NULL,path,MAX_PATH);
	string szModPath = string(path,size);
	size_t pos = szModPath.find_last_of("\\");
	string sz = szModPath.substr(0,pos);
	sz += "\\sde.cfg";
	ifstream iFstreamConfig;
	iFstreamConfig.open(sz.c_str(),std::ios_base::in|std::ios_base::_Nocreate);

	if (iFstreamConfig)
	{		
		iFstreamConfig.getline(path,MAX_PATH);
		char sz[MAX_PATH];	
		iFstreamConfig.getline(sz,2);
		if (sz[0]=='0')
		{
			Config::textureFileModel =0;
		}
		else if (sz[0] = '1')
		{
			Config::textureFileModel =1;
		}
		else
		{
			Config::textureFileModel =2;
		}
		iFstreamConfig.getline(path,MAX_PATH);
		iFstreamConfig.getline(sz,2);
		if (sz[0]=='0')
		{
			Config::featureFileModel =0; 
		}
		else if (sz[0] == '1')
		{
			Config::featureFileModel =1; 
		}
		
		int c[3];
		iFstreamConfig.getline(path,MAX_PATH);
		iFstreamConfig.getline(sz,MAX_PATH,',');
		c[0]=stringToNumber<int,char>(sz);
		iFstreamConfig.getline(sz,MAX_PATH,',');
		c[1]=stringToNumber<int,char>(sz);
		iFstreamConfig.getline(sz,MAX_PATH);
		c[2]=stringToNumber<int,char>(sz);
		Config::defaultQueryEnvelope = Envelope3d(0,c[0],0,c[1],0,c[2]);
		
		iFstreamConfig.getline(path,MAX_PATH);
		iFstreamConfig.getline(sz,MAX_PATH);
		Config::unloadDistance =stringToNumber<double,char>(sz);

		iFstreamConfig.getline(path,MAX_PATH);
		iFstreamConfig.getline(sz,MAX_PATH);
		Config::numberOfAddFeaturesPreframe =stringToNumber<unsigned int,char>(sz);

		iFstreamConfig.getline(path,MAX_PATH);
		iFstreamConfig.getline(sz,MAX_PATH);
		Config::cachedTexturesOfRender =stringToNumber<unsigned int,char>(sz);

		iFstreamConfig.getline(path,MAX_PATH);
		iFstreamConfig.getline(sz,MAX_PATH);
		Config::cachedSharedModelsOfRender =stringToNumber<unsigned int,char>(sz);

		iFstreamConfig.getline(path,MAX_PATH);
		iFstreamConfig.getline(sz,MAX_PATH);
		Config::cachedBlockNumber =stringToNumber<unsigned int,char>(sz);
		iFstreamConfig.getline(path,MAX_PATH);
		iFstreamConfig.getline(sz,MAX_PATH);
		Config::useSameBlockID4DomDem=stringToNumber<unsigned int,char>(sz);

		iFstreamConfig.getline(path,MAX_PATH);
		iFstreamConfig.getline(sz,MAX_PATH);
		Config::loadDistance=stringToNumber<double,char>(sz);

		iFstreamConfig.getline(path,MAX_PATH);
		iFstreamConfig.getline(sz,MAX_PATH);
		Config::fovy=stringToNumber<double,char>(sz);

		iFstreamConfig.getline(path,MAX_PATH);
		iFstreamConfig.getline(sz,MAX_PATH);
		Config::zNear=stringToNumber<double,char>(sz);

		iFstreamConfig.getline(path,MAX_PATH);
		iFstreamConfig.getline(sz,MAX_PATH);
		Config::zFar=stringToNumber<double,char>(sz);

		iFstreamConfig.getline(path, MAX_PATH);
		iFstreamConfig.getline(sz, MAX_PATH);
		Config::stereo = stringToNumber<unsigned int, char>(sz);

		iFstreamConfig.getline(path, MAX_PATH);
		iFstreamConfig.getline(sz, MAX_PATH);
		Config::stereoMode = stringToNumber<unsigned int, char>(sz);

		iFstreamConfig.getline(path, MAX_PATH);
		iFstreamConfig.getline(sz, MAX_PATH);
		Config::dispachMode = stringToNumber<unsigned int, char>(sz);

		iFstreamConfig.getline(path,MAX_PATH);
		iFstreamConfig.getline(sz,MAX_PATH);
		Config::cameraLightStatus=stringToNumber<int,char>(sz);
		 
		iFstreamConfig.getline(path, MAX_PATH);
		iFstreamConfig.getline(sz, MAX_PATH, ',');
		cacheSizes.push_back(stringToNumber<int, char>(sz));
		iFstreamConfig.getline(sz, MAX_PATH, ',');
		cacheSizes.push_back(stringToNumber<int, char>(sz));
		iFstreamConfig.getline(sz, MAX_PATH, ',');
		cacheSizes.push_back(stringToNumber<int, char>(sz));
		iFstreamConfig.getline(sz, MAX_PATH);
		cacheSizes.push_back(stringToNumber<int, char>(sz));
		for (auto it = cacheSizes.begin(); it != cacheSizes.end(); it++)
		{
			(*it) *=1048576;//from MB to BYTE
		}
	}

	iFstreamConfig.close();
	std::locale::global(_loc);//��ԭ����  
}


int Config::getNumberOfAddFeaturesPreframe(){
	return Config::numberOfAddFeaturesPreframe;
}

std::vector<unsigned long long> & Config::getCacheSizes() { return cacheSizes; }


static Config  g_Config;

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
