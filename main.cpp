#include <iostream>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#endif

void sleep_process(int seconds) {
    std::cout << "[Child Process] Starting to wait for " << seconds << " second(s)...\n";
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
    std::cout << "[Child Process] Finished after " << seconds << " second(s)!\n";
}

int main() {
    std::cout << "PARENT PROCESS: Start creating processes\n\n";

#ifdef _WIN32
    int times[] = { 1, 2, 3 };
    HANDLE hProcesses[3];
    PROCESS_INFORMATION pi[3];
    STARTUPINFO si[3];

    for (int i = 0; i < 3; i++) {
        ZeroMemory(&si[i], sizeof(si[i]));
        si[i].cb = sizeof(si[i]);
        ZeroMemory(&pi[i], sizeof(pi[i]));

        std::string command = "cmd.exe /c powershell -Command \"Start-Sleep -Seconds " + std::to_string(times[i]) + "\"";

        BOOL success = CreateProcessA(
            NULL,
            &command[0],
            NULL, NULL, FALSE, 0, NULL, NULL,
            &si[i], &pi[i]
        );

        if (success) {
            std::cout << "[Parent] Created Process " << (i + 1) << " (will wait for " << times[i] << " seconds).\n";
            hProcesses[i] = pi[i].hProcess;
        }
        else {
            std::cerr << "Error creating process on Windows!\n";
        }
    }

    std::cout << "\n[Parent] Waiting for all 3 processes to finish...\n";
    WaitForMultipleObjects(3, hProcesses, TRUE, INFINITE);

    for (int i = 0; i < 3; i++) {
        CloseHandle(pi[i].hProcess);
        CloseHandle(pi[i].hThread);
    }

#else
    int times[] = { 1, 2, 3 };

    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            std::cerr << "Error creating process with fork()!\n";
            return 1;
        }
        else if (pid == 0) {
            sleep_process(times[i]);
            return 0;
        }
        else {
            std::cout << "[Parent] Created Child Process with PID: " << pid << " (will wait for " << times[i] << " seconds).\n";
        }
    }

    std::cout << "\n[Parent] Waiting for all 3 processes to finish...\n";

    for (int i = 0; i < 3; i++) {
        int status;
        pid_t terminated_pid = wait(&status);
        std::cout << "[Parent] Child process with PID " << terminated_pid << " finished.\n";
    }

#endif

    std::cout << "\nPARENT PROCESS: All processes finished successfully!\n";
    return 0;
}#include <iostream>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#endif

void sleep_process(int seconds) {
    std::cout << "[Child Process] Starting to wait for " << seconds << " second(s)...\n";
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
    std::cout << "[Child Process] Finished after " << seconds << " second(s)!\n";
}

int main() {
    std::cout << "PARENT PROCESS: Start creating processes\n\n";

#ifdef _WIN32
    int times[] = { 1, 2, 3 };
    HANDLE hProcesses[3];
    PROCESS_INFORMATION pi[3];
    STARTUPINFO si[3];

    for (int i = 0; i < 3; i++) {
        ZeroMemory(&si[i], sizeof(si[i]));
        si[i].cb = sizeof(si[i]);
        ZeroMemory(&pi[i], sizeof(pi[i]));

        std::string command = "cmd.exe /c powershell -Command \"Start-Sleep -Seconds " + std::to_string(times[i]) + "\"";

        BOOL success = CreateProcessA(
            NULL,
            &command[0],
            NULL, NULL, FALSE, 0, NULL, NULL,
            &si[i], &pi[i]
        );

        if (success) {
            std::cout << "[Parent] Created Process " << (i + 1) << " (will wait for " << times[i] << " seconds).\n";
            hProcesses[i] = pi[i].hProcess;
        }
        else {
            std::cerr << "Error creating process on Windows!\n";
        }
    }

    std::cout << "\n[Parent] Waiting for all 3 processes to finish...\n";
    WaitForMultipleObjects(3, hProcesses, TRUE, INFINITE);

    for (int i = 0; i < 3; i++) {
        CloseHandle(pi[i].hProcess);
        CloseHandle(pi[i].hThread);
    }

#else
    int times[] = { 1, 2, 3 };

    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            std::cerr << "Error creating process with fork()!\n";
            return 1;
        }
        else if (pid == 0) {
            sleep_process(times[i]);
            return 0;
        }
        else {
            std::cout << "[Parent] Created Child Process with PID: " << pid << " (will wait for " << times[i] << " seconds).\n";
        }
    }

    std::cout << "\n[Parent] Waiting for all 3 processes to finish...\n";

    for (int i = 0; i < 3; i++) {
        int status;
        pid_t terminated_pid = wait(&status);
        std::cout << "[Parent] Child process with PID " << terminated_pid << " finished.\n";
    }

#endif

    std::cout << "\nPARENT PROCESS: All processes finished successfully!\n";
    return 0;
}