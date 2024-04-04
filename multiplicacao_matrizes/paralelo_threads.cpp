#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <ctime>
#include <cmath>
#include <cstdlib>

using namespace std;

long long CalculaDuracao(vector<thread>& threads) {
    auto start_time = chrono::high_resolution_clock::now();
    for (auto& thread_i : threads) {
        thread_i.join();
    }
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    return duration.count();
}

// Estrutura para armazenar os parâmetros de cada thread
struct ThreadData {
    vector<vector<int>>& M1;
    vector<vector<int>>& M2;
    vector<vector<int>>& result;
    int start_row;
    int end_row;
};

// Função que cada thread executará
void multiplyRows(ThreadData* data, vector<vector<int>>& result) {

    for (int i = data->start_row; i < data->end_row; ++i) {

        for (size_t j = 0; j < data->M2[0].size(); ++j) {

            int sum = 0;
            for (size_t k = 0; k < data->M1[i].size(); ++k) {

                sum += data->M1[i][k] * data->M2[k][j];
            }
            result[i][j] = sum;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Uso: " << argv[0] << " arquivo1 arquivo2 P" << endl;
        return 1;
    }

    // Ler os nomes dos arquivos e P da linha de comando
    string file1 = argv[1];
    string file2 = argv[2];
    int P = atoi(argv[3]);

    // Abrir e ler as matrizes dos arquivos
    ifstream infile1(file1);
    ifstream infile2(file2);

    int n1, m1, n2, m2;
    infile1 >> n1 >> m1;
    infile2 >> n2 >> m2;

    if (m1 != n2) {
        cerr << "Erro: as dimensões das matrizes não são compatíveis para multiplicação." << endl;
        return 1;
    }

    vector<vector<int>> M1(n1, vector<int>(m1));
    vector<vector<int>> M2(n2, vector<int>(m2));


    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < m1; ++j) {
            infile1 >> M1[i][j];
        }
    }

    for (int i = 0; i < n2; ++i) {
        for (int j = 0; j < m2; ++j) {
            infile2 >> M2[i][j];
        }
    }

    infile1.close();
    infile2.close();

    


    // Criar threads para calcular a multiplicação de matrizes
    vector<thread> threads;
    vector<vector<int>> result(n1, vector<int>(m2));
    int num_threads = static_cast<int>(ceil(static_cast<double>(n1 * m2) / P));

    for (int i = 0; i < num_threads; ++i) {
        int start_row = i * P;
        int end_row = min((i + 1) * P, n1);
        ThreadData* data = new ThreadData {M1, M2, result, start_row, end_row};

        threads.emplace_back(multiplyRows, data, std::ref(result));

    }


    long long execution_time = CalculaDuracao(threads);

    

    // Salvar os resultados em arquivos
    for (int i = 0; i < num_threads; ++i) {
        string output_file = "resultado_" + to_string(i) + ".txt";
        ofstream outfile(output_file);
        outfile << n1 << " " << m2 << endl;
        for (int row = i * P; row < min((i + 1) * P, n1); ++row) {
            for (int col = 0; col < m2; ++col) {
                outfile << result[row][col] << " ";
            }
            outfile << endl;
        }
        outfile.close();
    }


    
    cout << "Tempo: " << execution_time << " [ms]" << endl;
    

    return 0;
}
