//
//  vedio-player.hpp
//  CMD-Video-Player
//
//  Created by Robert He on 2024/9/2.
//

#ifndef video_player_hpp
#define video_player_hpp

#include <algorithm>
#include <chrono>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <thread>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#ifdef _WIN32
#include <windows.h>
#endif

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1 : 0)
void play_video(std::string video_path);

#endif /* video_player_hpp */
