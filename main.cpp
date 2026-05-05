#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

struct Nodo {
    int valor;
    int indice_original;
    int indice_abajo;
};

vector<vector<Nodo>> listas_aumentadas;
vector<vector<int>> listas_originales;

void construir(vector<vector<int>> listas) {
    listas_originales = listas;
    int k = listas.size();
    listas_aumentadas.resize(k);

    for (int j = 0; j < listas[k - 1].size(); j++) {
        Nodo nuevo;
        nuevo.valor = listas[k - 1][j];
        nuevo.indice_original = j;
        nuevo.indice_abajo = -1;
        listas_aumentadas[k - 1].push_back(nuevo);
    }

    for (int i = k - 2; i >= 0; i--) {
        vector<Nodo> mitad;
        for (int j = 1; j < listas_aumentadas[i + 1].size(); j += 2) {
            mitad.push_back(listas_aumentadas[i + 1][j]);
        }

        int indice_actual = 0;
        int indice_mitad = 0;
        int tam_actual = listas_originales[i].size();
        int tam_mitad = mitad.size();

        while (indice_actual < tam_actual || indice_mitad < tam_mitad) {
            
            if (indice_mitad == tam_mitad || (indice_actual < tam_actual && listas_originales[i][indice_actual] <= mitad[indice_mitad].valor)) {
                int abajo = indice_mitad * 2;
                if (abajo >= listas_aumentadas[i + 1].size()) {
                    abajo = listas_aumentadas[i + 1].size() - 1;
                }
                if (abajo < 0) abajo = 0;
                
                Nodo n;
                n.valor = listas_originales[i][indice_actual];
                n.indice_original = indice_actual;
                n.indice_abajo = abajo;
                listas_aumentadas[i].push_back(n);
                indice_actual++;
            } 
            else {
                int abajo = indice_mitad * 2 + 1;
                Nodo n;
                n.valor = mitad[indice_mitad].valor;
                n.indice_original = indice_actual; 
                n.indice_abajo = abajo;
                listas_aumentadas[i].push_back(n);
                indice_mitad++;
            }
        }
    }
}

vector<int> buscar_cascada(int objetivo) {
    vector<int> resultados;
    int k = listas_aumentadas.size();
    
    int izq = 0;
    int der = listas_aumentadas[0].size() - 1;
    int pos = der + 1;

    while (izq <= der) {
        int medio = (izq + der) / 2;
        if (listas_aumentadas[0][medio].valor >= objetivo) {
            pos = medio;
            der = medio - 1;
        } else {
            izq = medio + 1;
        }
    }

    for (int i = 0; i < k; i++) {
        if (pos < listas_aumentadas[i].size()) {
            int original = listas_aumentadas[i][pos].indice_original;
            while (original < listas_originales[i].size() && listas_originales[i][original] < objetivo) {
                original++;
            }
            if (original < listas_originales[i].size()) {
                resultados.push_back(listas_originales[i][original]);
            } else {
                resultados.push_back(-1); // -1 significa que no hay un número mayor o igual
            }
        } else {
            resultados.push_back(-1);
        }

        // Bajar a la siguiente lista usando el puntero de cascada
        if (i < k - 1) {
            if (pos < listas_aumentadas[i].size()) {
                pos = listas_aumentadas[i][pos].indice_abajo;
            } else {
                pos = listas_aumentadas[i + 1].size();
            }
            
            while (pos > 0 && listas_aumentadas[i + 1][pos - 1].valor >= objetivo) {
                pos--;
            }
        }
    }
    return resultados;
}

vector<int> buscar_normal(int objetivo) {
    vector<int> resultados;
    for (int i = 0; i < listas_originales.size(); i++) {
        int izq = 0;
        int der = listas_originales[i].size() - 1;
        int pos = -1;
        
        while (izq <= der) {
            int medio = (izq + der) / 2;
            if (listas_originales[i][medio] >= objetivo) {
                pos = medio;
                der = medio - 1;
            } else {
                izq = medio + 1;
            }
        }
        
        if (pos != -1) {
            resultados.push_back(listas_originales[i][pos]);
        } else {
            resultados.push_back(-1);
        }
    }
    return resultados;
}

int main() {
    int k = 100; 
    int n = 10000; 
    vector<vector<int>> listas(k, vector<int>(n));

    int valor = 0;
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < n; j++) {
            valor = valor + (rand() % 5) + 1;
            listas[i][j] = valor;
        }
    }

    construir(listas);

    vector<int> buscar;
    for (int i = 0; i < 100000; i++) {
        buscar.push_back(rand() % valor);
    }

    int inicio_normal = clock();
    for (int i = 0; i < buscar.size(); i++) {
        buscar_normal(buscar[i]);
    }
    int fin_normal = clock();

    int inicio_cascada = clock();
    for (int i = 0; i < buscar.size(); i++) {
        buscar_cascada(buscar[i]);
    }
    int fin_cascada = clock();

    cout << "Prueba terminada con " << k << " listas de " << n << " elementos." << endl;
    cout << "--------------------------------------------------" << endl;
    cout << "Tiempo de busqueda normal: " << fin_normal - inicio_normal << " ticks" << endl;
    cout << "Tiempo con cascada fraccional: " << fin_cascada - inicio_cascada << " ticks" << endl;

    return 0;
}