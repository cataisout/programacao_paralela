#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <Windows.h>
#include <string>

using namespace std;

//Calcular o tempo decorrido em milissegundos
long long calcularTempoDecorrido(const chrono::steady_clock::time_point& start_time) {
    auto end_time = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);

    return duration.count();
}

// Multiplicar duas matrizes
vector<vector<int>> multiplicarMatrizes(const vector<vector<int>>& matriz1, const vector<vector<int>>& matriz2) {
    int linhas1 = matriz1.size();
    int colunas1 = matriz1[0].size();
    int linhas2 = matriz2.size();
    int colunas2 = matriz2[0].size();

    if (colunas1 != linhas2) {
        cerr << "Erro: O número de linhas e colunas não permite a multiplicação!" <<endl;
        exit(1);
    }

    vector<vector<int>> resultado(linhas1, vector<int>(colunas2, 0));

    for (int i = 0; i <linhas1; i++) {
        for (int j = 0; j < colunas2; j++) {
            for (int k = 0; k < colunas1; k++) {
                resultado[i][j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }

    return resultado;
}

// Ler matriz de um arquivo
vector<vector<int>> lerMatrizDoArquivo(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);

    if(!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo." << nomeArquivo << endl;
        exit(1);
    }

    int linhas, colunas;
    arquivo >> linhas >> colunas;

    vector<vector<int>> matriz(linhas, vector<int>(colunas));

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            arquivo >> matriz[i][j];
        }
    }

    arquivo.close();

    return matriz;
}

// Escrever matriz em um arquivo
void escreverMatrizNoArquivo(const vector<vector<int>>& matriz, const string& nomeArquivo) {
    ofstream arquivo(nomeArquivo);

    if(!arquivo.is_open()) {
        cerr << "Erro ao criar o arquivo." << nomeArquivo << endl;
        exit(1);
    }

    int linhas = matriz.size();
    int colunas = matriz[0].size();

    arquivo << linhas << " " << colunas << endl;

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            arquivo << matriz[i][j] << " ";
        }
        arquivo << endl;
    }

    arquivo.close();
}

// Multiplicando matrizes em paralelo usando processos
void multiplicarMatrizesParalelo(const vector<vector<int>>& matriz1, const vector<vector<int>>& matriz2, int P) {
    int n1 = matriz1.size();
    int n2 = matriz2[0].size();
    int num_blocos = static_cast<int>(ceil(static_cast<double>(n1 * n2) / P));

    // Armazenando os IDs os processos filhos
    HANDLE* child_handles = new HANDLE[num_blocos];

    // Armazenando os tempos de execução dos processos
    long long* tempos_execucao = new long long[num_blocos];

    // Dividindo a multiplicação das matrizes em partes, onde cada parte contém P elementos da matriz resultante
    for (int i = 0; i < num_blocos; i++) {
        auto start_time = chrono::steady_clock::now();

        int start_row = i * P;
        int end_row = min((i + 1) * P, n1);

        // Criando novo processo filho
        PROCESS_INFORMATION pi;
        STARTUPINFO si;
        ZeroMemory(&pi, sizeof(pi));
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);

        // Definindo a string normal
        string command = "paralelo_processos_filho.exe" + std::to_string(start_row) + " " + std::to_string(end_row);

        // Convertendo string normal para wide string
        wstring wide_command(command.begin(), command.end()); 

        if (!CreateProcessW(NULL, const_cast<LPWSTR>(wide_command.c_str()), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
            cerr << "Erro ao criar o processo filho." << endl;
            exit(1);
        }

        // Aguardando a conclusão do processo filho
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Obtendo o tempo de execução do processo
        long long tempo_execucao = calcularTempoDecorrido(start_time);

        // Escrevendo o tempo de execução em um array
        tempos_execucao[i] = tempo_execucao;

        // Fechando identificadores de processo
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        // Verificando os tempos de execução de cada bloco
        for (int i = 0; i < num_blocos; i++) {
            cout <<"Tempo de execução do bloco " <<i<< ": " << tempos_execucao[i] << "ms" << endl;
        }
        
    }
}