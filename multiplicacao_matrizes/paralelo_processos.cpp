#include <iostream>
#include <vector>
#include <fstream>
#include <windows.h> 
#include <sstream>

using namespace std;

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

void saveResultToFile(const vector<vector<int>>& result, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        int n = result.size(); 
        int m = result[0].size(); 

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

    string file1 = argv[1];
    string file2 = argv[2];
    int P = atoi(argv[3]);

    ifstream infile1(file1);
    ifstream infile2(file2);

    int n1, m1, n2, m2;

    infile1 >> n1 >> m1;
    infile2 >> n2 >> m2;
    infile1.close();
    infile2.close();

    vector<vector<int>> matriz1 = lerMatrizDoArquivo(file1);
    vector<vector<int>> matriz2 = lerMatrizDoArquivo(file2);

    vector<vector<int>> result(n1, vector<int>(m2));

    int rows_per_process = (n1 + P - 1) / P; 
    int process_count = 0;

    DWORD start_time = GetTickCount();

    for (int i = 0; i < P; ++i) {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        ZeroMemory(&pi, sizeof(pi));
        si.cb = sizeof(si);

        string command = "\"" + string(argv[0]) + "\" " + file1 + " " + file2 + " 1 " + to_string(process_count * rows_per_process) + " " + to_string(min((process_count + 1) * rows_per_process, n1));
        wstring wideCommand(command.begin(), command.end());

        if (!CreateProcess(NULL, &wideCommand[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            cerr << "Erro ao criar processo." << endl;
            return 1;
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        process_count++;
    }

    DWORD end_time = GetTickCount();
    DWORD execution_time = end_time - start_time;

    string resultfile = "resultado.txt";
    saveResultToFile(result, resultfile);

    cout << execution_time << endl;

    return 0;
}