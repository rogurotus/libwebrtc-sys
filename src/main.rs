
extern crate sdl2;
use sdl2::pixels::Color;
use sdl2::rect::Point;
use sdl2::render::{Canvas};
use sdl2::video::Window;
use std::time::Duration;


#[derive(Debug)]
enum DeviceKind
{
    videoinput,
    audioinput,
    audiooutput,
}

#[derive(Debug)]
struct Device
{
    kind: DeviceKind,
    label: String,
}

impl Device
{
    fn enumeration_device() -> Vec<Device>
    {
        let mut vec = vec![];

        let video = ffi::video_device_names();
        for i in video.into_iter()
        {
            vec.push(Device{label: i.to_string(), kind: DeviceKind::videoinput});
        }

        let out_audio = ffi::audio_otput_device_names();
        for i in out_audio.into_iter()
        {
            vec.push(Device{label: i.to_string(), kind: DeviceKind::audiooutput});
        }

        let input_audio = ffi::audio_input_device_names();
        for i in input_audio.into_iter()
        {
            vec.push(Device{label: i.to_string(), kind: DeviceKind::audioinput});
        }
        vec
    }
}

#[cxx::bridge]
mod ffi {

    extern "Rust"
    {
        type RustRender;
        fn draw_image(render: &mut RustRender, width: i32, height: i32, image: &[u8]);
    }

    unsafe extern "C++" {
        include!("libwebrtc-sys/include/get_device.h");
        
        fn video_device_names() -> UniquePtr<CxxVector<CxxString>>;
        fn audio_otput_device_names() -> UniquePtr<CxxVector<CxxString>>;
        fn audio_input_device_names() -> UniquePtr<CxxVector<CxxString>>;

        include!("libwebrtc-sys/include/capture.h");
        fn test();

        include!("libwebrtc-sys/include/user_media.h");
        type UserMedia;
        fn create_user_media(video: bool, audio: bool) -> UniquePtr<UserMedia>;
        fn set_rend(self: Pin<&mut UserMedia>, rend: Pin<&mut Render>);

        include!("libwebrtc-sys/include/render.h");
        type Render;
        fn create_render(width: i32, height: i32) -> UniquePtr<Render>;
        fn set_rrender(self: Pin<&mut Render>, render: &mut RustRender);


    }
}

pub struct RustRender {
    canvas: Canvas<Window>,
    sdl: sdl2::Sdl,
}

impl RustRender {
    pub fn new() -> Self {
        let sdl_context = sdl2::init().unwrap();
        let video_subsystem = sdl_context.video().unwrap();

        let window = video_subsystem
            .window("rust-sdl2 demo", 640, 480)
            .position_centered()
            .build()
            .unwrap();

        let canvas = window.into_canvas().build().unwrap();
        RustRender {
            canvas: canvas,
            sdl: sdl_context,
        }
    }
}

pub fn draw_image(render: &mut RustRender, width: i32, height: i32, image: &[u8]) {
    render.canvas.clear();
    let mut iter = image.iter();
    for i in 0..width {
        for j in 0..height {
            render.canvas.set_draw_color(Color::RGB(
                *iter.next().unwrap(),
                *iter.next().unwrap(),
                *iter.next().unwrap(),
            ));
            let point = Point::new(i, j);
            render.canvas.draw_point(point).unwrap();
        }
    }
    render.canvas.present();
}


fn main() 
{
    let mut rrender = RustRender::new();
    let mut render = ffi::create_render(640, 480);
    render.pin_mut().set_rrender(&mut rrender);

    let mut user_media = ffi::create_user_media(true, true);
    user_media.pin_mut().set_rend(render.pin_mut());

    ::std::thread::sleep(Duration::new(50, 1_000_000_000u32 / 60));
}
