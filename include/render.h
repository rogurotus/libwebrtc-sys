
#include "api\video\i420_buffer.h"
#include "third_party\libyuv\include\libyuv\convert_argb.h"
#include "api\video\video_sink_interface.h"
#include "api\video\video_frame.h"
//#include "rust\cxx.h"

struct RustRender;

class Render : public rtc::VideoSinkInterface<webrtc::VideoFrame>
{
public:
	Render(int width, int height);
	uint8_t* image_;
	size_t size;
	RustRender* rrender;
	void OnFrame(const webrtc::VideoFrame& frame);
	void OnDiscardedFrame() {}
	void OnConstraintsChanged(const webrtc::VideoTrackSourceConstraints& constraints) {}
	void set_rrender(RustRender& rrender);
};

std::unique_ptr<Render> create_render(int width, int height);
