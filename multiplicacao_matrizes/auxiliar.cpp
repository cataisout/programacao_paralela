#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <locale>


using namespace std;

// Função para gerar uma matriz aleatória com dimensões n x m
void gerarMatrizAleatoria(int n, int m, const string& nomeArquivo) {
    ofstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo " << nomeArquivo << endl;
        exit(1);
    }

    arquivo << n << " " << m << endl; // Escreve as dimensões no arquivo

    // Preenche a matriz com valores aleatórios entre 1 e 100
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            arquivo << rand() % 100 + 1 << " ";
        }
        arquivo << endl;
    }

    arquivo.close();
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    
    if (argc != 5) {
        cerr << "Uso: " << argv[0] << " n1 m1 n2 m2" << endl;
        return 1;
    }

    // Obtém as dimensões das matrizes da linha de comando
    int n1 = atoi(argv[1]);
    int m1 = atoi(argv[2]);
    int n2 = atoi(argv[3]);
    int m2 = atoi(argv[4]);

    if (m1 != n2) {
        cerr << "Dimensões inválidas para a operação de multiplicação de matrizes" << endl;
        return 1;
    }

    // Inicializa o gerador de números aleatórios
    srand(time(nullptr));

    // Gera e salva a primeira matriz (M1)
    gerarMatrizAleatoria(n1, m1, "matriz1.txt");
    cout << "Matriz 1 gerada e salva em matriz1.txt" << endl;

    // Gera e salva a segunda matriz (M2)
    gerarMatrizAleatoria(n2, m2, "matriz2.txt");
    cout << "Matriz 2 gerada e salva em matriz2.txt" << endl;

    return 0;
}
