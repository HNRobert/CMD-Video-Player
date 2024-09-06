//
//  video-player.cpp
//  CMD-Video-Player
//
//  Created by Robert He on 2024/9/2.
//

#include "basic-functions.hpp"
#include "video-player.hpp"

bool is_escape_key_pressed() {
#ifdef _WIN32
    // Windows-specific code to check if the ESC key is pressed
    return GetAsyncKeyState(VK_ESCAPE) & 0x8000;
#else
    // For Linux and macOS, we can implement a similar functionality if needed.
    // Here, you could use termios.h or another library to handle keypresses.
    return false; // Placeholder; you might want to implement platform-specific code here.
#endif
}

std::string image_to_ascii(const cv::Mat &image) {
    const char *asciiChars = "@%#*+=-:. "; // 字符画字符集，从密到疏
    unsigned long asciiLength = strlen(asciiChars);
    std::string asciiImage;

    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            uchar pixel = image.at<uchar>(i, j);
            char asciiChar = asciiChars[(pixel * asciiLength) / 256];
            asciiImage += asciiChar;
        }
        asciiImage += '\n';
    }

    return asciiImage;
}

void play_video(std::string video_path) {
    // 打开视频文件
    cv::VideoCapture cap(video_path);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open video file." << std::endl;
        return;
    }

    // 获取视频帧率
    double fps = cap.get(cv::CAP_PROP_FPS);
    int frame_delay = static_cast<int>(1000.0 / fps), remaining_delay; // 计算帧间的延迟，单位为毫秒

    cv::Mat frame;
    while (cap.read(frame)) { // 逐帧读取视频
        // 记录开始时间
        auto start_time = std::chrono::high_resolution_clock::now();

        // 转换为灰度图像
        cv::Mat grayFrame;
        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

        // 获取终端窗口大小
        int termWidth, termHeight;
        get_terminal_size(termWidth, termHeight);

        // 计算适合终端的最大图像宽高
        int maxWidth = termWidth;
        int maxHeight = termHeight;

        // 计算保持宽高比的图像尺寸
        int newWidth = maxWidth;
        int newHeight = (grayFrame.rows * newWidth) / grayFrame.cols / 2;

        // 如果图像的高度超出了终端的高度，则调整宽度
        if (newHeight > maxHeight) {
            newHeight = maxHeight;
            newWidth = (grayFrame.cols * newHeight * 2) / grayFrame.rows;
        }

        // 缩放图像以适应终端窗口大小
        int width = 80;                                             // 字符画的宽度，调整以适应你的终端
        int height = (grayFrame.rows * width) / grayFrame.cols / 2; // 保持宽高比，终端字符宽高比为2:1
        cv::resize(grayFrame, grayFrame, cv::Size(width, height));

        // 转换图像为字符画
        std::string asciiArt = image_to_ascii(grayFrame);

        // 清屏
        clear_screen();
        printf("%s", asciiArt.c_str());

        // 记录处理结束时间
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds processing_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

        // 计算剩余的时间
        remaining_delay = frame_delay - (int)processing_time.count();
        if (remaining_delay > 0) {
            printf("%d %lld \n", remaining_delay, processing_time.count());
            std::this_thread::sleep_for(std::chrono::milliseconds(remaining_delay));
        }

        if (is_escape_key_pressed()) {
            break;
        }
    }

    cap.release();
    std::cout << "Completed!" << std::endl;
}
