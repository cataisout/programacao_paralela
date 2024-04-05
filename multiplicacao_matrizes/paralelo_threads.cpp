#include <iostream>
#include <vector>
#include <thread>
#include <fstream>

using namespace std;

// le arquivos das matrizes
vector<vector<int>> lerMatrizDoArquivo(const string& nomeArquivo) {
    ifstream infile(nomeArquivo);
    
    int linhas, colunas;
    infile >> linhas >> colunas;
    
    vector<vector<int>> matriz(linhas, vector<int>(colunas));
    
    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            infile >> matriz[i][j];
        }
    }
    
    infile.close();
    
    return matriz;
}


// Função a ser executada por cada thread
void multiplyRows(const vector<vector<int>>& matriz1, const vector<vector<int>>& matriz2, vector<vector<int>>& result, int start_row, int end_row) {
    int n1 = matriz1.size();
    int m2 = matriz2[0].size();
    int m1 = matriz1[0].size();

    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < m2; ++j) {
            result[i][j] = 0;
            for (int k = 0; k < m1; ++k) {
                result[i][j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }
}


// Função para salvar o resultado da multiplicação em um arquivo
void saveResultToFile(const vector<vector<int>>& result, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        int n = result.size(); // Número de linhas da matriz resultante
        int m = result[0].size(); // Número de colunas da matriz resultante

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                file << result[i][j] << " ";
            }
            file << endl;
        }
        file.close();
    
    }
}


int main(int argc, char *argv[]) {
    if (argc < 4) {
        cerr << "Uso: " << argv[0] << " arquivo1 arquivo2 P" << endl;
        return 1;
    }

    // Ler os nomes dos arquivos e P da linha de comando
    string file1 = argv[1];
    string file2 = argv[2];
    int P = atoi(argv[3]);

    //pega as dimensoes das matrizes
    ifstream infile1(file1);
    ifstream infile2(file2);

    int n1, m1, n2, m2;

    infile1 >> n1 >> m1;
    infile2 >> n2 >> m2;
    infile1.close();
    infile2.close();

    // Abrir e ler as matrizes dos arquivos

    vector<vector<int>> matriz1 = lerMatrizDoArquivo(file1);
    vector<vector<int>> matriz2 = lerMatrizDoArquivo(file2);

    // Cria matriz resultado
    vector<vector<int>> result(n1, vector<int>(m2));
    

    // Criando threads
    vector<thread> threads;
    int num_threads = P;


    // verifica quantas linhas cada thread processará
    int rows_per_thread = (n1 + P - 1) / P; 

    //comeca a marcar o tempo de execução
    auto start_time = std::chrono::high_resolution_clock::now();

    //cria as threads
    for (int i = 0; i < P; ++i) {
        int start_row = i * rows_per_thread;
        int end_row = min((i + 1) * rows_per_thread, n1);
        threads.emplace_back(multiplyRows, ref(matriz1), ref(matriz2), ref(result), start_row, end_row);
    }       

    // Aguarda o término das threads
    for (auto& thread : threads) {
        thread.join();
    }

    // Finaliza a contagem de tempo
    auto end_time = std::chrono::high_resolution_clock::now();
    auto execution_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    string resultfile = "resultado.txt";
    // Salvando o resultado em um arquivo
    saveResultToFile(result, resultfile);


    cout << execution_time.count() << endl;

    return 0;
}
