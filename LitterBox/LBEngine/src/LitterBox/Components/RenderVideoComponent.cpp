#include "RenderVideoComponent.h"
#include "LitterBox/Serialization/FileSystemManager.h"
#include <stdlib.h>
#include "LitterBox/Components/RenderComponent.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace LB
{
	//For the extern
	VideoPlayerSystem* VIDEOPLAYER = nullptr;
	VideoPlayerSystem::VideoPlayerSystem()
	{
		if (!VIDEOPLAYER) VIDEOPLAYER = this;
	}

	//Function to play the video
	void VideoPlayerSystem::OnPlayVideo()
	{
		//openGL stuff, shoddy attempt at trying to throw something to the screen. Can delete, not crucial
		GLuint tex_handle;
		glGenTextures(1, &tex_handle);
		glBindTexture(GL_TEXTURE_2D, tex_handle);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		//========= HOW VIDEOS WORK APPARENTLY =============================
		// Video.mp4 -> AVFormatContext -> AVStream -> AVPacket -> AVCodec -> AVFrame
		//1) Load the media file into AVFormatContext 
		//[Once they're loaded we can access their streams.]
		//2) From the stream we can extract the packets (I think it's still compressed)
		//3) We use the codec to uncompress the packets to load them into the frame
		//4) Once we have the frame, we technically have the raw frame data.
		//5) Videos are mostly done in YUV format. So we need to use sws to convert it to RGB...
		//[But somehow sws scaler gives a linker error so I think we need to do it manually...]


		//This sets the context for the video
		AVFormatContext* ctx = avformat_alloc_context();
		if (!ctx) std::cout << "Could not allocate memory for format context\n";

		//We try to open the file
		int test = avformat_open_input(&ctx, FILESYSTEM->GetFilePath("samplevideo").string().c_str(), NULL, NULL);
		if (test != 0) std::cout << "Could not open the file! \n";

		//Then we find the stream
		test = avformat_find_stream_info(ctx, NULL);
		if (test < 0) std::cout << "Could not get stream info! \n";

		//We try to find the stream and get the correct codec
		const AVCodec* codec = NULL;
		AVCodecParameters* pCodecParameters = NULL;
		int videoIndex = -1;

		for (int i = 0; i < ctx->nb_streams; ++i)
		{
			AVCodecParameters* localCodecParams = NULL;
			localCodecParams = ctx->streams[i]->codecpar;
			const AVCodec* localCodec = NULL;
			localCodec = avcodec_find_decoder(localCodecParams->codec_id);
			if (localCodec == NULL) std::cout << "Unsupported codec! \n";

			if (localCodecParams->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				if (videoIndex == -1)
				{
					videoIndex = i;
					codec = localCodec;
					pCodecParameters = localCodecParams;
				}
			}
		}

		if (videoIndex == -1) std::cout << "File does not contain video stream!\n";
		//This dump function will tell you more about the video
		//av_dump_format(ctx, videoIndex, FILESYSTEM->GetFilePath("samplevideo").string().c_str(), false);

		//Now we need to get the codec stuff
		AVCodecContext* avcc = avcodec_alloc_context3(codec);
		if (!avcc) std::cout << "failed to allocate mem for AVCodecContext\n";
		//And the correct codec params
		test = avcodec_parameters_to_context(avcc, ctx->streams[videoIndex]->codecpar);
		if (test < 0) std::cout << "failed to copy codec params to codec context\n";
		test = avcodec_open2(avcc, codec, NULL);
		if (test < 0) std::cout << "failed to open codec through avcodec_open2\n";
		//Allocating space for frame
		AVFrame* frame = av_frame_alloc();
		if (!frame) std::cout << "failed to allocate memory for frame\n";
		//Allocating space for packet
		AVPacket* packet = av_packet_alloc();
		if (!packet) std::cout << "failed to allocate memory for packet\n";

		int response = 0;
		//Helper int to limit the number of packets we process
		//int packets_to_process = 8;

		//As long as there are packets to be read
		while (av_read_frame(ctx, packet) >= 0)
		{	//if the packet is from the video stream
			if (packet->stream_index == videoIndex)
			{
				//Helper function to decode and read through the frame data
				response = decode_packet(packet, avcc, frame);
				//response = avcodec_receive_frame(avcc, frame);
				if (response < 0) break;
				//Helper check to limit the number of packets we process
				//if (--packets_to_process <= 0) break;
			}
			av_packet_unref(packet);
		}

		//By right we should be able to use this but I get linker errors :C
		//const int width = frame->width;
		//const int height = frame->height;
		//uint8_t* frameBuffer;		//I think I need to like memalign this to 16 or something???? Honestly not sure
		//Set up scaler 
		//SwsContext* swsScaler = sws_getContext(frame->width, frame->height, avcc->pix_fmt, frame->width, frame->height, AV_PIX_FMT_RGB0, SWS_BILINEAR, NULL, NULL, NULL);
		//uint8_t* dest[4] = { frameBuffer, NULL,NULL,NULL };
		//int dest_lineSize[4] = {width * 4,0,0,0};
		//sws_scale(swsScaler, frame->data, frame->linesize, 0, frame->height, dest, dest_lineSize);
		


		//Need to remember to do this or there will be leaks!
		//avformat_close_input(&ctx);
		//av_packet_free(&packet);
		//av_frame_free(&frame);
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
				save_gray_frame(pFrame->data[0], pFrame->linesize[0], pFrame->width, pFrame->height);
			}
		}
		pts = pFrame->pts;
		return 0;
	}

}




