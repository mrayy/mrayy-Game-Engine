#include "stdafx.h"
#include "CPNGLoader.h"
#include "IFileSystem.h"
#include <libpng/png.h>
#include "ILogManager.h"
#include "TextureResourceManager.h"
#include "StringConverter.h"
#include "common.h"
#include "TraceManager.h"
#include "ITexture.h"
#include "StringUtil.h"

namespace mray{
namespace loaders{

	/* Mipmapping parameters */
#define PNG_NOMIPMAPS      0 /* No mipmapping                        */
#define PNG_BUILDMIPMAPS  -1 /* Calls a clone of gluBuild2DMipmaps() */
#define PNG_SIMPLEMIPMAPS -2 /* Generates mipmaps without filtering  */

	/* Who needs an "S" anyway? */
#define PNG_NOMIPMAP     PNG_NOMIPMAPS
#define PNG_BUILDMIPMAP  PNG_BUILDMIPMAPS
#define PNG_SIMPLEMIPMAP PNG_SIMPLEMIPMAPS

	/* Transparency parameters */
#define PNG_CALLBACK  -3 /* Call the callback function to generate Alpha   */
#define PNG_ALPHA     -2 /* Use Alpha channel in PNG file, if there is one */
#define PNG_SOLID     -1 /* No transparency                                */
#define PNG_STENCIL    0 /* Sets Alpha to 0 for r=g=b=0, 1 otherwise       */
#define PNG_BLEND1     1 /* a = r+g+b                                      */
#define PNG_BLEND2     2 /* a = (r+g+b)/2                                  */
#define PNG_BLEND3     3 /* a = (r+g+b)/3                                  */
#define PNG_BLEND4     4 /* a = r*r+g*g+b*b                                */
#define PNG_BLEND5     5 /* a = (r*r+g*g+b*b)/2                            */
#define PNG_BLEND6     6 /* a = (r*r+g*g+b*b)/3                            */
#define PNG_BLEND7     7 /* a = (r*r+g*g+b*b)/4                            */
#define PNG_BLEND8     8 /* a = sqrt(r*r+g*g+b*b)                          */

CPNGLoader::CPNGLoader(){
#ifdef ___DEBUG___
	setDebugName("CPNGLoader");
#endif
}

static void png_cpexcept_error(png_structp png_ptr, png_const_charp msg)
{
	gLogManager.log(mT("PNG FATAL ERROR"),core::StringConverter::toString(msg),ELL_WARNING);
	longjmp(png_ptr->jmpbuf, 1);
}
// PNG function for file reading
void PNGAPI user_read_data_fcn(png_structp png_ptr, png_bytep data, png_size_t length)
{
	png_size_t check;

	// changed by zola {
	OS::IStreamPtr file=(OS::IStream*)png_ptr->io_ptr;
	check=(png_size_t) file->read((void*)data,length);
	// }

	if (check != length)
		png_error(png_ptr, "Read Error");
}

bool CPNGLoader::load(OS::IStream* file,video::ImageInfo* image,video::ETextureType target){

	traceFunction(eEngine);

	if(!image || target!=video::ETT_2DTex)
		return false;


	rowData = 0;

	uchar buffer[8];
	// Read the first few bytes of the PNG file
	if( file->read(buffer, 8) != 8 )
	{
		gLogManager.log(mT("CPNGLoader::load(): can't read png file- "), file->getStreamName(),ELL_WARNING);
		return 0;
	}

	// Check if it really is a PNG file
	if( png_sig_cmp(buffer, 0, 8) )
	{
		gLogManager.log(mT("CPNGLoader::load(): not a png file- "), file->getStreamName(),ELL_WARNING);
		return 0;
	}

	// Allocate the png read struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
		NULL, (png_error_ptr)png_cpexcept_error, NULL);
	if (!png_ptr)
	{
		gLogManager.log(mT("CPNGLoader::load(): Internal PNG create read struct failure- "), file->getStreamName(),ELL_WARNING);
		return 0;
	}

	// Allocate the png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		gLogManager.log(mT("CPNGLoader::load(): Internal PNG create info struct failure- "), file->getStreamName(),ELL_WARNING);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return 0;
	}

	// for proper error handling
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		if (rowData)
			delete [] rowData;
		return 0;
	}

	// changed by zola so we don't need to have public FILE pointers
	png_set_read_fn(png_ptr, file, user_read_data_fcn);

	png_set_sig_bytes(png_ptr, 8); // Tell png that we read the signature

	png_read_info(png_ptr, info_ptr); // Read the info section of the png file

	// Extract info
	png_get_IHDR(png_ptr, info_ptr,
		(png_uint_32*)&Width, (png_uint_32*)&Height,
		&BitDepth, &ColorType, NULL, NULL, NULL);

	// Convert palette color to true color
	if (ColorType==PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);

	// Convert low bit colors to 8 bit colors
	if (BitDepth < 8)
	{
		if (ColorType==PNG_COLOR_TYPE_GRAY || ColorType==PNG_COLOR_TYPE_GRAY_ALPHA)
			png_set_gray_1_2_4_to_8(png_ptr);
		else
			png_set_packing(png_ptr);
	}

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);

	// Convert high bit colors to 8 bit colors
	if (BitDepth == 16)
		png_set_strip_16(png_ptr);

	// Convert gray color to true color
	if (ColorType==PNG_COLOR_TYPE_GRAY || ColorType==PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png_ptr);

	// Update the changes
	png_read_update_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr,
		(png_uint_32*)&Width, (png_uint_32*)&Height,
		&BitDepth, &ColorType, NULL, NULL, NULL);
/*
	// Convert RGBA to BGRA
	if (ColorType==PNG_COLOR_TYPE_RGB_ALPHA)
	{
#ifdef __BIG_ENDIAN__
		png_set_swap_Alpha(png_ptr);
#else
		png_set_bgr(png_ptr);
#endif
	}
*/
	// Update the changes
	png_get_IHDR(png_ptr, info_ptr,
		(png_uint_32*)&Width, (png_uint_32*)&Height,
		&BitDepth, &ColorType, NULL, NULL, NULL);

	// Create the image structure to be filled by png data

//	Image->setResourceName(file->getStreamName());

	// Create array of pointers to rows in image data
	rowData = new png_bytep[Height];
	if (!rowData)
	{
		gLogManager.log(mT("CPNGLoader::load(): Internal PNG create row pointers failure- "), file->getStreamName(),ELL_WARNING);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return 0;
	}

	if (ColorType==PNG_COLOR_TYPE_RGB_ALPHA){
		image->createData(math::vector3di(Width,Height,1),video::EPixel_R8G8B8A8);
	}
	else
		image->createData(math::vector3di(Width,Height,1),video::EPixel_R8G8B8);
	if (!image->imageData)
	{
		gLogManager.log(mT("CPNGLoader::load(): Internal PNG create image struct failure- "), file->getStreamName(),ELL_WARNING);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return 0;
	}


	// Fill array of pointers to rows in image data
	unsigned char* data = (unsigned char*)image->imageData;
	for (uint i=Height; i>0; --i)
	{
		rowData[i-1]=data;
		data += image->getPitch();
	}

	// for proper error handling
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		delete [] rowData;
		return 0;
	}

	// Read data using the library function that handles all transformations including interlacing
	png_read_image(png_ptr, rowData);

	png_read_end(png_ptr, NULL);
	delete [] rowData;
	png_destroy_read_struct(&png_ptr,&info_ptr, 0); // Clean up memory

	return 1;

}

bool CPNGLoader::load(const  core::string&filename,video::ImageInfo* image,video::ETextureType target){
	traceFunction(eEngine);
 /* pngRawInfo info;

  video::ITexturePtr tex=new video::ITexture(device);

  char temp[512];
  core:: mcharo_char(temp,filename);
  if(!pngLoadRaw(temp, &info))
    return 0;

  tex->colorDepth=info.Components*8;
  tex->imageSize.x=info.Width;
  tex->imageSize.y=info.Height;
  tex->imageData=info.Data;
  tex->imageDataFormat=info.Components == 4 ? video::EPixel_R8G8B8A8 : info.Components == 3 ? video::EPixel_R8G8B8 : video::EPixel_LUMINANCE8;
  tex->imageDataType=video::EPixel_DATA_UNSIGNED_BYTE;

  tex->unlock(1);

  delete info.Palette;
*/
	OS::IStreamPtr fp=gFileSystem.openFile(filename,OS::BIN_READ);
	if(!fp){
		gLogManager.log(mT("Couldn't open PNG file"),filename,ELL_WARNING);
		return 0;
	}

	return load(fp,image,target);
}

bool CPNGLoader::canLoad(const core::string&name){
	core::string str=core::StringUtil::ToLower(name);
	return strSearch(str.c_str(),mT(".png"))!=0;
}
bool CPNGLoader::canLoad(OS::IStream* file)
{
	png_byte buff[8];
	file->seek(0,OS::ESeek_Set);
	if(file->read(&buff,8*sizeof(png_byte))!=8)return 0;
	return !png_sig_cmp(buff,0,8);
}



const core::string g_sPNGExt=mT("png");
const core::string g_sPNGDesc=mT("PNG Image Loader");

const core::string& CPNGLoader::getExt(){
	return g_sPNGExt;
}

const core::string& CPNGLoader::getDescription(){
	return g_sPNGDesc;
}

bool CPNGLoader::support(video::ETextureType type){
	return type==video::ETT_2DTex;
}


}
}
