#include "losp.h"
#include "FindFile.h"

#ifndef DEDICATED_ONLY

LOSPFontLoader LOSPFontLoader::instance;

bool LOSPFontLoader::canLoad(std::string const& path, std::string& name)
{
	if(GetFileExtensionWithDot(path) == ".lfn")
	{
		name = GetBaseFilenameWithoutExt(path);
		return true;
	}
	return false;
}

/*
Font format:
	INT32 surfaceWidth
	INT32 surfaceHeight
	
	STRUCT Dimensions
		INT32 x
		INT32 y
		INT32 width
		INT32 height
	END
	
	Dimensions chars[224]
	UINT8      surface[surfaceWidth * surfaceHeight]
*/
	
bool LOSPFontLoader::load(Font* font, std::string const& path)
{
	font->free();

	std::ifstream f;
	OpenGameFileR(f, path, std::ios::binary);
	if(!f)
		return false;
		
	long bitmapWidth = 0, bitmapHeight = 0;
	f.read((char *)&bitmapWidth, 4);
	f.read((char *)&bitmapHeight, 4);
		
	if(!f)
		return false;
		
	bool full = false;
		
	if(bitmapWidth < 0)
	{
		bitmapWidth = -bitmapWidth;
		full = true;
	}

	font->m_bitmap = create_bitmap_ex(8, (int)bitmapWidth, (int)bitmapHeight);
	if(!font->m_bitmap)
		return false;
		
	font->m_supportColoring = true;
	
	int max = 224;
	if(full)
	{
		max = 256;
	}
	else
		font->m_chars.assign(32, Font::CharInfo(Rect(0, 0, 1, 1), 0));
	
	for(int i = 0; i < max; ++i)
	{
		int x, y, w, h;
		f.read((char *)&x, 4);
		f.read((char *)&y, 4);
		f.read((char *)&w, 4);
		f.read((char *)&h, 4);
		
		if(!f)
			return false;

		font->m_chars.push_back(Font::CharInfo(Rect(x, y, x + w, y + h), 0));
	}
	
	if(!full)
		font->m_chars[11] = font->m_chars[(unsigned char)'^'];
	
	for(int y = 0; y < bitmapWidth; ++y)
	{
		for(int x = 0; x < bitmapHeight; ++x)
		{
			char v;
			if(!f.get(v))
				return false;
				
			int c = (unsigned char)v;

			putpixel(font->m_bitmap, x, y, c);
		}
	}
	
	font->buildSubBitmaps();
	
	return true;
}

const char* LOSPFontLoader::getName()
{
	return "LOSP font loader";
}

std::string LOSPFontLoader::format() { return "LOSP font"; }
std::string LOSPFontLoader::formatShort() { return "LOSP"; }

#endif
