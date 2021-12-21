#pragma once

#include <iostream>
#include "api\video_codecs\builtin_video_decoder_factory.h"
#include "api\video_codecs\builtin_video_encoder_factory.h"
#include "pc\video_track_source.h"
#include "media/base/video_broadcaster.h"
#include "media\base\video_adapter.h"
#include "modules\video_capture\video_capture.h"
#include "modules\video_capture\video_capture_factory.h"
#include "api/task_queue/default_task_queue_factory.h"
#include "api/create_peerconnection_factory.h"


#include "media/engine/webrtc_media_engine.h"
#include "api/audio_codecs/audio_encoder_factory_template.h"
#include "api/audio_codecs/audio_decoder_factory_template.h"
#include "api/audio_codecs/opus/audio_encoder_opus.h"
#include "api/audio_codecs/opus/audio_decoder_opus.h"
#include "api/rtc_event_log/rtc_event_log_factory.h"


#include "api/video_codecs/builtin_video_decoder_factory.h"
#include "api/video_codecs/builtin_video_encoder_factory.h"
#include "pc/video_track_source.h"
#include "media/base/video_broadcaster.h"
#include "media/base/video_adapter.h"
#include "modules/video_capture/video_capture.h"
#include "modules/video_capture/video_capture_factory.h"
#include "api/task_queue/default_task_queue_factory.h"
#include "api/create_peerconnection_factory.h"

class Capture : public rtc::VideoSinkInterface<webrtc::VideoFrame>, public rtc::VideoSourceInterface<webrtc::VideoFrame>
{
private:
    rtc::VideoBroadcaster broadcaster_;
    cricket::VideoAdapter video_adapter_;

public:
    rtc::scoped_refptr<webrtc::VideoCaptureModule> vcm_;
    webrtc::VideoCaptureCapability capability_;

    static Capture* Create(size_t width,
        size_t height,
        size_t target_fps,
        size_t capture_device_index);

    bool Init(size_t width,
        size_t height,
        size_t target_fps,
        size_t capture_device_index);

    void Destroy();

    void OnFrame(const webrtc::VideoFrame& frame);

    rtc::VideoSinkWants GetSinkWants();

    void RemoveSink(rtc::VideoSinkInterface<webrtc::VideoFrame>* sink);

    void AddOrUpdateSink(
        rtc::VideoSinkInterface<webrtc::VideoFrame>* sink,
        const rtc::VideoSinkWants& wants);

    void UpdateVideoAdapter();
};

class CapturerTrackSource : public webrtc::VideoTrackSource {
public:
    static rtc::scoped_refptr<CapturerTrackSource> Create();

protected:
    explicit CapturerTrackSource(
        std::unique_ptr<Capture> capturer)
        : VideoTrackSource(false), capturer_(std::move(capturer)) {}

private:
    rtc::VideoSourceInterface<webrtc::VideoFrame>* source();
    std::unique_ptr<Capture> capturer_;
};

void test();