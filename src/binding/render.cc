//#include "libwebrtc-sys/include/render.h"
#include "libwebrtc-sys/src/main.rs.h"

Render::Render(int width, int height)
{
	size = width * 3 * height;
	image_ = new uint8_t[size];
}

void Render::OnFrame(const webrtc::VideoFrame& frame)
{
	rtc::scoped_refptr<webrtc::I420BufferInterface> buffer(
		frame.video_frame_buffer()->ToI420());
	if (frame.rotation() != webrtc::kVideoRotation_0) {
		buffer = webrtc::I420Buffer::Rotate(*buffer, frame.rotation());
	}
	RTC_DCHECK(image_ != NULL);
	libyuv::I420ToRGB24(buffer->DataY(), buffer->StrideY(), buffer->DataU(),
		buffer->StrideU(), buffer->DataV(), buffer->StrideV(),
		image_,
		buffer->width() * 3,
		buffer->width(), buffer->height());

	auto data = rust::Slice<const uint8_t>(image_, size);
	draw_image(*rrender, 640, 480, data);
	
}

void Render::set_rrender(RustRender& render)
{
	rrender = &render;
}

std::unique_ptr<Render> create_render(int width, int height)
{
	return std::make_unique<Render>(Render(width, height));
}