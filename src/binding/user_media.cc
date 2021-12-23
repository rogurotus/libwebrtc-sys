
#include "libwebrtc-sys/src/main.rs.h"

cricket::MediaEngineDependencies CreateSomeMediaDeps(
    webrtc::TaskQueueFactory *task_queue_factory)
{
    cricket::MediaEngineDependencies media_deps;
    media_deps.task_queue_factory = task_queue_factory;
    media_deps.adm = webrtc::AudioDeviceModule::CreateForTest(
        webrtc::AudioDeviceModule::kDummyAudio, task_queue_factory);

    media_deps.audio_encoder_factory =
        webrtc::CreateAudioEncoderFactory<webrtc::AudioEncoderOpus>();
    media_deps.audio_decoder_factory =
        webrtc::CreateAudioDecoderFactory<webrtc::AudioDecoderOpus>();

    media_deps.video_encoder_factory = webrtc::CreateBuiltinVideoEncoderFactory();
    media_deps.video_decoder_factory = webrtc::CreateBuiltinVideoDecoderFactory();
    
    media_deps.audio_processing = webrtc::AudioProcessingBuilder().Create();
    return media_deps;
}

class PeerObserver : public webrtc::PeerConnectionObserver
{
    void OnAddTrack(
        rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver,
        const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>> &
            streams) {}

    void OnRemoveTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver) {}

    void OnIceCandidate(const webrtc::IceCandidateInterface *candidate) {}

    void OnDataChannel(
        rtc::scoped_refptr<webrtc::DataChannelInterface> channel) {}
    void OnSignalingChange(
        webrtc::PeerConnectionInterface::SignalingState new_state) {}
    void OnIceGatheringChange(
        webrtc::PeerConnectionInterface::IceGatheringState new_state) {}
};

UserMedia UserMedia::Create(bool video, bool audio)
{
    UserMedia result;
    auto task_queue_factory = webrtc::CreateDefaultTaskQueueFactory();
    auto media_deps = CreateSomeMediaDeps(task_queue_factory.get());

    auto peer_connection_factory = webrtc::CreatePeerConnectionFactory(
        rtc::Thread::Current(), rtc::Thread::Current(), rtc::Thread::Current(),
        std::move(media_deps.adm), std::move(media_deps.audio_encoder_factory),
        std::move(media_deps.audio_decoder_factory),
        std::move(media_deps.video_encoder_factory),
        std::move(media_deps.video_decoder_factory), nullptr, nullptr);

    auto cts = CapturerTrackSource::Create();

    rtc::scoped_refptr<webrtc::AudioTrackInterface> audio_track(
        peer_connection_factory->CreateAudioTrack(
            "label_a", peer_connection_factory->CreateAudioSource(
                           cricket::AudioOptions())));

    auto video_track = peer_connection_factory->CreateVideoTrack("label_v", cts);

    if (audio)
    {
        result.audio_tracks.push_back(audio_track.release());
    }
    if (video)
    {
        result.video_tracks.push_back(video_track.release());
    }
    return result;
}

std::unique_ptr<std::vector<webrtc::VideoTrackInterface *>> UserMedia::get_video_tracks()
{
    return std::make_unique<std::vector<webrtc::VideoTrackInterface *>>(video_tracks);
}
std::unique_ptr<std::vector<webrtc::AudioTrackInterface *>> UserMedia::get_audio_tracks()
{
    return std::make_unique<std::vector<webrtc::AudioTrackInterface *>>(audio_tracks);
}

std::unique_ptr<UserMedia> create_user_media(bool video, bool audio)
{
    auto um = UserMedia::Create(video, audio);
    return std::make_unique<UserMedia>(um);
}

void UserMedia::set_rend(Render &rend)
{
    video_tracks[0]->AddOrUpdateSink(&rend, rtc::VideoSinkWants());
}
