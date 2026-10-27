#include <iostream>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#endif

void adormi(int secunde) {
    std::cout << "[Proces Copil] Incep sa astept " << secunde << " secunda/secunde...\n";
    std::this_thread::sleep_for(std::chrono::seconds(secunde));
    std::cout << "[Proces Copil] Am terminat dupa " << secunde << " secunda/secunde!\n";
}

int main() {
    std::cout << "=== PROCESUL PARINTE: Start creare procese ===\n\n";

#ifdef _WIN32
    int timpi[] = { 1, 2, 3 };
    HANDLE hProcese[3];
    PROCESS_INFORMATION pi[3];
    STARTUPINFO si[3];

    for (int i = 0; i < 3; i++) {
        ZeroMemory(&si[i], sizeof(si[i]));
        si[i].cb = sizeof(si[i]);
        ZeroMemory(&pi[i], sizeof(pi[i]));

        std::string comanda = "cmd.exe /c powershell -Command \"Start-Sleep -Seconds " + std::to_string(timpi[i]) + "\"";

        BOOL succes = CreateProcessA(
            NULL,
            &comanda[0],
            NULL, NULL, FALSE, 0, NULL, NULL,
            &si[i], &pi[i]
        );

        if (succes) {
            std::cout << "[Parinte] Am creat Procesul " << (i + 1) << " (va astepta " << timpi[i] << " secunde).\n";
            hProcese[i] = pi[i].hProcess;
        }
    }

    std::cout << "\n[Parinte] Astept ca toate cele 3 procese sa se termine...\n";
    WaitForMultipleObjects(3, hProcese, TRUE, INFINITE);

    for (int i = 0; i < 3; i++) {
        CloseHandle(pi[i].hProcess);
        CloseHandle(pi[i].hThread);
    }

#else
    int timpi[] = { 1, 2, 3 };

    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            std::cerr << "Eroare la crearea procesului!\n";
            return 1;
        }
        else if (pid == 0) {
            adormi(timpi[i]);
            return 0;
        }
        else {
            std::cout << "[Parinte] Am creat Procesul Copil cu PID-ul: " << pid << " (va astepta " << timpi[i] << " secunde).\n";
        }
    }

    std::cout << "\n[Parinte] Astept ca toate cele 3 procese sa se termine...\n";

    for (int i = 0; i < 3; i++) {
        int status;
        pid_t pid_terminat = wait(&status);
        std::cout << "[Parinte] Procesul copil cu PID " << pid_terminat << " s-a incheiat.\n";
    }

#endif

    std::cout << "\n=== PROCESUL PARINTE: Toate procesele s-au terminat cu succes! ===\n";
    return 0;
}