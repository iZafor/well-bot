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