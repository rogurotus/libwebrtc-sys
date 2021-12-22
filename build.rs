
fn main() {
    println!("cargo:rustc-link-lib=webrtc");
    println!("cargo:rustc-link-lib=dmoguids");
    println!("cargo:rustc-link-lib=winmm");
    println!("cargo:rustc-link-lib=wmcodecdspuuid");
    println!("cargo:rustc-link-lib=secur32");
    println!("cargo:rustc-link-lib=msdmo");
    println!("cargo:rustc-link-lib=strmiids");

    //println!("cargo:rustc-link-lib=Mswsock");
    //println!("cargo:rustc-link-lib=Ws2_32");
    
    
    cxx_build::bridge("src/main.rs")
        .flag("-DWEBRTC_WIN")
        .file("src/binding/get_device.cc")
        .file("src/binding/capture.cc")
        .file("src/binding/user_media.cc")
        .file("src/binding/render.cc")
        .compile("libwebrtc-sys");
}

