#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "paralelo_processos.h"

using namespace std;

// Multiplicando as partes das matrizrs
vector<vector<int>> multiplicarParteMatriz(const vector<vector<int>>& matriz1, vector<vector<int>>& matriz2, int start_row, int end_row) {

    int colunas1 = matriz1[0].size();
    int colunas2 = matriz2[0].size();

    vector<vector<int>> resultado(end_row - start_row, vector<int>(colunas2, 0));

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < colunas2; j++) {
            for (int k = 0; k < colunas1; k++) {
                resultado[i - start_row][j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }

    return resultado;

}

int main(int argc, char *argv[]) {
    // Verificando se a quantidade do número de argumentos foi certa
    if (argc != 3) {
        cerr << "Uso: paralelo_processos_filho [start_row] [end_row]" << endl;
        return 1;
    }

    // Pegando os valores de start_row e end_row dos argumentos
    int start_row = atoi(argv[1]);
    int end_row = atoi(argv[2]);

    // Lendo as matrizes dos arquivos
    vector<vector<int>> matriz1 = lerMatrizDoArquivo("matriz1p.txt");
    vector<vector<int>> matriz2 = lerMatrizDoArquivo("matriz2p.txt");

    // Multiplicando parte da matriz 
    vector<vector<int>> resultado_parte = multiplicarParteMatriz(matriz1, matriz2, start_row, end_row);

    // Escrevendo o resulto em um arquivo
    escreverMatrizNoArquivo(resultado_parte, "resultado_" + to_string(start_row) + "_" + to_string(end_row) + ".txt");

    return 0;
}