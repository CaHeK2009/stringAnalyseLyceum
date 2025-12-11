#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <locale>

using namespace std;

ofstream fout("../resouces/output_l.txt");

string dir_name = "../resources/"; // Рабочий каталог с файлами
vector<string> texts(4); // Массив с текстами
vector<string> headers = { // Массив с критериями
    "Всего букв"            // 0
    , "Гласные"             // 1
    , "Согласные"           // 2
    , "Звонкие согласные"   // 3
    , "Глухие согласные"    // 4
    , "Редкие согласные"    // 5
    , "ь, ъ, ы, й"          // 6
    , "Знаки препинания"    // 7
    , "а"                   // 8
    , "б"                   // 9
    , "в"                   // 10
    , "г"                   // 11
    , "д"                   // 12
    , "е"                   // 13
    , "ё"                   // 14
    , "ж"                   // 15
    , "з"                   // 16
    , "и"                   // 17
    , "й"                   // 18
    , "к"                   // 19
    , "л"                   // 20
    , "м"                   // 21
    , "н"                   // 22
    , "о"                   // 23
    , "п"                   // 24
    , "р"                   // 25
    , "с"                   // 26
    , "т"                   // 27
    , "у"                   // 28
    , "ф"                   // 29
    , "х"                   // 30
    , "ц"                   // 31
    , "ч"                   // 32
    , "ш"                   // 33
    , "щ"                   // 34
    , "ъ"                   // 35
    , "ы"                   // 36
    , "ь"                   // 37
    , "э"                   // 38
    , "ю"                   // 39
    , "я"                   // 40
    , "Пробел"              // 41
    , "Всего слов"          // 42
    , "Средняя длина слова" // 43
    , "Количество предложений"              // 44
    , "Среднее число слов в предложении"    // 45
} ;
// Гласные буквы
const string vowels = "аеёиоуыэюя";
// Согласные буквы
const string consonants = "бвгджзйклмнпрстфхцчшщьъ";
// Весь алфавит
// const string alphabet = "ёйцукенгшщзхъфывапролджэячсмитьбюabcdefghijklmnopqrstuvwxyz";
const string alphabet = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
// Звонкие согласные
const string voiced = "бвгдзжлмнрй";  // й - сонорный, но условно к звонким
// Глухие согласные
const string voiceless = "пфктшсхчщц";
// Редкие согласные
const string rare_consonants = "фхцчшщж";
// ь, ъ, ы, й
const string sq = "ьъый" ;
// Знаки препинания
const string punctuation_marks = ".,:;?!-()\"\'" ;
// Встречаемость букв русского языка
vector<double> avg_freq = {
    // абвгдеёжзий
    0.064, 0.015, 0.039, 0.014, 0.026, 0.074, 0.074, 0.008, 0.015, 0.064, 0.010,
    // клмнопрстуф
    0.029, 0.036, 0.026, 0.056, 0.095, 0.024, 0.041, 0.047, 0.056, 0.021, 0.002,
    // хцчшщъыьэюя
    0.009, 0.004, 0.013, 0.006, 0.004, 0.015, 0.016, 0.015, 0.003, 0.007, 0.019
    // пробел
    , 0.145
};

// Предлоги и союзы для анализа
const vector<string> prepositions = {
    "в", "на", "с", "по", "из", "за", "к", "от", "до", "у",
    "для", "о", "при", "под", "над", "про", "со", "без", "через"
};

const vector<string> conjunctions = {
    "и", "а", "но", "или", "что", "как", "чтобы", "если", "когда",
    "потому", "так", "тоже", "либо", "ни", "да", "зато"
};

// для упрощения подсчета предлогов и союзов
vector<int> total_preps(4, 0);
vector<int> total_conjs(4, 0);
vector<string> top_preps(4, "-");
vector<string> top_conjs(4, "-");

// Функция для перевода всего, что на входе, в нижний регистр. Для упрощения обработки
wstring toLowerCase(wstring text) {
    const locale ru{"ru_RU.UTF-8"};
    auto& facet = use_facet<ctype<wchar_t>>(ru);
    facet.tolower(text.data(), text.data() + text.size());
    return text;
}

string readTextCP1251(const string& path){
    string text, line;
    ifstream fin(path);
    // const locale ru{".1251"};
    // fin.imbue(ru);
    // locale::global(ru);
    if (not fin.is_open()) {
        cout << "Unable to open file on path: " + path + "\n";
        return "error";
    }
    // while (!fin.eof()) {
    while (getline(fin, line)) {
        // text += L" ";
        text.append(line);
    }
    fin.close();
    fin.clear();
    return text;
}

vector<string> getWordsCP1251(string text) {
    vector<string> words;
    // const string alphabet = "ёйцукенгшщзхъфывапролджэячсмитьбю";
    text.push_back(' ');
    bool isWord = true;
    int i = 0;
    while (!text.empty()) {
        if (alphabet.find(text[i]) == string::npos) {
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

// Функция для перевода всего, что на входе, в нижний регистр. Для упрощения обработки
string to_lower(string text) {
    for (int i = 0; i < text.length(); i++) {
        if (text[i] >= 'А' && text[i] <= 'Я') {
            text[i] = text[i] - 'А' + 'а';
        } else if (text[i] >= 'A' && text[i] <= 'Z') {
            text[i] = text[i] - 'A' + 'a';
        }
    }
    return text;
}

// Функция для упрощения вывода - выравнивание по левому краю (добавляет пробелы справа)
string rpad(const string& str, int width, char filler = ' ') {
    string result = str;
    if (result.length() < width) {
        result.append(width - result.length(), filler);
    }
    return result;
}

// Функция для упрощения вывода - выравнивание по правому краю (добавляет пробелы слева)
string lpad(const string& str, int width, char filler = ' ') {
    string result;
    if (str.length() < width) {
        result.append(width - str.length(), filler);
    }
    result.append(str);
    return result;
}

// 1. Привести статистику встречаемости символов (по количеству и в процентах):
// а.	По всем буквам алфавита (сравнить со среднестатистической см. Приложение);
// b.	Отдельно по гласным и согласным буквам;
// c.	По использованию звонких и глухих согласных звуков;
// d.	По использованию редких согласных (ф, ч, х, ц, щ, ш, ж)
// e.	По использованию таких букв как ь, ъ, ы, й
// f.	По знакам препинания
// void firstCase() {

string firstCase(string text, const int scope = 0, const char s_sym = ' ') {
    const string lower_text = to_lower(std::move(text));

    int total_letters = 0;
    int vowel_count = 0;
    int consonants_count = 0;
    int voiced_count = 0;
    int voiceless_count = 0;
    int rare_count = 0;
    int sd_count = 0;
    int pm_count = 0;
    vector<int> l_counts(34, 0);
    int word_count = 0;
    int current_word_length = 0;
    int total_word_length = 0;
    int sentence_count = 0;

    for (int i = 0; i < lower_text.length(); i++) {
        const char c = lower_text[i];
        if ((c >= 'а' && c <= 'я') || c == 'ё') { // 'ё' стоит отдельно от остальных букв в кодовой таблице символов
            total_letters++;
            const int p = alphabet.find(c) ;
            l_counts[p]++;
            current_word_length++;
            if (vowels.find(c) != string::npos) vowel_count++;
            if (consonants.find(c) != string::npos) { // Могли бы просто сказать 'else',
                // но тем самым гарантируем, что ни одна из букв не потеряется. Все будет видно на общей сумме букв
            // else {
                consonants_count++;
                if (voiced.find(c) != string::npos) voiced_count++;
                else if (voiceless.find(c) != string::npos) voiceless_count++;
                if (rare_consonants.find(lower_text[i]) != string::npos) rare_count++;
            }
            if (c == 'ы' || c == 'й' || c == 'ь' || c == 'ъ') sd_count++;
        }
        else {
            // если нашли многоточие...
            if ((i<(lower_text.length()-2)) && c == '.' && lower_text[i+1] == '.'
                && lower_text[i+2] == '.') {
                    pm_count++ ; sentence_count++ ; i=i+2; continue;
                }
            // другие знаки препинания
            if (punctuation_marks.find(c) != string::npos) pm_count++;
            // слова
            if (current_word_length > 0) {
                word_count++;
                total_word_length += current_word_length;
                current_word_length = 0;
            }
        }
        // считаем пробел отдельно, последним индексом
        if (c == ' ') l_counts[33]++;
        // границы предложений
        if (c == '.' || c == '!' || c == '?') sentence_count++;
    }

    switch (scope) {
        case 0 : {// информация по гласным
            double vowel_percent;
            if (total_letters > 0) vowel_percent = (vowel_count * 100.0) / total_letters;
            else vowel_percent = 0;
            return to_string(vowel_count) +
                "(" + to_string(vowel_percent).substr(0,5) + "%)";
            break;
        }
        case 10 : {// информация по согласным
            double consonants_percent;
            if (total_letters > 0) consonants_percent = (consonants_count * 100.0) / total_letters;
            else consonants_percent = 0;
            return to_string(consonants_count) +
                "(" + to_string(consonants_percent).substr(0,5) + "%)";
            break ;
        }
        case 20 : {// звонкие согласные
            double voiced_percent;
            if (consonants_count > 0)
                voiced_percent = (voiced_count * 100.0) / consonants_count ;
            else voiced_percent = 0 ;
            return to_string(voiced_count) +
                "(" + to_string(voiced_percent).substr(0,5) + "%)";
            break;
        }
        case 30 : {// глухие согласные
            double voiceless_percent;
            if (consonants_count > 0)
                voiceless_percent = (voiceless_count * 100.0) / consonants_count ;
            else voiceless_percent = 0 ;
            return to_string(voiceless_count) +
                "(" + to_string(voiceless_percent).substr(0,5) + "%)";
            break;
        }
        case 40 : {// редкие согласные
            double rare_percent;
            if (consonants_count > 0)
                rare_percent = (rare_count * 100.0) / consonants_count ;
            else rare_percent = 0 ;
            return to_string(rare_count) +
                "(" + to_string(rare_percent).substr(0,5) + "%)";
            break;
        }
        case 50 : {// ь, ъ, ы, й
            double sd_percent;
            if (consonants_count > 0)
                sd_percent = (sd_count * 100.0) / total_letters ;
            else sd_percent = 0 ;
            return to_string(sd_count) +
                "(" + to_string(sd_percent).substr(0,5) + "%)";
            break;
        }
        case 60 : {// знаки препинания
            double pm_percent;
            if (pm_count > 0)
                pm_percent = (pm_count * 100.0) / total_letters ;
            else pm_percent = 0 ;
            return to_string(pm_count) +
                "(" + to_string(pm_percent).substr(0,5) + "%)";
            break;
        }
        case 70 : {// встречаемость букв
            if (s_sym == ' ') return to_string(l_counts[33]*1.0/total_letters).substr(0,5) +
                "(" + to_string(avg_freq[33]).substr(0,5) + ")";
            const int p = alphabet.find(s_sym) ;
            return to_string(l_counts[p]*1.0/total_letters).substr(0,5) +
            "(" + to_string(avg_freq[p]).substr(0,5) + ")";
            break ;
        }
        case 80 : {// всего слов
            if (word_count != 0) return to_string(word_count);
            break ;
        }
        case 85 : {// средняя длина слова
            if (word_count != 0) return to_string(total_word_length*1.0/word_count).substr(0,5);
            break;
        }
        case 90 : {// количество предложений
            if (sentence_count != 0) return to_string(sentence_count) ;
            break;
        }
        case 95 : {// среднее кол-во слов в предложении
            if (sentence_count != 0) return to_string(word_count*1.0/sentence_count).substr(0,5);
            break;
        }
        case 100 : // всего букв
            return to_string(total_letters) ;
            break ;
        default: return "";
    }
}

// 2.	Привести статистику по длине слов и предложений.
// Этот кусок делается в firstCase
void secondCase () {

}
// 3.	Привести статистику по предлогам и союзам (наиболее часто встречающимся)
// void thirdCase () {

// }

// Функция для разделения текста на слова
vector<string> split_to_words(const string& text) {
    vector<string> words;
    string word, _word;

    const string _text = to_lower(text);
    for (int i = 0; i < _text.length(); i++) {
        if (char c = _text[i]; (c >= 'а' && c <= 'я') || c == 'ё')
            // || (c >= 'А' && c <= 'Я') || c == 'Ё')
            {
            _word.push_back(c);
            word += _word;
            _word.clear();
        } else {
            if (!word.empty()) {
                words.push_back(word);
                word.clear(); _word.clear();
            }
        }
    }

    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

// Функция проверки, является ли слово предлогом
bool is_preposition(const string& word) {
    for (const auto & preposition : prepositions) {
        if (word == preposition) {
            return true;
        }
    }
    return false;
}

// Функция проверки, является ли слово союзом
bool is_conjunction(const string& word) {
    for (const auto & conjunction : conjunctions) {
        if (word == conjunction) {
            return true;
        }
    }
    return false;
}

// Упрощенная функция для подсчета статистики
void count_prep_conj_stats() {
    for (int file_num = 0; file_num < 4; file_num++) {
        vector<string> words = split_to_words(texts[file_num]);

        // Счетчики для каждого предлога и союза
        int prep_counts[50] = {0}; // предполагаем не более 50 разных
        int conj_counts[50] = {0};

        // Подсчитываем
        for (const auto& word : words) {
            // fout << word << " " << endl;

            // Проверяем предлоги
            for (int j = 0; j < prepositions.size(); j++) {
                if (word == prepositions[j]) {
                    prep_counts[j]++;
                    total_preps[file_num]++;
                    break;
                }
            }

            // Проверяем союзы
            for (int j = 0; j < conjunctions.size(); j++) {
                if (word == conjunctions[j]) {
                    conj_counts[j]++;
                    total_conjs[file_num]++;
                    break;
                }
            }
        }

        // Находим самый частый предлог
        int max_prep_count = 0;
        int max_prep_index = -1;
        for (int j = 0; j < prepositions.size(); j++) {
            if (prep_counts[j] > max_prep_count) {
                max_prep_count = prep_counts[j];
                max_prep_index = j;
            }
        }
        if (max_prep_index != -1 && max_prep_count > 0) {
            top_preps[file_num] = prepositions[max_prep_index] +
                                  "(" + to_string(max_prep_count) + ")";
        }

        // Находим самый частый союз
        int max_conj_count = 0;
        int max_conj_index = -1;
        for (int j = 0; j < conjunctions.size(); j++) {
            if (conj_counts[j] > max_conj_count) {
                max_conj_count = conj_counts[j];
                max_conj_index = j;
            }
        }
        if (max_conj_index != -1 && max_conj_count > 0) {
            top_conjs[file_num] = conjunctions[max_conj_index] +
                                  "(" + to_string(max_conj_count) + ")";
        }
    }
}

// предлоги и союзы
void thirdCase() {
    count_prep_conj_stats(); // сначала подсчитываем статистику

    // Добавляем новые строки в основную таблицу
    fout << rpad("Всего предлогов", 40);
    for (int i = 0; i < 4; i++) {
        fout << lpad(to_string(total_preps[i]), 20);
    }
    fout << endl;

    fout << rpad("Всего союзов", 40);
    for (int i = 0; i < 4; i++) {
        fout << lpad(to_string(total_conjs[i]), 20);
    }
    fout << endl;

    fout << rpad("Самый частый предлог", 40);
    for (int i = 0; i < 4; i++) {
        fout << lpad(top_preps[i], 20);
    }
    fout << endl;

    fout << rpad("Самый частый союз", 40);
    for (int i = 0; i < 4; i++) {
        fout << lpad(top_conjs[i], 20);
    }
    fout << endl;

    fout.close();
}

// 4.	Привести статистику по буквосочетаниям, наиболее часто встречающимся в русском языке:
//      СТ, НО, ЕН, ТО, НА, ОВ, НИ, РА, ВО, КО; СТО, ЕНО, НОВ, ТОВ, ОВО, ОВА
void fourthCase() {
    // Буквосочетания для анализа
    string bigrams_list[10] = {"ст", "но", "ен", "то", "на", "ов", "ни", "ра", "во", "ко"};
    string trigrams_list[6] = {"сто", "ено", "нов", "тов", "ово", "ова"};

    // Вывод заголовка для биграмм
    fout << endl << rpad("Буквосочетания (биграммы)", 40) ;
    // for (int i = 0; i < 4; i++) {
        // fout << lpad("Файл " + to_string(i+1), 20);
    // }
    fout << endl;

    // Обработка каждой биграммы
    for (const auto& bigram : bigrams_list) {
        fout << rpad(bigram, 40);

        // Для каждого файла считаем эту биграмму
        for (int file_num = 0; file_num < 4; file_num++) {
            string text = to_lower(texts[file_num]);
            int count = 0;

            // Считаем количество вхождений биграммы
            for (int i = 0; i <= text.length() - 2; i++) {
                string substr = text.substr(i, 2);

                // Проверяем, состоит ли из букв
                bool is_letters = true;
                for (char c : substr) {
                    if ((c < 'а' || c > 'я') && c != 'ё') {
                        is_letters = false;
                        break;
                    }
                }

                if (is_letters && substr == bigram) {
                    count++;
                }
            }

            fout << lpad(to_string(count), 20);
        }
        fout << endl;
    }

    // Вывод заголовка для триграмм
    fout << endl << rpad("Буквосочетания (триграммы)", 40);
    // for (int i = 0; i < 4; i++) {
        // fout << lpad("Файл " + to_string(i+1), 20);
    // }
    fout << endl;

    // Обработка каждой триграммы
    for (const auto& trigram : trigrams_list) {
        fout << rpad(trigram, 40);

        // Для каждого файла считаем эту триграмму
        for (int file_num = 0; file_num < 4; file_num++) {
            string text = to_lower(texts[file_num]);
            int count = 0;

            // Считаем количество вхождений триграммы
            for (int i = 0; i <= text.length() - 3; i++) {
                string substr = text.substr(i, 3);

                // Проверяем, состоит ли из букв
                bool is_letters = true;
                for (char c : substr) {
                    if ((c < 'а' || c > 'я') && c != 'ё') {
                        is_letters = false;
                        break;
                    }
                }

                if (is_letters && substr == trigram) {
                    count++;
                }
            }

            fout << lpad(to_string(count), 20);
        }
        fout << endl;
    }

    // Дополнительная статистика
    fout << endl << rpad("Самая частая биграмма", 40);
    for (int file_num = 0; file_num < 4; file_num++) {
        string text = to_lower(texts[file_num]);
        int max_count = 0;
        string max_bigram = "-";

        // Ищем самую частую биграмму среди всех 10
        for (const auto& bigram : bigrams_list) {
            int count = 0;

            for (int i = 0; i <= text.length() - 2; i++) {
                string substr = text.substr(i, 2);
                bool is_letters = true;
                for (char c : substr) {
                    if ((c < 'а' || c > 'я') && c != 'ё') {
                        is_letters = false;
                        break;
                    }
                }
                if (is_letters && substr == bigram) {
                    count++;
                }
            }

            if (count > max_count) {
                max_count = count;
                max_bigram = bigram;
            }
        }

        if (max_count > 0) {
            fout << lpad(max_bigram + "(" + to_string(max_count) + ")", 20);
        } else {
            fout << lpad("-", 20);
        }
    }
    fout << endl;

    fout << rpad("Самая частая триграмма", 40);
    for (int file_num = 0; file_num < 4; file_num++) {
        string text = to_lower(texts[file_num]);
        int max_count = 0;
        string max_trigram = "-";

        // Ищем самую частую триграмму среди всех 6
        for (const auto& trigram : trigrams_list) {
            int count = 0;

            for (int i = 0; i <= text.length() - 3; i++) {
                string substr = text.substr(i, 3);
                bool is_letters = true;
                for (char c : substr) {
                    if ((c < 'а' || c > 'я') && c != 'ё') {
                        is_letters = false;
                        break;
                    }
                }
                if (is_letters && substr == trigram) {
                    count++;
                }
            }

            if (count > max_count) {
                max_count = count;
                max_trigram = trigram;
            }
        }

        if (max_count > 0) {
            fout << lpad(max_trigram + "(" + to_string(max_count) + ")", 20);
        } else {
            fout << lpad("-", 20);
        }
    }
    fout << endl;

    fout.close();
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
    // SetConsoleCP(CP_UTF8);
    // SetConsoleOutputCP(CP_UTF8);

    ofstream fout(dir_name + "output_l.txt");

    // setlocale(LC_ALL, "ru");
    // SetConsoleCP(1251);
    // SetConsoleOutputCP(1251);

    // void print_string(string func_name, string header){
    //     cout << rpad(header, 40);
    //     for (int i = 1; i <= sizeof(texts); i++) {
    //         cout << lpad(func_name(texts[i]), 20) ;
    //     }
    //     fout << endl ;
    // }

    for (int i = 1; i <= 4; i++) {
        // cout << readText("../resources/input" + to_string(i) + "s.txt") << endl; // debug
        // cout << readTextCP1251("../resources/input" + to_string(i) + "s.txt") << endl; // debug
        // заполняем массив с текстами
        texts[i-1] = readTextCP1251(dir_name + "input" + to_string(i) + "l.txt") ;
    }

    fout << endl ;
    // выводим шапку
    fout << rpad("Файл",40) << lpad("1",20) << lpad("2",20)
        << lpad("3", 20) << lpad("4",20) << endl ;
    fout << "------------------------------------------------------------------------------------------------------------------------" << endl ;

    // выводим информацию по критериям, которые можно представить в виде таблицы
    // топорный способ с использованием switch, тапками не бросаться
    // fout << headers.size() << endl ; fout << sizeof(texts) << endl ;
    for (int i = 0; i < headers.size(); i++) {
        fout << rpad(headers[i], 40);
        for (auto & text : texts) {
            string result ;
            switch (i) {
                case 0: result = firstCase(text, 100) ; break ;
                case 1: result = firstCase(text, 0) ; break ;
                case 2: result = firstCase(text, 10) ; break ;
                case 3: result = firstCase(text, 20) ; break ;
                case 4: result = firstCase(text, 30) ; break ;
                case 5: result = firstCase(text, 40) ; break ;
                case 6: result = firstCase(text, 50) ; break ;
                case 7: result = firstCase(text, 60) ; break ;
                case 8: result = firstCase(text, 70, 'а') ; break ;
                case 9: result = firstCase(text, 70, 'б') ; break ;
                case 10: result = firstCase(text, 70, 'в') ; break ;
                case 11: result = firstCase(text, 70, 'г') ; break ;
                case 12: result = firstCase(text, 70, 'д') ; break ;
                case 13: result = firstCase(text, 70, 'е') ; break ;
                case 14: result = firstCase(text, 70, 'ё') ; break ;
                case 15: result = firstCase(text, 70, 'ж') ; break ;
                case 16: result = firstCase(text, 70, 'з') ; break ;
                case 17: result = firstCase(text, 70, 'и') ; break ;
                case 18: result = firstCase(text, 70, 'й') ; break ;
                case 19: result = firstCase(text, 70, 'к') ; break ;
                case 20: result = firstCase(text, 70, 'л') ; break ;
                case 21: result = firstCase(text, 70, 'м') ; break ;
                case 22: result = firstCase(text, 70, 'н') ; break ;
                case 23: result = firstCase(text, 70, 'о') ; break ;
                case 24: result = firstCase(text, 70, 'п') ; break ;
                case 25: result = firstCase(text, 70, 'р') ; break ;
                case 26: result = firstCase(text, 70, 'с') ; break ;
                case 27: result = firstCase(text, 70, 'т') ; break ;
                case 28: result = firstCase(text, 70, 'у') ; break ;
                case 29: result = firstCase(text, 70, 'ф') ; break ;
                case 30: result = firstCase(text, 70, 'х') ; break ;
                case 31: result = firstCase(text, 70, 'ц') ; break ;
                case 32: result = firstCase(text, 70, 'ч') ; break ;
                case 33: result = firstCase(text, 70, 'ш') ; break ;
                case 34: result = firstCase(text, 70, 'щ') ; break ;
                case 35: result = firstCase(text, 70, 'ъ') ; break ;
                case 36: result = firstCase(text, 70, 'ы') ; break ;
                case 37: result = firstCase(text, 70, 'ь') ; break ;
                case 38: result = firstCase(text, 70, 'э') ; break ;
                case 39: result = firstCase(text, 70, 'ю') ; break ;
                case 40: result = firstCase(text, 70, 'я') ; break ;
                case 41: result = firstCase(text, 70) ; break ;
                case 42: result = firstCase(text, 80) ; break ;
                case 43: result = firstCase(text, 85) ; break ;
                case 44: result = firstCase(text, 90) ; break ;
                case 45: result = firstCase(text, 95) ; break ;
                default: break;
            }
            fout << lpad(result, 20) ;
        }
        fout << endl ;
    }

    // for (int i = 0; i < 4; i++) {
        // secondCase();
        thirdCase();
        fourthCase();
        fifthCase();
        sixthCase();
        seventhCase();
        eighthCase();
        ninthCase();
    // }
    // for (const wstring& word : getWords(readText("../resources/input1s.txt"))) {
    // for (const string& word : getWordsCP1251(readTextCP1251("../resources/input1s.txt"))) {
    //     // wcout << word << " " ;
    //     cout << word << " " ;
    // }
    fout.close();
    return 0;
}
