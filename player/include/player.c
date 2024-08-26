#include "player.h"

void PlayerState_Init(PlayerState *state, char *filename) {
    state->filename = filename;

    state->vid_stream_index = -1;
    state->avfc = NULL;
    state->avcc = NULL;

    state->window = NULL;
    state->renderer = NULL;   
    state->texture = NULL;
}

int play(char *filename) {
    int errcode = 0;
    PlayerState state;

    PlayerState_Init(&state, filename);

    errcode = player_read_file(&state);
    if (errcode < 0) goto end; 

    errcode = player_configure_decoder(&state);
    if (errcode < 0) goto end; 

    errcode = player_prepare_rendering(&state);
    if (errcode < 0) goto end; 

    errcode = player_render(&state);
    if (errcode < 0) goto end; 

    end:
        player_quit(&state);
        return errcode;
}

int player_read_file(PlayerState *state) {
    int errcode = 0;

    if ((errcode = avformat_open_input(&state->avfc, state->filename, NULL, NULL)) < 0) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Failed to open input file! Message: %s\n",
            av_err2str(errcode)
        );
        return errcode;
    }
    SDL_Log("File opened\n");

    if ((errcode = avformat_find_stream_info(state->avfc, NULL)) < 0) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Failed to read stream info! Message: %s\n",
            av_err2str(errcode)
        );
        return errcode;
    }
    SDL_Log("Read stream info\n");
    av_dump_format(state->avfc, 0, state->filename, 0);

    return errcode;
}