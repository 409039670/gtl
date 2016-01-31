#include "spatialreference.h" 
begin_gtl_namespace
begin_gdb_namespace
//derived from gdal 1.10
//������Ҫ�����ݷ�����gtl::getDataHome()���ص�Ŀ¼�£�����������ֵ�����ֻ��Ҫ�����ļ�������
//����ĸ�ʽ�μ�epsg.wkt,esri_extra.wkt,cubewerx_extra.wkt
//��Щ�ļ���ÿ�������ַ���<=1024*1024=1Mb
#define MAX_LINE_CHARS 1048576

bool SpatialReference::importFromDict(const char *pszDict, const char *pszCode)
{ 
	bool eErr = false;

	/* -------------------------------------------------------------------- */
	/*      open file.                                             */
	/* -------------------------------------------------------------------- */ 
	std::locale _loc = std::locale::global(std::locale(""));
	string sz = gtl::getDataHome()+"\\";
	sz = sz + pszDict;
	std::ifstream iFstreamDict(sz.c_str(), std::ios_base::in);
	/* -------------------------------------------------------------------- */
	/*      Process lines.                                                  */
	/* -------------------------------------------------------------------- */
	char pszLine[MAX_LINE_CHARS];
	
	
	while (!iFstreamDict.eof())
	{
		iFstreamDict.getline(pszLine, MAX_LINE_CHARS);
		if (pszLine[0] == '#') {
			/*ע���У� do nothing */
			continue;
		}
		else if (cslNIEqualString(pszLine, "include ", 8))
		{
			eErr = importFromDict(pszLine + 8, pszCode);
			if (eErr != false)
				break;
		}
		else if (strstr(pszLine, ",") == NULL) {
			/* do nothing */
			continue;
		}
		else if (cslNIEqualString(pszLine, pszCode, strlen(pszCode))
			&& pszLine[strlen(pszCode)] == ',')
		{
			char *pszWKT = (char *)pszLine + strlen(pszCode) + 1;

			eErr = importFromWkt(&pszWKT);
			break;
		}
	}

	/* -------------------------------------------------------------------- */
	/*      Cleanup                                                         */
	/* -------------------------------------------------------------------- */
	iFstreamDict.close();
	return eErr;
}
end_gdb_namespace
end_gtl_namespace