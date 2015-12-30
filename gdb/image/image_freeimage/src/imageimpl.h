#pragma once
#include "image.h"
#include "../inc/freeimage.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

class ImageImpl :	public cn::edu::cug::gdb::Image
{
protected:
	/// DIB data
	FIBITMAP *_dib;
	/// Original (or last saved) fif format if available, FIF_UNKNOWN otherwise
	FREE_IMAGE_FORMAT _fif;
	/// TRUE whenever the display need to be refreshed
	mutable BOOL _bHasChanged;
protected:
	bool  replace(FIBITMAP *new_dib);
	bool  writeDDSBuffer(Buffer & buf);
public:
	ImageImpl(void);
	ImageImpl(unsigned w, unsigned h, unsigned bpp, void * pdata);
	virtual ~ImageImpl(void);
	/** 
	* @param [in] lpszPathName
	* @param [in] flag
	* @return ִ�гɹ�����true�����򷵻�false
	*/
	virtual bool load(const char* lpszPathName,int flag=0 );
	/** 
	* @param [in] filename
	* @param [in] flags
	* @return ִ�гɹ�����true�����򷵻�false
	*/
	virtual bool save( const char *filename,int flag=0 );

	virtual void  clear() ;

	/**
	����Ӱ�����ؿ��
	@see FreeImage_GetWidth
	*/
	virtual unsigned int getWidth() const;
	
	/**
	����Ӱ�����ظ߶�
	@see FreeImage_GetHeight
	*/
	virtual unsigned int getHeight() const;
	
	/**
	����ɨ��������뷽ʽ���ֽڿ��
	@see FreeImage_GetPitch
	*/
	virtual unsigned int getScanWidth() const;

	/**
	����λͼ���ֽڿ��.<br>
	<b>����scanline���Ȳ�һ��</b>.
	@�鿴 FreeImage_GetLine, getScanWidth
	*/
	virtual unsigned char * getScanLine(unsigned int line) const;

	/**
	����X����ķֱ���,��λ������/����
	@see FreeImage_GetDotsPerMeterX
	*/
	virtual double getHorizontalResolution() const;
	
	/**
	����Y����ķֱ���,��λ������/����
	@see FreeImage_GetDotsPerMeterY
	*/
	virtual double getVerticalResolution() const;
	

	/** ��Ӱ��������ڴ棬ִ�гɹ�����true�����򷵻�false*/
	virtual bool isValid() const;
	 
	/**
	����Ӱ���ֽ����. <br>
	��Ӱ������Ϊ FIT_BITMAP, ��Ч�ֽ���ȿ����� 1, 4, 8, 16, 24 �� 32.
	@see FreeImage_GetBPP, getImageType
	*/
	virtual unsigned int getBitsPerPixel() const;
	
	/**��ȡָ�����е�������ɫ
	* @param [in] x        ָ��x�᷽��ĵ�x������
	* @param [in] y        ָ��y�᷽��ĵ�y������
	* @param [in,out] c    ָ�����������������ɫ
	* @return ִ�гɹ�����true�����򷵻ؿյ�false
	*/
	virtual bool getPixel(unsigned int x, unsigned int y, Color4f & c);

	/**��ȡ���е�������ɫֵ
	* @param [in,out] data   ָ�����������Buffer
	* @return ִ�гɹ�����true�����򷵻ؿյ�false
	*/
	virtual bool getPixels(Buffer & data);
	/**��ȡ���е�������ɫֵ
	* @return ִ�гɹ���������ֵ�׵�ַ�����򷵻�0
	*/
	virtual unsigned char * getPixels( ) const ;
	/**
	* @param [in] left   ָ�����ƾ����������
	* @param [in] top    ָ�����ƾ����ϱ�����
	* @param [in] right  ָ�����ƾ����ұ�����
	* @param [in] bottom ָ�����ƾ����±�����
	* @return ִ�гɹ�����ImageSharedPtr�����򷵻ؿյ�ImageSharedPtr
	*/
	virtual ImageSharedPtr copy( int  left, int top, int right, int bottom)const ;
	/**
	* @param [in] left ָ��ճ������ߵ�����
	* @param [in] top  ָ��ճ�����ϱߵ�����
	* @param [in] alpha Alpha �ں�
	* @return ִ�гɹ�����true�����򷵻ؿյ�false
	*/
	virtual bool paste(const ImageSharedPtr & src, int left, int top, int alpha = 256);
	/**
	* @param [in] left   ָ���ü������������
	* @param [in] top    ָ���ü������ϱ�����
	* @param [in] right  ָ���ü������ұ�����
	* @param [in] bottom ָ���ü������±�����
	* @return ִ�гɹ�����ImageSharedPtr�����򷵻ؿյ�ImageSharedPtr
	*/
	virtual bool crop(int  left, int top, int right, int bottom) ;
	/**
	* @param [in] new_width   ָ���ز����Ŀ��
	* @param [in] new_height  ָ���ز����ĸ߶�
	* @param [in] filter      ָ���ز����ķ���
	* @return ִ�гɹ�����true�����򷵻ؿյ�false
	*/
	virtual bool rescale(unsigned new_width, unsigned new_height, ImageFilter& filter);
	/**
	* @param [in] width   ָ��������Ӱ��Ŀ��
	* @param [in] height  ָ��������Ӱ��߶�
	* @param [in] bpp         ָ��������Ӱ��bpp
	* @return ִ�гɹ�����true�����򷵻ؿյ�false
	*/
	virtual bool reset(unsigned width, unsigned height, unsigned bpp);
	/**
	* @param [in] x       ָ��Ҫ�趨Ӱ���x
	* @param [in] y       ָ��Ҫ�趨Ӱ���y
	* @param [in] value   ָ��Ҫ�趨Ӱ���value
	* @return ִ�гɹ�����true�����򷵻ؿյ�false
	*/
	virtual bool setPixelColor(unsigned x, unsigned y, unsigned char *value);

	/**��ͼ��p�滻��ͼ������ݣ��滻��p�е����ݽ�ת�Ƶ���ͼ���У�p��Ϊһ����ͼ��
	* @param [in] p        Դͼ����ָ��
	* @return ִ�гɹ�����true�����򷵻ؿյ�false
	*/
	virtual bool transfer(const ImageSharedPtr & p);
	/**��ͼ��д��Buffer���Դ˷ֱ�Ϊunsinged int ��width,height,bpp��width*height*bpp/8���ַ�����
	* @param [in,out] buf      ������
	* @return ִ�гɹ�����true�����򷵻ؿյ�false
	*/
	virtual bool write(Buffer& buf);
	/**��ͼ���ÿգ���Buffer�ֱ��ȡunsinged int���͵�width,height,bpp��width*height*bpp/8���ַ����飬�����µ�ͼ��
	* @param [in,out] buf      ������
	* @return ִ�гɹ�����true�����򷵻ؿյ�false
	*/
	virtual bool read(const Buffer& buf);

	virtual bool write(int imageFormat, Buffer & fout, int flags=0);

	virtual int getFormat() const ;

	virtual int getType() const ;
};

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace


#include "factory.h"
class ImageFactoryImpl:public cn::edu::cug::gdb::ImageFactory
{
public:
	ImageFactoryImpl(void);
	virtual ~ImageFactoryImpl(void);
	virtual  cn::edu::cug::gdb::ImageSharedPtr newImage( );
	virtual  cn::edu::cug::gdb::ImageSharedPtr newImage(unsigned w, unsigned h, unsigned bpp, void * pdata );
};

struct ImageFactoryDeletor{
	void operator ()(cn::edu::cug::gdb::Factory * p){
		delete (ImageFactoryImpl*)((cn::edu::cug::gdb::ImageFactory* )p);
	}
};