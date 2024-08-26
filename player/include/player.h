#pragma once

#include <SDL.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>

typedef struct PlayerState {
    /* FFMPEG variables */
    uint32_t vid_stream_index;
    AVFormatContext *avfc;
    AVCodecContext *avcc;

    /* SDL variables */
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
} PlayerState;
