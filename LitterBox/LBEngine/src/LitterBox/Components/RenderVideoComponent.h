/*!************************************************************************
 \file				RenderVideoComponent.h
 \author(s)			Ryan Tan Jian Hao
 \par DP email(s):	ryanjianhao.tan\@digipen.edu
 \par Course:       CSD2401A
 \date				09/03/2024
 \brief
 This file contains class definition of the VideoPlayerSystem class.
 NOTE: despite the file name, this class is actually an engine
 system and not a class.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
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
		Initialises a texture for the cutscene system to use. This texture is
		reserved throughout the lifetime of the engine.
		*************************************************************************/
		void Initialize() override;

		/*!***********************************************************************
		\brief
		Fixed update runs the video playback at a fixed interval

		NOTE: for now the interval is not actually synced to the videos specific
		FPS
		*************************************************************************/
		void FixedUpdate() override;
		/*!***********************************************************************
		\brief
		Destroy frees all data used by the system
		*************************************************************************/
		void Destroy() override;

		/*!***********************************************************************
		\brief
		Wrapper function for other files to easily play a video whenever they wish.

		\param
		video_file_name is the file name of the video without extension

		\param
		next_scene is file name (w/o extension) of the next scene you want to load
		after the video finishes.
		*************************************************************************/
		void PlayCutscene(const char* video_file_name, const char* next_scene);

		//ctor for the video player system (only sets the extern)
		VideoPlayerSystem() = default;
		~VideoPlayerSystem() = default;


		/*!***********************************************************************
		\brief
		Getter function to get the texture ID in openGL

		NOTE: THIS RETURNS THE TEXTURE ID AND NOT THE TEXTURE UNIT THAT THE TEXTURE
		ID IS BOUND TO.
		*************************************************************************/
		const unsigned int& get_frame() const { return tex_handle; };
	private:
		/*!***********************************************************************
		\brief
		load video file allocates memory for the appropriate contexts required
		by the library to open and stream data from a video file.

		\param
		video_file_name is the file name of the video without extension
		*************************************************************************/
		void load_video_file(const char* video_file_name);
		/*!***********************************************************************
		\brief
		load video frame will stream the next frame in the video file, already
		loaded through load_video_file function.

		To get the next frame the user simply needs to call the function iteratively
		until they get to a particular frame they want or the eof.
		*************************************************************************/
		bool load_video_frame();
		/*!***********************************************************************
		\brief
		free_video_state deallocates all memory that was used when load_video_file
		was called. This is because each video file is unique and may require
		different decoder contexts.
		*************************************************************************/
		void free_video_state();

		VideoRenderState vrs;
		std::string scene_to_transition;
		unsigned int tex_handle{ 0 };
		bool playCutscene{ false };
	};
}
