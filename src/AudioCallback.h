#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>
#include <endpointvolume.h>


class AudioCallback :
    public IAudioEndpointVolumeCallback
{
public:
    void SetCallback(
    void (*callback)(float))
{
    volumeChanged = callback;
}


    ULONG STDMETHODCALLTYPE AddRef() override;

    ULONG STDMETHODCALLTYPE Release() override;

    HRESULT STDMETHODCALLTYPE QueryInterface(
        REFIID riid,
        void** ppvObject) override;


    HRESULT STDMETHODCALLTYPE OnNotify(
        PAUDIO_VOLUME_NOTIFICATION_DATA data) override;


private:

    LONG refCount = 1;

    void (*volumeChanged)(float) = nullptr;
};