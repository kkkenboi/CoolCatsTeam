#pragma once
#include "LitterBox/Core/System.h" // For ISystem
#include "LitterBox/Scene/SceneManager.h" //For SCENEMANAGER
#include <string>

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
	class VideoPlayerSystem : public ISystem, public Singleton<VideoPlayerSystem>
	{
		struct VideoRenderState
		{
			int width{ 0 }, height{ 0 }, video_stream_index{ -1 };
			unsigned char* fbuffer{ nullptr };
			unsigned int tex_handle{ 0 };

			AVFormatContext* av_format_ctx{ nullptr };
			AVCodecContext* av_codec_ctx{ nullptr };
			AVCodecParameters* av_codec_params{ nullptr };
			AVCodec* av_codec{ nullptr };
			AVFrame* av_frame{ nullptr };
			AVPacket* av_packet{ nullptr };
			SwsContext* sws_scaler_ctx{ nullptr };
		};

	public:
		/*!***********************************************************************
		 \brief
		 Initalises the Game Logic system
		*************************************************************************/
		void Initialize() override;
		//void Update() override;
		//void FixedUpdate() override;
		void Destroy() override;

		void PlayCutscene(const char* video_file_name, const char* next_scene);

		//ctor for the video player system (only sets the extern)
		VideoPlayerSystem() = default;
		~VideoPlayerSystem() = default;

		const unsigned int& get_frame() const { return vrs.tex_handle; };
	private:
		void load_video_file(const char* video_file_name);
		bool load_video_frame();
		void free_video_state();

		VideoRenderState vrs;
		std::string scene_to_transition;
	};
}
