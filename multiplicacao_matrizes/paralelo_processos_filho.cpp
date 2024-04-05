#include <iostream>
#include <vector>
#include <string>
#include "paralelo_processos.h"

using namespace std;

int main(int argc, char *argv[]) {
    
    // Lendo matrizes dos arquivos
    vector<vector<int>> matriz1p = lerMatrizDoArquivo("matriz1p.txt");
    vector<vector<int>> matriz2p = lerMatrizDoArquivo("matriz2p.txt");

    // Chamando a função multiplicarMatrizes com as linhas propostas
    vector<vector<int>> resultado = multiplicarMatrizes(matriz1p, matriz2p);

    // Imprimindo o resultado
    cout<< "Resultado da multiplicação: " << endl;

    for (const auto&linha : resultado) {
        for (int elemento : linha) {
            cout << elemento << " ";
        }
        cout << endl;
    }

    return 0;
}

