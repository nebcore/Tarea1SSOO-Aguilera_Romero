#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <unordered_set>
#include <algorithm>

std::mutex mtx; // mutex para sincronizar la salida a la consola
std::condition_variable cv; // condition variable para notificar a los threads
bool avanzar = false; // bandera para indicar si un auto debe avanzar
int autoSeleccionado = -1; // ID del auto seleccionado para avanzar
bool carreraTerminada = false; // bandera para indicar que la carrera ha terminado

// Secuencias de escape ANSI para colores
const std::string RESET = "\033[0m";
const std::string GREEN = "\033[32m";

void carreraAuto(int id, int M, std::vector<int>& distancias, std::vector<int>& ordenLlegada, int& lugar, std::mt19937& gen, std::uniform_int_distribution<>& distAvance) {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return avanzar && autoSeleccionado == id || carreraTerminada; });

        if (carreraTerminada) {
            break;
        }

        if (distancias[id] >= M) {
            return;
        }

        int avance = distAvance(gen);
        distancias[id] += avance;
        if (distancias[id] > M) {
            distancias[id] = M;
        }

        std::cout << "Auto" << id << " avanza " << avance << " metros (total: " << distancias[id] << " metros)" << std::endl;

        if (distancias[id] == M && std::find(ordenLlegada.begin(), ordenLlegada.end(), id) == ordenLlegada.end()) {
            ordenLlegada.push_back(id);
            lugar++;
            std::cout << GREEN << "Auto" << id << " ha terminado la carrera en el lugar " << lugar << "!" << RESET << std::endl;
        }

        avanzar = false;
        cv.notify_all();
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <distancia_total> <numero_autos>" << std::endl;
        return 1;
    }

    int M = std::stoi(argv[1]); // distancia total de la carrera
    int N = std::stoi(argv[2]); // cantidad de autos

    std::vector<int> distancias(N, 0); // vector que almacena la distancia recorrida por cada auto
    std::vector<int> ordenLlegada; // vector que almacena el orden de llegada de los autos
    std::unordered_set<int> autosTerminados; // conjunto para almacenar los autos que han terminado la carrera
    int lugar = 0; // contador para el lugar de llegada

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distAvance(1, 10);
    std::uniform_int_distribution<> distAuto(0, N - 1);

    std::vector<std::thread> threads;
    for (int i = 0; i < N; ++i) {
        threads.emplace_back(carreraAuto, i, M, std::ref(distancias), std::ref(ordenLlegada), std::ref(lugar), std::ref(gen), std::ref(distAvance));
    }

    do {
        {
            std::lock_guard<std::mutex> lock(mtx);
            do {
                autoSeleccionado = distAuto(gen);
            } while (autosTerminados.find(autoSeleccionado) != autosTerminados.end());
            avanzar = true;
        }
        cv.notify_all();

        // esperar a que el auto seleccionado avance
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [] { return !avanzar; });
        }

        // verificar si el auto seleccionado ha terminado la carrera
        if (distancias[autoSeleccionado] == M) {
            autosTerminados.insert(autoSeleccionado);            
        }

        // verificar si todos los autos han terminado la carrera
        if (ordenLlegada.size() == N) {
            std::lock_guard<std::mutex> lock(mtx);
            carreraTerminada = true;
            cv.notify_all(); // notificar a todas las hebras para que terminen
        }

        // esperar un tiempo aleatorio antes de la siguiente iteracion
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
    } while (!carreraTerminada);

    // esperar a que todos los threads terminen
    for (auto& t : threads) {
        t.join();
    }

    // tabla final
    std::cout << "\nLugar\tAuto" << std::endl;
    std::cout << "----------------" << std::endl;
    for (size_t i = 0; i < ordenLlegada.size(); ++i) {
        std::cout << i + 1 << "\tAuto" << ordenLlegada[i] << std::endl;
    }

    return 0;
}
