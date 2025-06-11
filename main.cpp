#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

struct Tourist {
    int passportCode;
    string surname;
    int days;

    void print() const {
        cout << left << setw(20) << surname << setw(12) << passportCode << setw(8) << days << endl;
    }
};

struct Node {
    Tourist data;
    Node* left;
    Node* right;
    Node(const Tourist& t) : data(t), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
private:
    Node* root;

    Node* insertBySurname(Node* node, const Tourist& t) {
        if (!node) {
            return new Node(t);
        }
        if (t.surname <= node->data.surname) {
            node->left = insertBySurname(node->left, t);
        } else {
            node->right = insertBySurname(node->right, t);
        }
        return node;
    }

    Node* insertByPassportCode(Node* node, const Tourist& t) {
        if (!node) {
            return new Node(t);
        }
        if (t.passportCode <= node->data.passportCode) {
            node->left = insertByPassportCode(node->left, t);
        } else {
            node->right = insertByPassportCode(node->right, t);
        }
        return node;
    }

    void printTree(Node* node, int level = 0) const {
        if (node) {
            printTree(node->right, level + 1);
            cout << string(level * 4, ' ') << "-> ";
            node->data.print();
            printTree(node->left, level + 1);
        }
    }

    void calculateDays(Node* node, int& sum, int& count) const {
        if (node) {
            sum += node->data.days;
            count++;
            calculateDays(node->left, sum, count);
            calculateDays(node->right, sum, count);
        }
    }

    void deleteTree(Node*& node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
            node = nullptr;
        }
    }

    void collectData(Node* node, vector<Tourist>& data) const {
        if (node) {
            data.push_back(node->data);
            collectData(node->left, data);
            collectData(node->right, data);
        }
    }

public:
    BinarySearchTree() : root(nullptr) {}

    ~BinarySearchTree() {
        deleteTree(root);
    }

    void insertBySurname(const Tourist& t) {
        root = insertBySurname(root, t);
    }

    void insertByPassportCode(const Tourist& t) {
        root = insertByPassportCode(root, t);
    }

    void print() const {
        cout << left << setw(20) << "Прізвище" << setw(12) << "Паспорт" << setw(8) << "Дні" << endl;
        cout << string(40, '-') << endl;
        if (!root) {
            cout << "Дерево порожнє!" << endl;
        } else {
            printTree(root);
        }
        cout << endl;
    }

    double averageDays() const {
        int sum = 0, count = 0;
        calculateDays(root, sum, count);
        return count > 0 ? static_cast<double>(sum) / count : 0.0;
    }

    vector<Tourist> collectAndClear() {
        vector<Tourist> data;
        collectData(root, data);
        deleteTree(root);
        return data;
    }

    bool isEmpty() const {
        return root == nullptr;
    }
};

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    const int T = 12; 
    BinarySearchTree treeBySurname;

    cout << "Введіть дані для " << T << " туристів:\n";
    for (int i = 0; i < T; i++) {
        Tourist t;
        cout << "\nТурист " << i + 1 << ":\n";
        cout << "Прізвище: ";
        cin >> t.surname;
        if (t.surname.empty()) {
            cout << "Помилка: Прізвище не може бути порожнім!\n";
            i--;
            continue;
        }

        cout << "Код паспорта (ціле число > 0): ";
        while (!(cin >> t.passportCode) || t.passportCode <= 0) {
            cout << "Помилка: Введіть ціле число більше 0!\n";
            clearInputBuffer();
        }

        cout << "Кількість днів (ціле число >= 0): ";
        while (!(cin >> t.days) || t.days < 0) {
            cout << "Помилка: Введіть невід'ємне ціле число!\n";
            clearInputBuffer();
        }
        clearInputBuffer();

        treeBySurname.insertBySurname(t);
    }

    cout << "\nДвійкове дерево, організоване за прізвищем:\n";
    treeBySurname.print();

    if (!treeBySurname.isEmpty()) {
        cout << "Середнє значення кількості днів: " << fixed << setprecision(2) << treeBySurname.averageDays() << endl;
    } else {
        cout << "Середнє значення не може бути обчислене: дерево порожнє!\n";
    }

    cout << "\nПеренесення даних у нове дерево, організоване за кодом паспорта...\n";
    vector<Tourist> tourists = treeBySurname.collectAndClear(); 
    BinarySearchTree treeByPassportCode;
    for (const auto& t : tourists) {
        treeByPassportCode.insertByPassportCode(t);
    }

    cout << "Перевірка: перше дерево " << (treeBySurname.isEmpty() ? "порожнє" : "не порожнє") << endl;

    cout << "\nДвійкове дерево, організоване за кодом паспорта:\n";
    treeByPassportCode.print();

    return 0;
}