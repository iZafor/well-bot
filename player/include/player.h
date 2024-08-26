#pragma once

#include <SDL.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>

typedef struct PlayerState {
    char *filename;

    /* FFMPEG variables */
    int32_t vid_stream_index;
    AVFormatContext *avfc;
    AVCodecContext *avcc;

    /* SDL variables */
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
} PlayerState;

void PlayerState_Init(PlayerState *state, char *filename);

int play(char *filename);

int player_read_file(PlayerState *state);

int player_configure_decoder(PlayerState *state);

int player_prepare_rendering(PlayerState *state);

int player_render(PlayerState *state);

void player_quit(PlayerState *state);
