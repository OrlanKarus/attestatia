#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
using namespace std;

// Класс оборудования оффиса
class Equipment {
public:
    string name, status; //название и статус
    int quantity; // кол-во
    //Значения по умолчанию
    Equipment(string name = "None", string status = "None", int quantity = 0) {
        this->name = name;
        this->status = status;
        this->quantity = quantity;
    }
};
//перегрузка для вывода объекта Equipment в консоль
ostream& operator<<(ostream& os, const Equipment& Equipment)
{
    os << "name: " << Equipment.name << endl << "status: " << Equipment.status <<
        endl << "quantity: " << Equipment.quantity << "\n\n";
    return os;
}

// Вывод всего массива оборудования
void print_Office(vector<Equipment> office) {
    int id = 0;
    for (Equipment equipment : office) {
        cout << id++ << endl; // вывод индекса элемента
        cout << equipment;
    }
}

//чтение json строки из файла
string jsonFile_to_string(string jsonFile) {
    ifstream ijson("office.txt");
    string json;
    getline(ijson, json);
    ijson.close();
    return json;
}

//создание массива объектов из json строки
vector<Equipment> from_JSON(string jsonFile) {
    string json = jsonFile_to_string(jsonFile);
    vector<Equipment> Office;
    string name;
    string status;
    int quantity;
    while (json.find('{') != string::npos) {
        json = json.substr(json.find(':') + 1);
        json = json.substr(json.find('"') + 1);
        name = json.substr(0, json.find('"'));

        json = json.substr(json.find(':') + 1);
        json = json.substr(json.find('"') + 1);
        status = json.substr(0, json.find('"'));

        json = json.substr(json.find(':') + 1);
        quantity = stoi(json.substr(0, json.find('}')));

        Office.push_back(Equipment(name, status, quantity));
    }
    return Office;
}

//генерация json строки и запись в файл
void to_jsonFile(vector<Equipment> Office, string jsonFile) {
    if (Office.size() > 0) {
        string to_json;
        to_json += "[";
        for (Equipment equipment : Office) {
            to_json += "{";
            to_json += "\"name\":\"" + equipment.name + "\"";
            to_json += ",\"status\":\"" + equipment.status + "\"";
            to_json += ",\"quantity\":" + to_string(equipment.quantity);
            to_json += "},";
        }
        to_json = to_json.erase(to_json.rfind(','), 1);
        to_json += "]";
        ofstream ojsonFile(jsonFile);
        ojsonFile << to_json;
        ojsonFile.close();
    }

}

int main()
{
    setlocale(LC_ALL, "Russian");
    string help = R"(
Доступные команды:
help - вывод команд
print - вывод всего оборудования из json файл
add - добавление оборудования
del - удаление оборудования по индексу
change - замена значения оборудования
exit - выход из программы
)";

    cout << "Работа с json файлом\n";
    cout << help;

    vector<Equipment> Office; //массив оборудования 
    string jsonFile = "office.txt"; //адрес json файла
    Office = from_JSON(jsonFile); //заполнение массива оборудования значениями из json файла
    string option; // команда пользователя

    while (option != "stop") {
        cin >> option;

        if (option == "print") { //печать
            print_Office(Office);
        }

        else if (option == "add") { // добавление
            cout << "Введите name: ";
            string name;
            cin >> name;
            cout << "Введите status: ";
            string status;
            cin >> status;
            cout << "Введите количестов: ";
            int quantity;
            cin >> quantity;
            Office.push_back(Equipment(name, status, quantity));
            cout << "\nОборудование добавлено\n";
        }

        else if (option == "del") { // удаление
            cout << "Введите номер элемента для удаления: ";
            int idE;
            cin >> idE;
            if (Office.size() <= idE || idE <= 0) {
                cout << "\nНет элемента под таким номером" << endl;
            }
            else {
                Office.erase(Office.begin() + idE);
                cout << "\nОборудование удалено\n";
            }
        }

        else if (option == "change") { // замена значений
            cout << "Введие номер элемента,\nкакой элемент заменить (name, status, quantity),\nна что заменить\n";
            int id;
            cin >> id;
            if (Office.size() <= id || id <= 0) {
                cout << "\nНет элемента под таким номером" << endl;
            }
            else {
                string change_option;
                cin >> change_option;
                if (change_option == "name") { // имя
                    string name;
                    cin >> name;
                    Office[id].name = name;
                    cout << "\nЗначение изменено\n";
                }
                else if (change_option == "status") { // статус
                    string status;
                    cin >> status;
                    Office[id].status = status;
                    cout << "\nЗначение изменено\n";
                }
                else if (change_option == "quantity") { // кол-во
                    int quantity;
                    cin >> quantity;
                    Office[id].quantity = quantity;
                    cout << "\nЗначение изменено\n";
                }
                else { // неправильные данные
                    cout << "\nОшибка в выборе элмента для замены" << endl;
                }
            }
        }

        else if (option == "help") {
            cout << help;
        }

        else if (option == "exit") {
            return 0;
        }

        else {
            cout << "\nНеизвестная команда";
        }
        to_jsonFile(Office, jsonFile);
    }
    return 0;
}