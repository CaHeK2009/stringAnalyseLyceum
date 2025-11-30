#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
#include <filesystem>

using namespace std;

ifstream fin("");

string read_file(string file_name){
    string s_out, s;
    ifstream fin(file_name);

    cout << endl ;
    if (not fin.is_open()) {
        cout << "Unable to open file " + file_name + "\n";
        return "error";
    }

    while (getline(fin, s)){
        cout << file_name << " : " << s << endl;
        s_out += s; 
    }

    fin.close();
    cout << endl ;
    return s_out; 
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    
    string file1, inp;

    cout << "Please enter the file name code (<1l> or <2s> etc.): ";
    getline(cin, inp);

    cout << filesystem::current_path() << endl ;
    file1 = read_file("input"+inp+".txt");
    if (file1 != "error") {
        // вставляем функции обработки здесь
        cout << file1 << endl;
    }
    else return 1;
    return 0;
}
// 1. Привести статистику встречаемости символов (по количеству и в процентах):
// а.	По всем буквам алфавита (сравнить со среднестатистической см. Приложение);
// b.	Отдельно по гласным и согласным буквам;
// c.	По использованию звонких и глухих согласных звуков;
// d.	По использованию редких согласных (ф, ч, х, ц, щ, ш, ж)
// e.	По использованию таких букв как ь, ъ, ы, й
// f.	По знакам препинания


void firstCase () {

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