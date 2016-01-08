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
#include "buffer.h" 



begin_gtl_namespace
begin_gdb_namespace

class  Image;

/** @defgroup Image ImageFilter
*  @{
*/
class CORE_API ImageFilter{
public:
	enum IMAGE_FILTER {
		FILTER_BOX		  = 0,	// Box, pulse, Fourier window, 1st order (constant) b-spline
		FILTER_BICUBIC	  = 1,	// Mitchell & Netravali's two-param cubic filter
		FILTER_BILINEAR   = 2,	// Bilinear filter
		FILTER_BSPLINE	  = 3,	// 4th order (cubic) b-spline
		FILTER_CATMULLROM = 4,	// Catmull-Rom spline, Overhauser spline
		FILTER_LANCZOS3	  = 5	// Lanczos3 filter
	};
	ImageFilter(){ mytype = FILTER_BOX; }
	ImageFilter(int type){
		mytype = type;
	}
	 int getType(){
		return mytype;	
	}
protected:
	int mytype;
	friend class Image;
};
/** @} */

typedef std::shared_ptr<Image> ImageSharedPtr;

/** @defgroup Image Image-ͼ��ӿ�
*  @{
*/
class CORE_API Image{
public:
	enum{
		    IMAGE_FORMAT_UNKNOWN = -1,
			IMAGE_FORMAT_BMP		= 0,
			IMAGE_FORMAT_ICO		= 1,
			IMAGE_FORMAT_JPEG	= 2,
			IMAGE_FORMAT_JNG		= 3,
			IMAGE_FORMAT_KOALA	= 4,
			IMAGE_FORMAT_LBM		= 5,
			IMAGE_FORMAT_IFF = IMAGE_FORMAT_LBM,
			IMAGE_FORMAT_MNG		= 6,
			IMAGE_FORMAT_PBM		= 7,
			IMAGE_FORMAT_PBMRAW	= 8,
			IMAGE_FORMAT_PCD		= 9,
			IMAGE_FORMAT_PCX		= 10,
			IMAGE_FORMAT_PGM		= 11,
			IMAGE_FORMAT_PGMRAW	= 12,
			IMAGE_FORMAT_PNG		= 13,
			IMAGE_FORMAT_PPM		= 14,
			IMAGE_FORMAT_PPMRAW	= 15,
			IMAGE_FORMAT_RAS		= 16,
			IMAGE_FORMAT_TARGA	= 17,
			IMAGE_FORMAT_TIFF	= 18,
			IMAGE_FORMAT_WBMP	= 19,
			IMAGE_FORMAT_PSD		= 20,
			IMAGE_FORMAT_CUT		= 21,
			IMAGE_FORMAT_XBM		= 22,
			IMAGE_FORMAT_XPM		= 23,
			IMAGE_FORMAT_DDS		= 24,
			IMAGE_FORMAT_GIF     = 25,
			IMAGE_FORMAT_HDR		= 26,
			IMAGE_FORMAT_FAXG3	= 27,
			IMAGE_FORMAT_SGI		= 28,
			IMAGE_FORMAT_EXR		= 29,
			IMAGE_FORMAT_J2K		= 30,
			IMAGE_FORMAT_JP2		= 31,
			IMAGE_FORMAT_PFM		= 32,
			IMAGE_FORMAT_PICT	= 33,
			IMAGE_FORMAT_RAW		= 34
	};
	virtual int getFormat() const =0;
	/** Image type used in FreeImage.
	*/
	enum {
		    IMAGE_TYPE_UNKNOWN = 0,	// unknown type
			IMAGE_TYPE_BITMAP  = 1,	// standard image			: 1-, 4-, 8-, 16-, 24-, 32-bit
			IMAGE_TYPE_UINT16	= 2,	// array of unsigned short	: unsigned 16-bit
			IMAGE_TYPE_INT16	= 3,	// array of short			: signed 16-bit
			IMAGE_TYPE_UINT32	= 4,	// array of unsigned long	: unsigned 32-bit
			IMAGE_TYPE_INT32	= 5,	// array of long			: signed 32-bit
			IMAGE_TYPE_FLOAT	= 6,	// array of float			: 32-bit IEEE floating point
			IMAGE_TYPE_DOUBLE	= 7,	// array of double			: 64-bit IEEE floating point
			IMAGE_TYPE_COMPLEX	= 8,	// array of FICOMPLEX		: 2 x 64-bit IEEE floating point
			IMAGE_TYPE_RGB16	= 9,	// 48-bit RGB image			: 3 x 16-bit
			IMAGE_TYPE_RGBA16	= 10,	// 64-bit RGBA image		: 4 x 16-bit
			IMAGE_TYPE_RGBF	= 11,	// 96-bit RGB float image	: 3 x 32-bit IEEE floating point
			IMAGE_TYPE_RGBAF	= 12	// 128-bit RGBA float image	: 4 x 32-bit IEEE floating point
	};
	virtual int getType() const =0;
	
public:
	/** 
	* @param [in] lpszPathName
	* @param [in] flag
	* @return ִ�гɹ�����true�����򷵻�false
	*/
	virtual bool load(const char* lpszPathName,int flag=0)=0;
	/** 
	* @param [in] filename
	* @param [in] flags
	* @return ִ�гɹ�����true�����򷵻�false
	*/
	virtual bool save( const char *filename,int flag=0)=0;

	virtual void  clear() =0;

	/**
	����Ӱ�����ؿ��
	@�鿴 FreeImage_GetWidth
	*/
	virtual unsigned int getWidth() const=0;
	
	/**
	����Ӱ�����ظ߶�
	@�鿴 FreeImage_GetHeight
	*/
	virtual unsigned int getHeight() const=0;
	
	/**
	����ɨ��������뷽ʽ���ֽڿ��
	@�鿴 FreeImage_GetPitch
	*/
	virtual unsigned int getScanWidth() const=0;
	/**
	 @brief ����ָ��ɨ���е����ݵ���ʼ��ַ
	* @param [in] line ����ָ������
	* @return ִ�гɹ�����ָ��ɨ���е����ݵ���ʼ��ַ
	@�鿴 FreeImage_GetScanLine, FreeImage documentation
	*/
	virtual unsigned char * getScanLine(unsigned int line) const =0;

	/**
	����X����ķֱ���,��λ������/����
	@�鿴 FreeImage_GetDotsPerMeterX
	*/
	virtual double getHorizontalResolution() const =0;
	
	/**
	����Y����ķֱ���,��λ������/����
	@�鿴 FreeImage_GetDotsPerMeterY
	*/
	virtual double getVerticalResolution() const =0;
	

	/** ��Ӱ��������ڴ棬ִ�гɹ�����true�����򷵻�false*/
	virtual bool isValid() const=0;
	 
	/**
	����Ӱ���ֽ����. <br>
	��Ӱ������Ϊ FIT_BITMAP, ��Ч�ֽ���ȿ����� 1, 4, 8, 16, 24 �� 32.
	@�鿴 FreeImage_GetBPP, getImageType
	*/
	virtual unsigned int getBitsPerPixel() const=0;

	/**��ȡָ�����е�������ɫ
	* @param [in] x        ָ��x�᷽��ĵ�x������
	* @param [in] y        ָ��y�᷽��ĵ�y������
	* @param [in,out] c    ָ�����������������ɫ
	* @return ִ�гɹ�����true�����򷵻ؿյ�false
	*/
	virtual bool getPixel(unsigned int x, unsigned int y, Color4f & c)=0;

	/**��ȡ���е�������ɫֵ
	* @param [in,out] data   ָ�����������Buffer
	* @return ִ�гɹ�����true�����򷵻ؿյ�false
	*/
	virtual bool getPixels(Buffer & data)=0;
	/**��ȡ���е�������ɫֵ
	* @return ִ�гɹ���������ֵ�׵�ַ�����򷵻�0
	*/
	virtual unsigned char * getPixels( ) const =0;
	/**
	* @param [in] left   ָ�����ƾ����������
	* @param [in] top    ָ�����ƾ����ϱ�����
	* @param [in] right  ָ�����ƾ����ұ�����
	* @param [in] bottom ָ�����ƾ����±�����
	* @return ִ�гɹ�����ImageSharedPtr�����򷵻ؿյ�ImageSharedPtr
	*/
	virtual ImageSharedPtr copy( int  left, int top, int right, int bottom)const=0 ;
	/**
	* @param [in] left ָ��ճ������ߵ�����
	* @param [in] top  ָ��ճ�����ϱߵ�����
	* @param [in] alpha Alpha �ں�
	* @return ִ�гɹ�����true�����򷵻ؿյ�false
	*/
	virtual bool paste(const ImageSharedPtr & src, int left, int top, int alpha = 256)=0;
	/**
	* @param [in] left   ָ���ü������������
	* @param [in] top    ָ���ü������ϱ�����
	* @param [in] right  ָ���ü������ұ�����
	* @param [in] bottom ָ���ü������±�����
	* @return ִ�гɹ�����ImageSharedPtr�����򷵻ؿյ�ImageSharedPtr
	*/
	virtual bool crop(int  left, int top, int right, int bottom)=0 ;
	
	/**
	* @param [in] new_width   ָ���ز����Ŀ��
	* @param [in] new_height  ָ���ز����ĸ߶�
	* @param [in] filter      ָ���ز����ķ���
	* @return ִ�гɹ�����true�����򷵻ؿյ�false
	*/
	virtual bool rescale(unsigned new_width, unsigned new_height, ImageFilter& filter)=0;
	/**
	* @param [in] x       ָ��Ҫ�趨Ӱ���x
	* @param [in] y       ָ��Ҫ�趨Ӱ���y
	* @param [in] value   ָ��Ҫ�趨Ӱ���value
	* @return ִ�гɹ�����true�����򷵻ؿյ�false
	*/
	virtual bool setPixelColor(unsigned x, unsigned y, unsigned char *value)=0;
	/**����ͼ���С��BPP
	* @param [in] width        Ҫ�趨Ӱ��Ŀ��
	* @param [in] height       Ҫ�趨Ӱ��ĸ߶�
	* @param [in] bpp          Ӱ���BPPֵ
	* @return ִ�гɹ�����true�����򷵻ؿյ�false
	*/
	virtual bool reset(unsigned width, unsigned height, unsigned bpp)=0;

	/**��ͼ��p�滻��ͼ������ݣ��滻��p�е����ݽ�ת�Ƶ���ͼ���У�p��Ϊһ����ͼ��
	* @param [in] p        Դͼ����ָ��
	* @return ִ�гɹ�����true�����򷵻ؿյ�false
	*/
	virtual bool transfer(const ImageSharedPtr & p)=0;

	/**��ͼ��д��Buffer���Դ˷ֱ�Ϊunsinged int ��width,height,bpp��width*height*bpp/8���ַ�����
	* @param [in,out] buf      ������
	* @return ִ�гɹ�����true�����򷵻ؿյ�false
	*/
	virtual bool write(Buffer& buf)=0;
	/**��ͼ���ÿգ���Buffer�ֱ��ȡunsinged int���͵�width,height,bpp��width*height*bpp/8���ַ����飬�����µ�ͼ��
	* @param [in,out] buf      ������
	* @return ִ�гɹ�����true�����򷵻ؿյ�false
	*/
	virtual bool read(const Buffer& buf)=0;

	virtual bool write(int imageFormat, Buffer & fout, int flags=0)=0;
public:
	/** 
	* @return ��̬��������һ��Ӱ����ָ��ʵ��
	*/
	static ImageSharedPtr create();
	/**����һ����ͼ��
	* @param [in] width        Ҫ�趨Ӱ��Ŀ��
	* @param [in] height       Ҫ�趨Ӱ��ĸ߶�
	* @param [in] bpp          Ӱ���BPPֵ
	* @param [in] pdata        �������ݣ��������У���СΪwidth*height*bpp/8���ֽ�
	* @return ͼ����ָ��
	*/
	static ImageSharedPtr create(unsigned width, unsigned height, unsigned bpp, void * pdata);
	/**����һ����ͼ��,��������p�е�����
	* @param [in] p        Դͼ��
	* @return ͼ����ָ��
	*/
	static ImageSharedPtr create(const ImageSharedPtr & p);

};
/** @} */ 


end_gdb_namespace
end_gtl_namespace



