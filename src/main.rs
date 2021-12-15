
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

    unsafe extern "C++" {
        include!("libwebrtc-sys/include/get_device.h");
        
        fn video_device_names() -> UniquePtr<CxxVector<CxxString>>;
        fn audio_otput_device_names() -> UniquePtr<CxxVector<CxxString>>;
        fn audio_input_device_names() -> UniquePtr<CxxVector<CxxString>>;

        include!("libwebrtc-sys/include/get_video.h");
        fn create();
    }
}

fn main() 
{

    ffi::create();

    let devs = Device::enumeration_device();
    for i in devs
    {
        println!("{:?}",i);
    }
}