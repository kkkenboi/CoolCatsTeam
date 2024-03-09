#pragma once
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <inttypes.h>
}
//
//
//This script has to be done in C not C++
//References : https://github.com/leandromoreira/ffmpeg-libav-tutorial
//struct VideoReader {
//	int width, height;
//	AVRational time;
//
//	AVFormatContext* avFormatContext;	//stores the video context
//	AVCodecContext* avCodecContext;		//stores the codec context
//	int videoStreamIndex;				//video stream id (there's also audio stream ID)
//	//Video stream splits into frames and packets
//	AVFrame* avFrame;					
//	AVPacket* avPacket;
//	SwsContext* swsContext;				//context for scaling the video
//
//};

namespace LB
{
	class VideoPlayerSystem
	{
		struct VideoRenderState
		{

		};

	public:
		//ctor for the video player system (only sets the extern)
		VideoPlayerSystem();
		//Function to attempt to play the video
		void OnPlayVideo();

		void load_video_file();
		void load_video_frame();
		void free_video_state();
	private:
		//Helper functions to decode the packet
		int decode_packet(AVPacket* pPacket, AVCodecContext* pCodecContext, AVFrame* pFrame);
		//Helper function to log the data
		void logging(const char* fmt, ...);
		//Helper function to test to see if I can read the data
		void save_gray_frame(unsigned char* buf, int wrap, int xsize, int ysize);
	};

	extern VideoPlayerSystem* VIDEOPLAYER;
}
