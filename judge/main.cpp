#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

int attack() {
    if (int pid = fork(); pid != 0) {
        return pid;
    } else {
        return execl("attacker", "attacker", nullptr);
    }
}

int defend() {
    if (int pid = fork(); pid != 0) {
        return pid;
    } else {
        return execl("defender", "defender", nullptr);
    }
}

int main(int argc, char* argv[]) {
    int defender_pid = defend();
    if (defender_pid == -1)
        return -1;

    sleep(1);

    int attacker_pid = attack();
    if (attacker_pid == -1)
        return -1;

    int status;
    waitpid(defender_pid, &status, 0);

    if (WIFEXITED(status)) {
        if (int code = WEXITSTATUS(status); code == -1)
            std::cout << "[Judge] Failed to execute both programs.";
        else if (code == 0)
            std::cout << "[Judge] The attacker has run out of time." << std::endl;
        else if (code == 1)
            std::cout << "[Judge] The attacker has been detected!" << std::endl;
        else if (code == 2)
            std::cout << "[Judge] The defender has been pwned!" << std::endl;
        else
            std::cout << "[Judge] The defender has exited." << std::endl;
    } else if (WIFSIGNALED(status)) {
        int signal = WTERMSIG(status);
        std::cout << "[Judge] Disqualified! The defender was terminated by "
                  << strsignal(signal) << "!" << std::endl;
    } else {
        std::cout << "[Judge] What happened to the defender?" << std::endl;
    }

    kill(attacker_pid, SIGTERM);
    return 0;
}
