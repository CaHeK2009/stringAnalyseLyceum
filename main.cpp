#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
#include <vector>
#include <locale>

using namespace std;

// Функция для перевода всего, что на входе, в нижний регистр. Для упрощения обработки
wstring toLowerCase(wstring text) {
    const locale ru{"ru_RU.UTF-8"};
    auto& facet = use_facet<ctype<wchar_t>>(ru);
    facet.tolower(text.data(), text.data() + text.size());
    return text;
}

wstring readText(const string& path){
    wstring text;
    wifstream fin(path);
    const locale ru{"ru_RU.UTF-8"};
    fin.imbue(ru);
    locale::global(ru);
    if (not fin.is_open()) {
        cout << "Unable to open file on path: " + path + "\n";
        return L"error";
    }
    while (!fin.eof()) {
        wstring line;
        getline(fin, line);
        text += L" ";
        text.append(line);
    }
    fin.close();
    fin.clear();
    return text;
}

vector<wstring> getWords(wstring text) {
    vector<wstring> words;
    const wstring alphabet = L"ёйцукенгшщзхъфывапролджэячсмитьбю";
    text.push_back(' ');
    bool isWord = true;
    int i = 0;
    while (!text.empty()) {
        if (alphabet.find(text[i]) == wstring::npos) {
            if (isWord) {
                words.push_back(text.substr(0, i));
                text.erase(0, i + 1);
                isWord = false;
            } else {
                text.erase(text.begin());
            }
            i = 0;
        } else {
            isWord = true;
            i++;
        }
    }
    return words;
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

    // for (int i = 1; i <= 4; i++) {
    //     cout << readText("../resources/input" + std::to_string(i) + "s.txt") << endl; // debug
    //     firstCase();
    //     secondCase();
    //     thirdCase();
    //     fourthCase();
    //     fifthCase();
    //     sixthCase();
    //     seventhCase();
    //     eighthCase();
    //     ninthCase();
    // }
    for (const wstring& word : getWords(readText("../resources/input1s.txt"))) {
        wcout << word << " ";
    }
    return 0;
}
