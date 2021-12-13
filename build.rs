
fn main() {
    println!("cargo:rustc-link-lib=webrtc");
    println!("cargo:rustc-link-lib=dmoguids");
    println!("cargo:rustc-link-lib=winmm");
    println!("cargo:rustc-link-lib=wmcodecdspuuid");
    println!("cargo:rustc-link-lib=secur32");
    println!("cargo:rustc-link-lib=msdmo");
    println!("cargo:rustc-link-lib=strmiids");
    
    cxx_build::bridge("src/main.rs")
        .flag("-DWEBRTC_WIN")
        .file("src/get_device.cc")
        .compile("libwebrtc-sys");
}

