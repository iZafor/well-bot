#include "player.h"

int main(int argc, char *args[]) {
    if (argc != 2) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION, 
            "Invalid number of arguments! Expected 2 received %d\n",
            argc
        );
        return -1;
    }
    return play(args[1]);
}