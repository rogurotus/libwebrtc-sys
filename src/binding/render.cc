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
	
	//buffer = webrtc::I420Buffer::Rotate(*buffer, webrtc::VideoRotation::kVideoRotation_270);


	auto image = new uint8_t[size];
	RTC_DCHECK(image != NULL);
	libyuv::I420ToRGB24(
		buffer->DataY(), buffer->StrideY(), 
		buffer->DataU(), buffer->StrideU(), 
		buffer->DataV(), buffer->StrideV(),
		image,
		buffer->width() * 3,
		buffer->width(), buffer->height());

	for (int i = 0; i < 640*480*3-1; i+=3)
	{
		auto temp = image[i];
		image[i] = image[i+2];
		image[i+2] = temp;
	}

	auto data = rust::Slice<const uint8_t>(image, size);
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