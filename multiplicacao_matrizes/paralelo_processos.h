#ifndef PARALELO_PROCESSOS_H
#define PARALELO_PROCESSOS_H

#include <vector>
#include <string>

struct ResultadoParte {
    std::vector<std::vector<int>> resultado;
    long long tempo_execucao;
};

std::vector<std::vector<int>> multiplicarMatrizes(const std::vector<std::vector<int>>& matriz1p, const std::vector<std::vector<int>>& matriz2p, int start_row, int end_row);

std::vector<std::vector<int>> lerMatrizDoArquivo(const std::string& arquivop);

void escreverMatrizNoArquivo(const std::vector<std::vector<int>>& matriz, const std::string& nomeArquivo);

std::vector<ResultadoParte> multiplicarMatrizesParalelo(const std::vector<std::vector<int>>& matriz1, const std::vector<std::vector<int>>& matriz2);

#endif