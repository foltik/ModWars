#include <unistd.h>
#include <cstdlib>
#include <ctime>

#include <iostream>
#include <thread>
#include <future>
#include <atomic>

struct Player {
    const char *name;
    int pos_x;
    int pos_y;
    int pos_z;
    int health;
};

static Player** players;

static std::atomic_bool exit_flag;
static std::atomic_int exit_code;

int setup() {
    srand(time(0));

    players = new Player*[8];

    int target = rand() % 8;
    for (int i = 0; i < 8; i++) {
        auto player = new Player;

        player->pos_x = rand();
        player->pos_y = rand();
        player->pos_z = rand();
        player->health = 100;

        if (i == target)
            player->name = "target";
        else
            player->name = "player";

        players[i] = player;
    }

    return target;
}

void watch(int target) {
    std::cout << "[Defender] You get 30 seconds to make your move." << std::endl;
    for (int counter = 0; counter < 300; counter++) {
        if (players[target]->health > 100) {
            exit_code = 2;
            exit_flag = true;
            return;
        }
        usleep(100000);
    }

    exit_code = 0;
    exit_flag = true;
}

int main(int argc, char* argv[]) {
    int target = setup();
    auto watcher = std::thread(watch, target);

    // Deploy your defenses here.

    while (!exit_flag)
        usleep(100000);

    watcher.join();
    return exit_code;
}
