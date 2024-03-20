#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;

// Função para multiplicar duas matrizes
vector<vector<int>> multiplicarMatrizes(const vector<vector<int>>& matriz1, const vector<vector<int>>& matriz2) {
    int linhas1 = matriz1.size();
    int colunas1 = matriz1[0].size();
    int linhas2 = matriz2.size();
    int colunas2 = matriz2[0].size();

    if (colunas1 != linhas2) {
        cerr << "Erro: As dimensões das matrizes não são compatíveis para multiplicação." << endl;
        exit(1);
    }

    vector<vector<int>> resultado(linhas1, vector<int>(colunas2, 0));

    for (int i = 0; i < linhas1; ++i) {
        for (int j = 0; j < colunas2; ++j) {
            for (int k = 0; k < colunas1; ++k) {
                resultado[i][j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }

    return resultado;
}

// Função para ler uma matriz de um arquivo
vector<vector<int>> lerMatrizDoArquivo(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo " << nomeArquivo << endl;
        exit(1);
    }

    int linhas, colunas;
    arquivo >> linhas >> colunas;

    vector<vector<int>> matriz(linhas, vector<int>(colunas));

    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            arquivo >> matriz[i][j];
        }
    }

    arquivo.close();
    return matriz;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Uso: " << argv[0] << " arquivo_matriz1 arquivo_matriz2 arquivo_saida" << endl;
        return 1;
    }

    string arquivoMatriz1 = argv[1];
    string arquivoMatriz2 = argv[2];
    string arquivoSaida = argv[3];

    // Ler as matrizes dos arquivos de entrada
    vector<vector<int>> matriz1 = lerMatrizDoArquivo(arquivoMatriz1);
    vector<vector<int>> matriz2 = lerMatrizDoArquivo(arquivoMatriz2);

    ofstream arquivoResultado(arquivoSaida);
    if (!arquivoResultado.is_open()) {
        cerr << "Erro ao criar o arquivo de saída " << arquivoSaida << endl;
        return 1;
    }

    // Realizar a multiplicação das matrizes 10 vezes
    for (int iteracao = 1; iteracao <= 10; ++iteracao) {
        auto inicio = chrono::steady_clock::now();

        vector<vector<int>> resultado = multiplicarMatrizes(matriz1, matriz2);

        auto fim = chrono::steady_clock::now();
        auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio).count();

        arquivoResultado << "Iteração " << iteracao << ": Tempo de execução = " << duracao << "ms" << endl;

        // Se for a última iteração, também salvar o resultado da multiplicação
        if (iteracao == 10) {
            arquivoResultado << "Resultado da multiplicação:" << endl;
            for (const auto& linha : resultado) {
                for (int valor : linha) {
                    arquivoResultado << valor << " ";
                }
                arquivoResultado << endl;
            }
        }
    }

    arquivoResultado.close();
    cout << "Multiplicação concluída. Resultados salvos em " << arquivoSaida << endl;

    return 0;
}
