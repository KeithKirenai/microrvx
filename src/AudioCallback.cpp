#include "AudioCallback.h"

ULONG AudioCallback::AddRef()
{
    return InterlockedIncrement(&refCount);
}


ULONG AudioCallback::Release()
{
    ULONG count = InterlockedDecrement(&refCount);

    if(!count)
        delete this;

    return count;
}


HRESULT AudioCallback::QueryInterface(
    REFIID riid,
    void** ppv)
{
    if(!ppv)
        return E_POINTER;


    if(riid == IID_IUnknown ||
       riid == __uuidof(IAudioEndpointVolumeCallback))
    {
        *ppv = static_cast<IAudioEndpointVolumeCallback*>(this);

        AddRef();

        return S_OK;
    }


    *ppv = nullptr;

    return E_NOINTERFACE;
}


HRESULT AudioCallback::OnNotify(
    PAUDIO_VOLUME_NOTIFICATION_DATA data)
{
    if(volumeChanged)
        volumeChanged(data->fMasterVolume);

    return S_OK;
}