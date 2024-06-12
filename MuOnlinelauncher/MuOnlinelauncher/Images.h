#pragma once
#include <map>
#include <string>
#include <memory>
#include <gdiplus.h>
#include <io.h> // Para _waccess

using namespace Gdiplus;

enum LAUNCHER_IMAGES {
    LAUNCHER_IMAGE_BACKGROUND,
    SETTINGS_IMAGE_BACKGROUND,
};

class CImages
{
public:
    CImages(HINSTANCE hInstance);
    virtual ~CImages();
    void loadImages();
    Image* getImage(LAUNCHER_IMAGES image);
private:
    void loadImageFromResource(LAUNCHER_IMAGES id, int resourceID);
    HINSTANCE m_hInstance;
    std::map<LAUNCHER_IMAGES, std::unique_ptr<Image>> m_images;
};
