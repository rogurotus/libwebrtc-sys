#include "libwebrtc-sys/include/get_device.h"

#include <iostream>
#include <string>
#include <memory>
#include <vector>


using namespace webrtc;
using namespace std;


std::unique_ptr<std::vector<std::string>> video_device_names()
{
    std::unique_ptr<std::vector<std::string>> result = make_unique<vector<string>>();
    auto video = webrtc::VideoCaptureFactory::CreateDeviceInfo();
    int n = video->NumberOfDevices();
    char name[kVideoCaptureDeviceNameLength];
    char id[kVideoCaptureProductIdLength];


    for (int i = 0; i < n; ++i)
    {

        name[0] = '/0';
        id[0] = '/0';

        video->GetDeviceName(
            i,
            name,
            kVideoCaptureDeviceNameLength,
            id,
            kVideoCaptureProductIdLength);

        result->push_back(string(name));
    }
    return result;
}

std::unique_ptr<std::vector<std::string>> audio_input_device_names()
{
    auto result = make_unique<vector<string>>();
    auto audio = new webrtc::AudioDeviceWindowsCore();

    char name[webrtc::kAdmMaxDeviceNameSize];
    char id[webrtc::kAdmMaxGuidSize];
    int n = audio->RecordingDevices();

    for (int i = 0; i < n; ++i)
    {

        name[0] = '/0';
        id[0] = '/0';

        audio->RecordingDeviceName(
            i,
            name,
            id);

        result->push_back(string(name));
    }

    return result;

}

std::unique_ptr<std::vector<std::string>> audio_otput_device_names()
{
    auto result = make_unique<vector<string>>();
    auto audio = new webrtc::AudioDeviceWindowsCore();

    char name[webrtc::kAdmMaxDeviceNameSize];
    char id[webrtc::kAdmMaxGuidSize];
    int n = audio->PlayoutDevices();

    for (int i = 0; i < n; ++i)
    {

        name[0] = '/0';
        id[0] = '/0';

        audio->PlayoutDeviceName(
            i,
            name,
            id);

        result->push_back(string(name));
    }

    return result;

}
