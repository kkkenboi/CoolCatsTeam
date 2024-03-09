#include "RenderVideoComponent.h"
#include "LitterBox/Serialization/FileSystemManager.h"
#include <stdlib.h>
#include "LitterBox/Components/RenderComponent.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

unsigned int tex_handle{ 0 };

namespace LB
{
	//For the extern
	VideoPlayerSystem* VIDEOPLAYER = nullptr;
	VideoPlayerSystem::VideoPlayerSystem()
	{
		if (!VIDEOPLAYER) VIDEOPLAYER = this;
	}

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

	//Function to play the video
	void VideoPlayerSystem::OnPlayVideo()
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

		AVFormatContext* av_format_ctx = avformat_alloc_context();
		if (!av_format_ctx)
		{
			DebuggerLogError("Could not create AVFormatContxt");
			return;
		}

		if (avformat_open_input(&av_format_ctx, FILESYSTEM->GetFilePath("samplevideo").string().c_str(), NULL, NULL) != 0)
		{
			DebuggerLogError("Could not Open video file");
			return;
		}

		int video_stream_index = -1;
		AVCodecParameters* av_codec_params{ nullptr };
		AVCodec* av_codec{ nullptr };

		for (int i{ 0 }; i < av_format_ctx->nb_streams; ++i)
		{
			auto stream = av_format_ctx->streams[i];
			av_codec_params = av_format_ctx->streams[i]->codecpar;
			const AVCodec* av_codec_inside = avcodec_find_decoder(av_codec_params->codec_id);

			if (!av_codec_inside)
			{
				continue;
			}

			if (av_codec_inside->type == AVMEDIA_TYPE_VIDEO)
			{
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

		AVCodecContext* av_codec_ctx = avcodec_alloc_context3(av_codec);
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

		AVFrame* av_frame = av_frame_alloc();
		if (!av_frame)
		{
			DebuggerLogError("Could not allocate AVFrame");
			return;
		}
		AVPacket* av_packet = av_packet_alloc();
		if (!av_packet)
		{
			DebuggerLogError("Could not allocate AVPacket");
			return;
		}

		int response{ 0 };
		while (av_read_frame(av_format_ctx, av_packet) >= 0)
		{
			if (av_packet->stream_index != video_stream_index)
			{
				continue;
			}

			response = avcodec_send_packet(av_codec_ctx, av_packet);
			if (response < 0)
			{
				DebuggerLogError("failed to decode packet");
				return;
			}

			response = avcodec_receive_frame(av_codec_ctx, av_frame);
			if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
			{
				continue;
			}
			else if (response < 0)
			{
				DebuggerLogError("failed to decode packet");
				return;
			}

			av_packet_unref(av_packet);
			break;
		}

		unsigned char* data = new unsigned char[av_frame->width * av_frame->height * 4];
		/*for (int x{ 0 }; x < av_frame->width; ++x)
			for (int y{ 0 }; y < av_frame->height; ++y)
			{
				data[y * av_frame->width * 3 + x * 3] = av_frame->data[0][y * av_frame->linesize[0] + x];
				data[y * av_frame->width * 3 + x * 3 + 1] = av_frame->data[0][y * av_frame->linesize[0] + x];
				data[y * av_frame->width * 3 + x * 3 + 2] = av_frame->data[0][y * av_frame->linesize[0] + x];
			}*/

		SwsContext* sws_scalar_ctx = sws_getContext(av_frame->width,
			av_frame->height,
			av_codec_ctx->pix_fmt,
			av_frame->width,
			av_frame->height,
			AV_PIX_FMT_RGB0,
			SWS_BILINEAR,
			NULL, NULL, NULL);

		if (!sws_scalar_ctx)
		{
			DebuggerLogError("Could not initialize sw scalar");
			return;
		}
		uint8_t* dest[4] = { data, NULL, NULL, NULL };
		int dest_linesize[4] = { av_frame->width * 4, 0, 0, 0 };
		sws_scale(sws_scalar_ctx, av_frame->data, av_frame->linesize, 0, av_frame->height, dest, dest_linesize);
		sws_freeContext(sws_scalar_ctx);

		glBindTexture(GL_TEXTURE_2D, tex_handle);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, av_frame->width, av_frame->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);

		avformat_close_input(&av_format_ctx);
		avformat_free_context(av_format_ctx);
		av_frame_free(&av_frame);
		av_packet_free(&av_packet);
		avcodec_free_context(&av_codec_ctx);
	}

	void VideoPlayerSystem::logging(const char* fmt, ...)
	{
		va_list args;
		fprintf(stderr, "LOG: ");
		va_start(args, fmt);
		vfprintf(stderr, fmt, args);
		va_end(args);
		fprintf(stderr, "\n");
	}
	void VideoPlayerSystem::save_gray_frame(unsigned char* buf, int wrap, int xsize, int ysize)
	{

		FILE* f;
		int i;
		f = fopen("generated_image", "w");
		// writing the minimal required header for a pgm file format
		// portable graymap format -> https://en.wikipedia.org/wiki/Netpbm_format#PGM_example
		fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);

		// writing line by line
		for (i = 0; i < ysize; i++)
			fwrite(buf + i * wrap, 1, xsize, f);
		fclose(f);
	}
	int VideoPlayerSystem::decode_packet(AVPacket* pPacket, AVCodecContext* pCodecContext, AVFrame* pFrame)
	{
		char err[1024];
		int response = avcodec_send_packet(pCodecContext, pPacket);
		int pts;	//this keeps track of the time of the video (unused for now)
		//If anything fails when we try to retrieve the packet, it will print the error
		if (response < 0) {
			std::cout << av_make_error_string(err, 1024, response) << '\n';
			return response;
		}
		
		while (response >= 0)
		{
			response = avcodec_receive_frame(pCodecContext, pFrame);
			//char frame_filename[1024];
			if (response >= 0)
			{	
				//We pass in the first index and alignment and save it to a PGM
				//save_gray_frame(pFrame->data[0], pFrame->linesize[0], pFrame->width, pFrame->height);

				/*glBindTexture(GL_TEXTURE_2D, tex_handle);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pFrame->width, pFrame->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pFrame->data);
				glBindTexture(GL_TEXTURE_2D, 0);*/
			}
		}
		pts = pFrame->pts;
		return 0;
	}

}




