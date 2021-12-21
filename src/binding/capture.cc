
#include "libwebrtc-sys/include/capture.h"

Capture* Capture::Create(size_t width,
    size_t height,
    size_t target_fps,
    size_t capture_device_index)
{
    std::unique_ptr<Capture> vcm_capturer(new Capture());
    vcm_capturer->Init(width, height, target_fps, capture_device_index);
    return vcm_capturer.release();
}

bool Capture::Init(size_t width,
    size_t height,
    size_t target_fps,
    size_t capture_device_index)
{
    std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> device_info(
        webrtc::VideoCaptureFactory::CreateDeviceInfo());
    char device_name[256];
    char unique_name[256];
    if (device_info->GetDeviceName(static_cast<uint32_t>(capture_device_index),
        device_name, sizeof(device_name), unique_name,
        sizeof(unique_name)) != 0) {
        Destroy();
        return false;
    }
    vcm_ = webrtc::VideoCaptureFactory::Create(unique_name);
    if (!vcm_) {
        return false;
    }
    vcm_->RegisterCaptureDataCallback(this);
    device_info->GetCapability(vcm_->CurrentDeviceName(), 0, capability_);
    capability_.width = static_cast<int32_t>(width);
    capability_.height = static_cast<int32_t>(height);
    capability_.maxFPS = static_cast<int32_t>(target_fps);
    capability_.videoType = webrtc::VideoType::kI420;
    if (vcm_->StartCapture(capability_) != 0) {
        Destroy();
        return false;
    }
    RTC_CHECK(vcm_->CaptureStarted());
    return true;
}

void  Capture::Destroy()
{
    if (!vcm_)
        return;
    vcm_->StopCapture();
    vcm_->DeRegisterCaptureDataCallback();
    vcm_ = nullptr;
}

void  Capture::OnFrame(const webrtc::VideoFrame& frame)
{
    broadcaster_.OnFrame(frame);
}

rtc::VideoSinkWants  Capture::GetSinkWants() {
    return broadcaster_.wants();
}
void  Capture::RemoveSink(rtc::VideoSinkInterface<webrtc::VideoFrame>* sink) {
    broadcaster_.RemoveSink(sink);
    UpdateVideoAdapter();
}

void  Capture::AddOrUpdateSink(
    rtc::VideoSinkInterface<webrtc::VideoFrame>* sink,
    const rtc::VideoSinkWants& wants) {
    broadcaster_.AddOrUpdateSink(sink, wants);
    UpdateVideoAdapter();
}

void  Capture::UpdateVideoAdapter() {
    video_adapter_.OnSinkWants(broadcaster_.wants());
}

rtc::scoped_refptr<CapturerTrackSource> CapturerTrackSource::Create() {
        const size_t kWidth = 640;
        const size_t kHeight = 480;
        const size_t kFps = 30;
        std::unique_ptr<Capture> capturer;
        std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> info(
            webrtc::VideoCaptureFactory::CreateDeviceInfo());
        if (!info) {
            return nullptr;
        }
        int num_devices = info->NumberOfDevices();
        for (int i = 0; i < num_devices; ++i) {
            capturer = absl::WrapUnique(
                Capture::Create(kWidth, kHeight, kFps, i));
            if (capturer) {
                return new rtc::RefCountedObject<CapturerTrackSource>(
                    std::move(capturer));
            }
        }

        return nullptr;
    }

rtc::VideoSourceInterface<webrtc::VideoFrame>* CapturerTrackSource::source() {
        return capturer_.get();
    }
    std::unique_ptr<Capture> capturer_;

void test()
{
    auto cap = CapturerTrackSource::Create().release();
}