#pragma once

#include "api\media_stream_interface.h"
#include "media\base\video_broadcaster.h"
#include "media\base\video_adapter.h"
#include "modules\video_capture\video_capture.h"
#include "modules\video_capture\video_capture_factory.h"
#include "pc\video_track_source.h"
#include "media\engine\webrtc_media_engine.h"

#include "api\audio_codecs\audio_encoder_factory_template.h"
#include "api\audio_codecs\opus\audio_encoder_opus.h"
#include "api\audio_codecs\audio_decoder_factory_template.h"
#include "api\audio_codecs\opus\audio_decoder_opus.h"

#include "api\video_codecs\builtin_video_encoder_factory.h"
#include "api\video_codecs\builtin_video_decoder_factory.h"

#include "api\task_queue\default_task_queue_factory.h"
#include "api\create_peerconnection_factory.h"

#include <memory>

class Render;

class UserMedia
{
public:
    static UserMedia Create(bool video, bool audio);
    std::unique_ptr<std::vector<webrtc::VideoTrackInterface*>> get_video_tracks();
    std::unique_ptr<std::vector<webrtc::AudioTrackInterface*>> get_audio_tracks();
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> _pc;
    std::vector<webrtc::AudioTrackInterface*> audio_tracks;
    std::vector<webrtc::VideoTrackInterface*> video_tracks;
    void set_rend(Render& rend);
private:

};

std::unique_ptr<UserMedia> create_user_media(bool video, bool audio);