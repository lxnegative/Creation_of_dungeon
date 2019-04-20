#include "ImageResource.h"
#include "ResourceManager.h"
#include <assert.h>

ImageResource::ImageResource(std::string fileName)
    : Resource(fileName)
    , _handle(NULL)
{
    _handle = LoadGraph(fileName.c_str());

	if (_handle == -1) {
		_handle = NULL;
	}

    int iWidth = 0;
    int iHeight = 0;
    GetGraphSize(_handle, &iWidth, &iHeight);
    _width = static_cast<double>(iWidth);
    _height = static_cast<double>(iHeight);
}

ImageResource::~ImageResource()
{
    DeleteGraph(_handle);
}