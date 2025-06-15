#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <limits>
#include "sum.h"

using std::cout;



using namespace std;

class Video {
protected:
    string id, nombre, genero;
    double duracion;
    int calificacion;
public:
    Video(string id, string nombre, double duracion, int calificacion, string genero)
        : id(id), nombre(nombre), duracion(duracion), calificacion(calificacion), genero(genero) {}

    virtual void mostrar() const = 0;
    virtual void calificar(int nuevaCalificacion) {
        if (nuevaCalificacion >= 1 && nuevaCalificacion <= 5)
            calificacion = nuevaCalificacion;
    }

    string getGenero() const { return genero; }
    int getCalificacion() const { return calificacion; }
    string getNombre() const { return nombre; }
    virtual bool esPelicula() const = 0;
    virtual ~Video() {}
};

class Pelicula : public Video {
public:
    Pelicula(string id, string nombre, double duracion, int calificacion, string genero)
        : Video(id, nombre, duracion, calificacion, genero) {}

    void mostrar() const override {
        cout << "Pelicula - " << nombre << " | ID: " << id
             << " | Duracion: " << duracion << " mins | Calificacion: " << calificacion
             << " | Genero: " << genero << endl;
    }

    bool esPelicula() const override {
        return true;
    }
};

struct Episodio {
    string titulo;
    int temporada;
    int calificacion;

    void mostrar() const {
        cout << "  Episodio: " << titulo << " | Temporada: " << temporada
             << " | Calificacion: " << calificacion << endl;
    }
};

class Serie : public Video {
private:
    vector<Episodio> episodios;
public:
    Serie(string id, string nombre, double duracion, int calificacion, string genero)
        : Video(id, nombre, duracion, calificacion, genero) {}

    void agregarEpisodio(string titulo, int temporada, int calif) {
        episodios.push_back({titulo, temporada, calif});
    }

    void mostrar() const override {
        cout << "Serie - " << nombre << " | ID: " << id
             << " | Calificacion promedio: " << calificacion << " | Genero: " << genero << endl;
        for (const auto& ep : episodios)
            ep.mostrar();
    }

    bool esPelicula() const override {
        return false;
    }

    void mostrarEpisodiosConCalificacion(int calif) const {
        for (const auto& ep : episodios) {
            if (ep.calificacion == calif)
                ep.mostrar();
        }
    }
};

void mostrarMenu() {
    cout << "\n--- Menu ---\n"
         << "1. Cargar archivo de datos\n"
         << "2. Mostrar videos por calificacion o genero\n"
         << "3. Mostrar episodios de una serie por calificacion\n"
         << "4. Mostrar peliculas por calificacion\n"
         << "5. Calificar un video\n"
         << "0. Salir\n"
         << "Opcion: ";
}

int main() {
    std::cout << "sum(1, 5) = " << sum(1, 5) << '\n';
    vector<shared_ptr<Video>> videos;
    int opcion;

    do {
        mostrarMenu();

        while (!(cin >> opcion) || opcion < 0 || opcion > 5) {
            cout << "Entrada invalida. Ingresa un numero entre 0 y 5: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore();

        if (opcion == 1) {
            videos.clear();

            videos.push_back(make_shared<Pelicula>("P001", "Matrix", 136, 5, "accion"));
            videos.push_back(make_shared<Pelicula>("P002", "Inception", 148, 4, "misterio"));
            videos.push_back(make_shared<Pelicula>("P003", "The Godfather", 175, 5, "drama"));

            auto s1 = make_shared<Serie>("S001", "Breaking Bad", 60, 5, "drama");
            s1->agregarEpisodio("Piloto", 1, 5);
            s1->agregarEpisodio("Cat's in the Bag...", 1, 4);
            videos.push_back(s1);

            auto s2 = make_shared<Serie>("S002", "Stranger Things", 50, 4, "misterio");
            s2->agregarEpisodio("The Vanishing", 1, 4);
            s2->agregarEpisodio("The Weirdo", 1, 3);
            videos.push_back(s2);

            auto s3 = make_shared<Serie>("S003", "The Mandalorian", 45, 4, "accion");
            s3->agregarEpisodio("Chapter 1", 1, 5);
            s3->agregarEpisodio("Chapter 2", 1, 4);
            videos.push_back(s3);

            cout << "\nVideos creados:\n";
            for (const auto& v : videos)
                v->mostrar();
        }

        else if (opcion == 2) {
            cout << "Filtrar por (g)enero o (c)alificacion? ";
            char filtro;
            cin >> filtro;
            cin.ignore();

            if (filtro == 'g') {
                string genero;
                cout << "Genero (drama, accion, misterio): ";
                getline(cin, genero);
                for (const auto& v : videos)
                    if (v->getGenero() == genero)
                        v->mostrar();
            } else if (filtro == 'c') {
                int calif;
                cout << "Calificacion (1-5): ";
                while (!(cin >> calif) || calif < 1 || calif > 5) {
                    cout << "Entrada invalida. Ingresa un numero entre 1 y 5: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                cin.ignore();
                for (const auto& v : videos)
                    if (v->getCalificacion() == calif)
                        v->mostrar();
            }
        }

        else if (opcion == 3) {
            string nombreSerie;
            int calif;
            cout << "Nombre de la serie: ";
            getline(cin, nombreSerie);
            cout << "Calificacion a filtrar (1-5): ";
            while (!(cin >> calif) || calif < 1 || calif > 5) {
                cout << "Entrada invalida. Ingresa un numero entre 1 y 5: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cin.ignore();
            for (const auto& v : videos) {
                if (!v->esPelicula() && v->getNombre() == nombreSerie) {
                    dynamic_cast<Serie*>(v.get())->mostrarEpisodiosConCalificacion(calif);
                }
            }
        }

        else if (opcion == 4) {
            int calif;
            cout << "Calificacion (1-5): ";
            while (!(cin >> calif) || calif < 1 || calif > 5) {
                cout << "Entrada invalida. Ingresa un numero entre 1 y 5: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cin.ignore();
            for (const auto& v : videos)
                if (v->esPelicula() && v->getCalificacion() == calif)
                    v->mostrar();
        }

        else if (opcion == 5) {
            string nombre;
            int nuevaCalif;
            cout << "Titulo del video a calificar: ";
            getline(cin, nombre);
            cout << "Nueva calificacion (1-5): ";
            while (!(cin >> nuevaCalif) || nuevaCalif < 1 || nuevaCalif > 5) {
                cout << "Entrada invalida. Ingresa un numero entre 1 y 5: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cin.ignore();

            bool encontrado = false;
            for (auto& v : videos) {
                if (v->getNombre() == nombre) {
                    v->calificar(nuevaCalif);
                    cout << "Calificacion actualizada a " << nuevaCalif << " para \"" << nombre << "\".\n";
                    v->mostrar();
                    encontrado = true;
                    break;
                }
            }
            if (!encontrado) cout << "Video no encontrado.\n";
        }

    } while (opcion != 0);

    cout << "Programa finalizado.\n";
    return 0;
}
