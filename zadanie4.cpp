#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
using namespace std;

// ����� ������������ ������
class Equipment {
public:
    string name, status; //�������� � ������
    int quantity; // ���-��
    //�������� �� ���������
    Equipment(string name = "None", string status = "None", int quantity = 0) {
        this->name = name;
        this->status = status;
        this->quantity = quantity;
    }
};
//���������� ��� ������ ������� Equipment � �������
ostream& operator<<(ostream& os, const Equipment& Equipment)
{
    os << "name: " << Equipment.name << endl << "status: " << Equipment.status <<
        endl << "quantity: " << Equipment.quantity << "\n\n";
    return os;
}

// ����� ����� ������� ������������
void print_Office(vector<Equipment> office) {
    int id = 0;
    for (Equipment equipment : office) {
        cout << id++ << endl; // ����� ������� ��������
        cout << equipment;
    }
}

//������ json ������ �� �����
string jsonFile_to_string(string jsonFile) {
    ifstream ijson("office.txt");
    string json;
    getline(ijson, json);
    ijson.close();
    return json;
}

//�������� ������� �������� �� json ������
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

//��������� json ������ � ������ � ����
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
��������� �������:
help - ����� ������
print - ����� ����� ������������ �� json ����
add - ���������� ������������
del - �������� ������������ �� �������
change - ������ �������� ������������
exit - ����� �� ���������
)";

    cout << "������ � json ������\n";
    cout << help;

    vector<Equipment> Office; //������ ������������ 
    string jsonFile = "office.txt"; //����� json �����
    Office = from_JSON(jsonFile); //���������� ������� ������������ ���������� �� json �����
    string option; // ������� ������������

    while (option != "stop") {
        cin >> option;

        if (option == "print") { //������
            print_Office(Office);
        }

        else if (option == "add") { // ����������
            cout << "������� name: ";
            string name;
            cin >> name;
            cout << "������� status: ";
            string status;
            cin >> status;
            cout << "������� ����������: ";
            int quantity;
            cin >> quantity;
            Office.push_back(Equipment(name, status, quantity));
            cout << "\n������������ ���������\n";
        }

        else if (option == "del") { // ��������
            cout << "������� ����� �������� ��� ��������: ";
            int idE;
            cin >> idE;
            if (Office.size() <= idE || idE <= 0) {
                cout << "\n��� �������� ��� ����� �������" << endl;
            }
            else {
                Office.erase(Office.begin() + idE);
                cout << "\n������������ �������\n";
            }
        }

        else if (option == "change") { // ������ ��������
            cout << "������ ����� ��������,\n����� ������� �������� (name, status, quantity),\n�� ��� ��������\n";
            int id;
            cin >> id;
            if (Office.size() <= id || id <= 0) {
                cout << "\n��� �������� ��� ����� �������" << endl;
            }
            else {
                string change_option;
                cin >> change_option;
                if (change_option == "name") { // ���
                    string name;
                    cin >> name;
                    Office[id].name = name;
                    cout << "\n�������� ��������\n";
                }
                else if (change_option == "status") { // ������
                    string status;
                    cin >> status;
                    Office[id].status = status;
                    cout << "\n�������� ��������\n";
                }
                else if (change_option == "quantity") { // ���-��
                    int quantity;
                    cin >> quantity;
                    Office[id].quantity = quantity;
                    cout << "\n�������� ��������\n";
                }
                else { // ������������ ������
                    cout << "\n������ � ������ ������� ��� ������" << endl;
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
            cout << "\n����������� �������";
        }
        to_jsonFile(Office, jsonFile);
    }
    return 0;
}