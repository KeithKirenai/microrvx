#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>

#include <mmdeviceapi.h>

#include <endpointvolume.h>

#include "Window.h"

#include "AudioCallback.h"

constexpr int HOTKEY_UP_ID = 1;
constexpr int HOTKEY_DOWN_ID = 2;

constexpr UINT HOTKEY_MODS = MOD_CONTROL | MOD_SHIFT;

constexpr UINT HOTKEY_UP = VK_ADD;
constexpr UINT HOTKEY_DOWN = VK_SUBTRACT;

OSDWindow * g_osd = nullptr;
IAudioEndpointVolume * g_volume = nullptr;

void VolumeChanged(float volume) {
  if (g_osd) {
    int percent = static_cast<int>(volume * 100.0f);

    g_osd -> Show(percent);
  }
}

void ChangeVolume(float amount) {
  if (!g_volume)
    return;

  float current = 0.0f;

  g_volume -> GetMasterVolumeLevelScalar( &
    current);

  current += amount;

  if (current < 0.0f)
    current = 0.0f;

  if (current > 1.0f)
    current = 1.0f;

  g_volume -> SetMasterVolumeLevelScalar(
    current,
    nullptr);
}

int WINAPI WinMain(
  HINSTANCE instance,
  HINSTANCE,
  LPSTR,
  int) {

  CoInitializeEx(
    nullptr,
    COINIT_MULTITHREADED);

  OSDWindow osd;

  if (!osd.Create(instance))
    return 0;

RegisterHotKey(
    nullptr,
    HOTKEY_UP_ID,
    HOTKEY_MODS,
    HOTKEY_UP);

RegisterHotKey(
    nullptr,
    HOTKEY_DOWN_ID,
    HOTKEY_MODS,
    HOTKEY_DOWN);

  g_osd = & osd;

  IMMDeviceEnumerator * enumerator = nullptr;

  CoCreateInstance(
    __uuidof(MMDeviceEnumerator),
    nullptr,
    CLSCTX_ALL,
    __uuidof(IMMDeviceEnumerator),
    (void ** ) & enumerator);
	
	if(!enumerator)
{
    CoUninitialize();
    return 0;
}

  IMMDevice * device = nullptr;

  enumerator -> GetDefaultAudioEndpoint(
    eRender,
    eMultimedia, &
    device);
	
	if(!device)
{
    enumerator->Release();
    CoUninitialize();
    return 0;
}

  IAudioEndpointVolume * volume = nullptr;

  device -> Activate(
    __uuidof(IAudioEndpointVolume),
    CLSCTX_ALL,
    nullptr,
    (void ** ) & volume);
	
	if(!volume)
{
    device->Release();
    enumerator->Release();
    CoUninitialize();
    return 0;
}

  g_volume = volume;

  AudioCallback * callback =
    new AudioCallback();

  callback -> SetCallback(
    VolumeChanged);

  volume -> RegisterControlChangeNotify(
    callback);

  MSG msg;

  while (GetMessage( &
      msg,
      nullptr,
      0,
      0)) {

    if (msg.message == WM_HOTKEY) {
      if (msg.wParam == HOTKEY_UP_ID) {
        ChangeVolume(0.05f);
      }

      if (msg.wParam == HOTKEY_DOWN_ID) {
        ChangeVolume(-0.05f);
      }

      continue;
    }

    TranslateMessage( & msg);

    DispatchMessage( & msg);
  }

  if (volume) {
    volume -> UnregisterControlChangeNotify(
      callback);

    volume -> Release();
  }

  if (callback) {
    callback -> Release();
  }

  if (device) {
    device -> Release();
  }

  if (enumerator) {
    enumerator -> Release();
  }

  UnregisterHotKey(
    nullptr,
    HOTKEY_UP_ID);

  UnregisterHotKey(
    nullptr,
    HOTKEY_DOWN_ID);

  CoUninitialize();

  return 0;
}