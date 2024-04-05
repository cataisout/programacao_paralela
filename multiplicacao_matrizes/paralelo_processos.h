#ifndef PARALELO_PROCESSOS_H
#define PARALELO_PROCESSOS_H

#include <vector>
#include <string>

std::vector<std::vector<int>> multiplicarMatrizes(const std::vector<std::vector<int>>& matriz1p, const std::vector<std::vector<int>>& matriz2p, int start_row, int end_row);

std::vector<std::vector<int>> lerMatrizDoArquivo(const std::string& arquivop);

#endif