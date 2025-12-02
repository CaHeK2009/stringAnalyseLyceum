#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
#include <vector>

using namespace std;

string readText(const string& path){
    string text;
    ifstream fin(path);
    if (not fin.is_open()) {
        cout << "Unable to open file on path: " + path + "\n";
        return "error";
    }
    fin.close();
    fin.clear();
    return text;
}

vector<string> getWords(string& text) {
    vector<string> words;
    const string alphabet = "qwertyuiopasdfghjklzxcvbnmёйцукенгшщзхъфывапролджэячсмитьбюЁЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮQWERTYUIOPASDFGHJKLZXCVBNM";
    text = ' ' + text + ' ';
    bool isWord = false;
    while (!text.empty()) {
        if (alphabet.find(text[0]) == string::npos) {
            if (isWord) {

            } else {
                text.erase(text.begin());
            }
        }
    }
}
// 1. Привести статистику встречаемости символов (по количеству и в процентах):
// а.	По всем буквам алфавита (сравнить со среднестатистической см. Приложение);
// b.	Отдельно по гласным и согласным буквам;
// c.	По использованию звонких и глухих согласных звуков;
// d.	По использованию редких согласных (ф, ч, х, ц, щ, ш, ж)
// e.	По использованию таких букв как ь, ъ, ы, й
// f.	По знакам препинания
void firstCase() {

}
// 2.	Привести статистику по длине слов и предложений.
void secondCase () {

}
// 3.	Привести статистику по предлогам и союзам (наиболее часто встречающимся)
void thirdCase () {

}
// 4.	Привести статистику по буквосочетаниям, наиболее часто встречающимся в русском языке:
//      СТ, НО, ЕН, ТО, НА, ОВ, НИ, РА, ВО, КО; СТО, ЕНО, НОВ, ТОВ, ОВО, ОВА
void fourthCase () {

}
// 5.	Привести статистику по чередованию гласных и согласных букв.
//      Для каждой гласной (согласной) буквы определить гласная или согласная буква стоит после нее (перед ней).
void fifthCase () {

}
// 6.	Привести статистику по парам слов, начинающихся/заканчивающихся на гласную-согласную (по всем парам: ГГ, ГС, СГ, СС).
void sixthCase () {

}
// 7.	Привести статистику по количеству сочетания трех гласны и трех согласных в слове подряд.
void seventhCase () {

}
// 8.	Привести статистику по количеству пар слов, первое из которых заканчивается на 2-3 гласные, а второе начинается с 2-3 согласных.
void eighthCase () {

}
// 9.	Найти наиболее часто встречающиеся слово (слова).
void ninthCase () {

}

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    for (int i = 1; i <= 4; i++) {
        cout << readText("../resources/input" + std::to_string(i) + "s.txt") << endl; // debug
        firstCase();
        secondCase();
        thirdCase();
        fourthCase();
        fifthCase();
        sixthCase();
        seventhCase();
        eighthCase();
        ninthCase();
    }
    return 0;
}
