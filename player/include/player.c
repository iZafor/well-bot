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

int player_configure_decoder(PlayerState *state) {
    for (uint32_t i = 0; i < state->avfc->nb_streams; i++) {
        AVCodecParameters *params = state->avfc->streams[i]->codecpar;

        if (params->codec_type == AVMEDIA_TYPE_VIDEO) {
            const AVCodec *avc = avcodec_find_decoder(params->codec_id);
            if (avc == NULL) {
                SDL_LogError(
                    SDL_LOG_CATEGORY_APPLICATION,
                    "Decoder not found!\n"
                );
                return AVERROR_DECODER_NOT_FOUND;
            }
            SDL_Log("Found decoder\n");

            state->avcc = avcodec_alloc_context3(avc);
            if (state->avcc == NULL) {
                SDL_LogError(
                    SDL_LOG_CATEGORY_APPLICATION,
                    "Failed to allocate memory for AVCodecContext!\n"
                );
                return AVERROR(ENOMEM);
            } 
            SDL_Log("Memory allocated for AVCodecContext\n");

            int errcode = avcodec_open2(state->avcc, avc, NULL);
            if (errcode < 0) {
                SDL_LogError(
                    SDL_LOG_CATEGORY_APPLICATION,
                    "Failed to open the decoder! Message: %s\n",
                    av_err2str(errcode)
                );
                return errcode;
            }
            SDL_Log("Ready for decoding\n");

            state->vid_stream_index = i;

            break;
        }
    }

    if (state->vid_stream_index == -1) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Video stream not found!\n"
        );
        return AVERROR_INVALIDDATA;
    }

    return 0;
}

int player_prepare_rendering(PlayerState *state) {
    state->window = SDL_CreateWindow(
        "Player",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        0, 0,
        SDL_WINDOW_FULLSCREEN_DESKTOP
    );
    if (state->window == NULL) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Failed to create to window! Message: %s\n",
            SDL_GetError()
        );
        return -1;
    }
    SDL_Log("Window created\n");

    state->renderer = SDL_CreateRenderer(state->window, -1, SDL_RENDERER_SOFTWARE);
    if (state->renderer == NULL) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Failed to create to renderer! Message: %s\n",
            SDL_GetError()
        );
        return -1;
    }
    SDL_Log("Renderer created\n");

    state->texture = SDL_CreateTexture(
        state->renderer,
        SDL_PIXELFORMAT_YV12,
        SDL_TEXTUREACCESS_STREAMING,
        state->avcc->width,
        state->avcc->height
    );
    if (state->texture == NULL) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Failed to create to texture! Message: %s\n",
            SDL_GetError()
        );
        return -1;
    }
    SDL_Log("Texture created\n");

    return 0;
}