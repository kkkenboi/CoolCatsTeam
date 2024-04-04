/*!************************************************************************
 \file				RenderVideoComponent.cpp
 \author(s)			Ryan Tan Jian Hao
 \par DP email(s):	ryanjianhao.tan\@digipen.edu
 \par Course:       CSD2401A
 \date				09/03/2024
 \brief
 This file contains member functions definitions 
 of the VideoPlayerSystem class.
 
 NOTE: despite the file name, this class is actually an engine
 system and not a class.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "RenderVideoComponent.h"
#include "LitterBox/Serialization/FileSystemManager.h"
#include <stdlib.h>
#include "LitterBox/Components/RenderComponent.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Audio/AudioManager.h"
#include "LitterBox/Engine/Input.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace LB
{
	/*!***********************************************************************
	\brief
	Initialises a texture for the cutscene system to use. This texture is
	reserved throughout the lifetime of the engine.
	*************************************************************************/
	void VideoPlayerSystem::Initialize()
	{
		//openGL stuff, shoddy attempt at trying to throw something to the screen. Can delete, not crucial
		glGenTextures(1, &tex_handle);
		glBindTexture(GL_TEXTURE_2D, tex_handle);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		ASSETMANAGER->AddReservedTexture(tex_handle);
	}

	/*!***********************************************************************
	\brief
	Fixed update runs the video playback at a fixed interval

	NOTE: for now the interval is not actually synced to the videos specific
	FPS
	*************************************************************************/
	void VideoPlayerSystem::Update()
	{
		/*if (playCutscene && !load_video_frame())
		{
			playCutscene = false;
			//free_video_state();
			SCENEMANAGER->LoadScene(scene_to_transition);
			AUDIOMANAGER->StopAllChannels();
		}
		play_video_frame();*/

		//if we are playing cutscene
		if (playCutscene)
		{
			//load max 5 frames ahead
			if (frame_queue.size() < 5 && !last_frame_read)
			{
				//we load data until the last frame
				if (!last_frame_read && !load_video_frame())
					last_frame_read = true; //flag to indicate we have read the last frame
			}

			//if we still have objects in the queue
			if (!frame_queue.empty())
			{
				//we play video frame if it is supposed to be played
				while (!frame_queue.empty() && frame_queue.front().ptss * cutoff < timer)
					play_video_frame(); //pops the frame in the queue
			}
			//else if we have read and played the last frame
			else if (frame_queue.empty() && last_frame_read)
			{
				playCutscene = false;
				free_video_state();
				SCENEMANAGER->LoadScene(scene_to_transition);
				AUDIOMANAGER->StopAllChannels();
			}

			timer += TIME->GetDeltaTime();
		}

		if (INPUT->IsKeyPressed(KeyCode::KEY_ESCAPE))
		{
			while (!frame_queue.empty())
				frame_queue.pop();

			last_frame_read = true;
		}
	}

	/*!***********************************************************************
	\brief
	Destroy frees all data used by the system
	*************************************************************************/
	void VideoPlayerSystem::Destroy()
	{
		ASSETMANAGER->RemoveReservedTexture();
		if (tex_handle)
			glDeleteTextures(1, &tex_handle);
		if (vrs.av_codec_ctx)
			free_video_state();
	}

	/*!***********************************************************************
	\brief
	Helper function to account for depercated pixel format of video frames.

	\param
	The pixel format of the video frame
	*************************************************************************/
	static AVPixelFormat correct_for_deprecated_pixel_format(AVPixelFormat pix_fmt) {
		// Fix swscaler deprecated pixel format warning
		// (YUVJ has been deprecated, change pixel format to regular YUV)
		switch (pix_fmt) {
		case AV_PIX_FMT_YUVJ420P: return AV_PIX_FMT_YUV420P;
		case AV_PIX_FMT_YUVJ422P: return AV_PIX_FMT_YUV422P;
		case AV_PIX_FMT_YUVJ444P: return AV_PIX_FMT_YUV444P;
		case AV_PIX_FMT_YUVJ440P: return AV_PIX_FMT_YUV440P;
		default:                  return pix_fmt;
		}
	}

	/*!***********************************************************************
	\brief
	Wrapper function for other files to easily play a video whenever they wish.

	\param
	video_file_name is the file name of the video without extension

	\param
	next_scene is file name (w/o extension) of the next scene you want to load 
	after the video finishes. 
	*************************************************************************/
	void VideoPlayerSystem::PlayCutscene(const char* video_file_name, const char* next_scene)
	{
		SCENEMANAGER->LoadScene("SceneCutscene");

		scene_to_transition = next_scene;
		load_video_file(video_file_name);
		int channel = AUDIOMANAGER->PlaySound(video_file_name);
		AUDIOMANAGER->SetChannelVolume(channel, 0.3f);
		playCutscene = true;
		last_frame_read = false;
		timer = 0.0;
	}

	/*!***********************************************************************
	\brief
	load video file allocates memory for the appropriate contexts required
	by the library to open and stream data from a video file.

	\param
	video_file_name is the file name of the video without extension
	*************************************************************************/
	void VideoPlayerSystem::load_video_file(const char* video_file_name)
	{
		AVCodecParameters*& av_codec_params = vrs.av_codec_params;
		auto&				av_format_ctx	= vrs.av_format_ctx;
		AVCodecContext*&	av_codec_ctx	= vrs.av_codec_ctx;
		unsigned char*&		data			= vrs.fbuffer;
		AVPacket*&			av_packet		= vrs.av_packet;
		AVCodec*&			av_codec		= vrs.av_codec;
		AVFrame*&			av_frame		= vrs.av_frame;
		int&				video_stream_index = vrs.video_stream_index;

		//1. open the file
		av_format_ctx = avformat_alloc_context();
		if (!av_format_ctx)
		{
			DebuggerLogError("Could not create AVFormatContxt");
			return;
		}

		if (ASSETMANAGER->LoadVideo(&av_format_ctx, FILESYSTEM->GetFilePath(video_file_name).string().c_str()) != 0)//avformat_open_input(&av_format_ctx, FILESYSTEM->GetFilePath(video_file_name).string().c_str(), NULL, NULL) != 0)
		{
			DebuggerLogError("Could not Open video file");
			return;
		}

		//2. find the video stream that we want in the file
		video_stream_index = -1;
		for (unsigned int i{ 0 }; i < av_format_ctx->nb_streams; ++i)
		{
			//auto stream = av_format_ctx->streams[i];
			av_codec_params = av_format_ctx->streams[i]->codecpar;
			const AVCodec* av_codec_inside = avcodec_find_decoder(av_codec_params->codec_id);

			if (!av_codec_inside)
			{
				continue;
			}

			if (av_codec_inside->type == AVMEDIA_TYPE_VIDEO)
			{
				//vrs.time_base = av_format_ctx->streams[i]->time_base;
				cutoff = (double)av_format_ctx->streams[i]->time_base.num / 
					(double)av_format_ctx->streams[i]->time_base.den;
				av_codec = const_cast<AVCodec*>(av_codec_inside);
				video_stream_index = i;
				break;
			}
		}

		if (video_stream_index == -1)
		{
			DebuggerLogError("Could not find valid video stream inside file");
			return;
		}

		//3. set up decoder
		av_codec_ctx = avcodec_alloc_context3(av_codec);
		if (!av_codec_ctx)
		{
			DebuggerLogError("Could not create AVCodecContext");
			return;
		}

		if (avcodec_parameters_to_context(av_codec_ctx, av_codec_params) < 0)
		{
			DebuggerLogError("Could not initialize AVCodecContext");
			return;
		}
		if (avcodec_open2(av_codec_ctx, av_codec, NULL) < 0)
		{
			DebuggerLogError("Could not open codec");
			return;
		}

		//4. create necessary tools for decoding the frame
		av_frame = av_frame_alloc();
		if (!av_frame)
		{
			DebuggerLogError("Could not allocate AVFrame");
			return;
		}
		av_packet = av_packet_alloc();
		if (!av_packet)
		{
			DebuggerLogError("Could not allocate AVPacket");
			return;
		}

		//create a buffer to store the video frame data
		//the data is stored in YUVJ format which is 4 bytes per pixel
		data = new unsigned char[av_codec_params->width * av_codec_params->height * 4];
	}

	/*!***********************************************************************
	\brief
	load video frame will stream the next frame in the video file, already
	loaded through load_video_file function.

	To get the next frame the user simply needs to call the function iteratively
	until they get to a particular frame they want or the eof.
	*************************************************************************/
	bool VideoPlayerSystem::load_video_frame()
	{
		static bool trying{ true };

		AVFormatContext*&	av_format_ctx		= vrs.av_format_ctx;
		AVCodecContext*&	av_codec_ctx		= vrs.av_codec_ctx;
		unsigned char*&		data				= vrs.fbuffer;
		SwsContext*&		sws_scalar_ctx		= vrs.sws_scaler_ctx;
		AVPacket*&			av_packet			= vrs.av_packet;
		AVFrame*&			av_frame			= vrs.av_frame;
		int&				video_stream_index	= vrs.video_stream_index;
		
		//1. decode one frame
		int response{ 0 }, test{ av_read_frame(av_format_ctx, av_packet) >= 0 };
		//read the next frame
		while (test)
		{
			

			//if the packet does not contain video data we read the next packet
			if (av_packet->stream_index != video_stream_index)
			{
				av_packet_unref(av_packet);
				test = av_read_frame(av_format_ctx, av_packet) >= 0;
				continue;
			}

			//if the packet does contain video data then we send it
			//to the decoder to get broken down into an image
			response = avcodec_send_packet(av_codec_ctx, av_packet);
			if (response < 0)
			{
				DebuggerLogError("failed to decode packet");
				return false;
			}

			//we load the image from the packet into the frame data struct
			//via the decoder 
			response = avcodec_receive_frame(av_codec_ctx, av_frame);
			if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
			{
				test = av_read_frame(av_format_ctx, av_packet) >= 0;
				continue;
			}
			else if (response < 0)
			{
				DebuggerLogError("failed to decode packet");
				return false;
			}

			av_packet_unref(av_packet);
			break;
		}

		//we create a sws context incase we don't have on already
		if (!sws_scalar_ctx)
		{
			auto source_pix_fmt = correct_for_deprecated_pixel_format(av_codec_ctx->pix_fmt);
			sws_scalar_ctx = sws_getContext(
				av_frame->width,
				av_frame->height,
				source_pix_fmt,
				av_frame->width,
				av_frame->height,
				AV_PIX_FMT_RGB0,
				SWS_BILINEAR,
				NULL, NULL, NULL);
		}

		if (!sws_scalar_ctx)
		{
			DebuggerLogError("Could not initialize sw scalar");
			return false;
		}

		//we need to convert the video data from YUVJ to RGBA which is done through sws_scale
		uint8_t* dest[4] = { data, NULL, NULL, NULL };
		int dest_linesize[4] = { av_frame->width * 4, 0, 0, 0 };
		sws_scale(sws_scalar_ctx, av_frame->data, av_frame->linesize, 0, av_frame->height, dest, dest_linesize);

		frame frame{};
		frame.data = new uint8_t[av_frame->width * av_frame->height * 4];
		memcpy(frame.data, data, av_frame->width * av_frame->height * 4);
		frame.width = av_frame->width;
		frame.height = av_frame->height;
		frame.ptss = av_frame->pts;
		frame_queue.push(frame);

		//load the data onto the texture that is reserved by the system
		/*glBindTexture(GL_TEXTURE_2D, tex_handle);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, av_frame->width, av_frame->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);*/
		

		if (!test)
			return false;

		return true;
	}

	/*!***********************************************************************
	\brief
	free_video_state deallocates all memory that was used when load_video_file
	was called. This is because each video file is unique and may require
	different decoder contexts.
	*************************************************************************/
	void VideoPlayerSystem::free_video_state()
	{
		AVFormatContext*& av_format_ctx = vrs.av_format_ctx;
		AVCodecContext*& av_codec_ctx = vrs.av_codec_ctx;
		unsigned char*& data = vrs.fbuffer;
		SwsContext*& sws_scalar_ctx = vrs.sws_scaler_ctx;
		AVPacket*& av_packet = vrs.av_packet;
		AVFrame*& av_frame = vrs.av_frame;

		sws_freeContext(sws_scalar_ctx);
		ASSETMANAGER->FreeVideo(&av_format_ctx);
		avformat_close_input(&av_format_ctx);
		avformat_free_context(av_format_ctx);
		av_frame_free(&av_frame);
		av_packet_free(&av_packet);
		avcodec_free_context(&av_codec_ctx);

		delete[] data;
		memset(&vrs, 0, sizeof(vrs));
		vrs.video_stream_index = -1;

		pts = 0;
		timer = 0.0;
		cutoff = 0.0;
		playCutscene = false ;
		last_frame_read = false;

		while (!frame_queue.empty())
		{
			if(frame_queue.front().data)
				delete[] frame_queue.front().data;
			frame_queue.pop();
		}
	}

	bool VideoPlayerSystem::play_video_frame()
	{
		if (!frame_queue.empty())
		{
			glBindTexture(GL_TEXTURE_2D, tex_handle);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame_queue.front().width, frame_queue.front().height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame_queue.front().data);
			glBindTexture(GL_TEXTURE_2D, 0);
			
			delete[] frame_queue.front().data;
			frame_queue.pop();
			
			return true;
		}
		return false;
	}
}