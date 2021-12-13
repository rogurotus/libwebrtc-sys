#pragma once

#include <memory>
#include <vector>
#include <string>

#include "modules\audio_device\win\audio_device_core_win.h"
#include "modules\video_capture\video_capture_factory.h"


std::unique_ptr<std::vector<std::string>> video_device_names();
std::unique_ptr<std::vector<std::string>> audio_input_device_names();
std::unique_ptr<std::vector<std::string>> audio_otput_device_names();