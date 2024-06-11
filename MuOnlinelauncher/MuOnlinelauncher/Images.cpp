#include "pch.h"
#include "Images.h"
#include "resource.h"

CImages::CImages(HINSTANCE hInstance): m_hInstance(hInstance)
{
}

CImages::~CImages()
{
}

void CImages::loadImages()
{

    loadImageFromResource(LAUNCHER_IMAGE_BACKGROUND, IDB_LAUNCHER_BG);
}

void CImages::loadImageFromResource(LAUNCHER_IMAGES id, int resourceID)
{
    HRSRC hResource = FindResource(m_hInstance, MAKEINTRESOURCE(resourceID), L"PNG");
    if (hResource)
    {
        HGLOBAL hMemory = LoadResource(m_hInstance, hResource);
        if (hMemory)
        {
            void* pResourceData = LockResource(hMemory);
            DWORD size = SizeofResource(m_hInstance, hResource);
            HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, size);
            if (hBuffer)
            {
                void* pBuffer = GlobalLock(hBuffer);
                if (pBuffer)
                {
                    CopyMemory(pBuffer, pResourceData, size);
                    IStream* pStream = nullptr;
                    if (CreateStreamOnHGlobal(hBuffer, TRUE, &pStream) == S_OK)
                    {
                        auto image = std::make_unique<Bitmap>(pStream, FALSE);
                        if (image->GetLastStatus() == Ok)
                        {
                            std::wcerr << L"Image loaded successfully from resources" << std::endl;
                            m_images[id] = std::move(image);
                        }
                        else
                        {
                            std::wcerr << L"Failed to load image from resources: Status code " << image->GetLastStatus() << std::endl;
                        }
                        pStream->Release();
                    }
                    GlobalUnlock(hBuffer);
                }
                GlobalFree(hBuffer);
            }
        }
        FreeResource(hMemory);
    }
    else
    {
        std::wcerr << L"Failed to find image resource" << std::endl;
    }
}

Image* CImages::getImage(LAUNCHER_IMAGES image)
{
    auto it = m_images.find(image);
    if (it != m_images.end())
    {
        return it->second.get();
    }
    return nullptr;
}
