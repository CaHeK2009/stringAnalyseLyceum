#include <iostream>
#include <string>
using namespace std;

// Функция для перевода всего, что на входе, в нижний регистр. для упрощения обработки
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

// Функция анализирует текст и возвращает его "цифровую подпись" (signature) в виде строки
string analyze_text(string text) {
    string lower_text = to_lower(text);
    string signature = ""; // цифровая подпись стиля

    // 1.1. Процент гласных букв.
    string vowels = "аеёиоуыэюя";
    int total_letters = 0;
    int vowel_count = 0;

    for (int i = 0; i < lower_text.length(); i++) {
        char c = lower_text[i];
        if ((c >= 'а' && c <= 'я') || c == 'ё') { // 'ё' стоит отдельно от остальных букв в кодовой таблице символов
            total_letters++;
            if (vowels.find(c) != string::npos) {
                vowel_count++;
            }
        }
    }

    // Ниже сокращенная запись условия. можно использовать как альтернативу if-then-else для уменьшения объема кода.
    // double vowel_percent = total_letters > 0 ? (vowel_count * 100.0) / total_letters : 0;
    double vowel_percent;

    if (total_letters > 0) {
        vowel_percent = (vowel_count * 100.0) / total_letters;
    } else {
        vowel_percent = 0;
    }

    signature += "v:" + to_string((int)vowel_percent) + ";";

    // 1.2. Процент согласных букв.
    string consonants = "бвгджзйклмнпрстфхцчшщ";
    total_letters = 0;
    vowel_count = 0;
    int consonants_count = 0;

    for (int i = 0; i < lower_text.length(); i++) {
        char c = lower_text[i];
        if ((c >= 'а' && c <= 'я') || c == 'ё') { // 'ё' стоит отдельно от остальных букв в кодовой таблице символов
            total_letters++;
            if (consonants.find(c) != string::npos) {
                consonants_count++;
            }
        }
    }

    // Ниже сокращенная запись условия. можно использовать как альтернативу if-then-else для уменьшения объема кода.
    // double vowel_percent = total_letters > 0 ? (vowel_count * 100.0) / total_letters : 0;
    double consonants_percent;

    if (total_letters > 0) {
        consonants_percent = (consonants_count * 100.0) / total_letters;
    } else {
        consonants_percent = 0;
    }

    signature += "c:" + to_string((int)consonants_percent) + ";";

    // 2. Средняя длина слова
    int word_count = 0;
    int total_word_length = 0;
    int current_length = 0;

    for (int i = 0; i < lower_text.length(); i++) {
        char c = lower_text[i];
        if ((c >= 'а' && c <= 'я') || c == 'ё') {
            current_length++;
        } else {
            if (current_length > 0) {
                word_count++;
                total_word_length += current_length;
                current_length = 0;
            }
        }
    }
    if (current_length > 0) {
        word_count++;
        total_word_length += current_length;
    }

    // double avg_word_length = word_count > 0 ? (double)total_word_length / word_count : 0;
    double avg_word_length;
    if (word_count > 0) {
        avg_word_length = (double)total_word_length / word_count;
    } else {
        avg_word_length = 0;
    }
    // умножаем на 10 для сохранения точности и для преобразования дробного числа в целое
    signature += "w:" + to_string((int)(avg_word_length * 10)) + ";";

    // 3. Частота буквосочетания "ст". аналогично можно сделать для других буквосочетаний, расширяя условие.
    // для трехбуквенных испльзовать другую функцию, где в substr используется 3 символа
    int st_count = 0;
    for (int i = 0; i < lower_text.length() - 1; i++) {
        if (lower_text.substr(i, 2) == "ст") {
            st_count++;
        }
    }
    signature += "st:" + to_string(st_count) + ";";

    // 4. Частота редких согласных
    string rare_consonants = "фхцчшщж";
    int rare_count = 0;
    for (int i = 0; i < lower_text.length(); i++) {
        if (rare_consonants.find(lower_text[i]) != string::npos) {
            rare_count++;
        }
    }
    signature += "r:" + to_string(rare_count) + ";";

    // 5. Частота знаков препинания
    int punctuation_count = 0;
    for (int i = 0; i < text.length(); i++) {
        char c = text[i];
        if (c == '.' || c == ',' || c == '!' || c == '?' || c == ':' || c == ';') {
            punctuation_count++;
        }
    }
    signature += "p:" + to_string(punctuation_count) + ";";

    // 6. Соотношение слов к предложениям
    int sentence_count = 0;
    for (int i = 0; i < text.length(); i++) {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?') {
            sentence_count++;
        }
    }
    double words_per_sentence;

    if (sentence_count > 0) {
        words_per_sentence = (double)word_count / sentence_count;
    } else {
        words_per_sentence = 0;
    }

    signature += "wps:" + to_string((int)(words_per_sentence * 100)) + ";";

    return signature;
}

// Функция вычисляет "расстояние" между двумя "подписями"
int calculate_distance(const string& sig1, const string& sig2) {
    int distance = 0;

    // Извлекаем числовые значения из подписей и сравниваем
    size_t pos1 = 0, pos2 = 0;
    while (pos1 < sig1.length() && pos2 < sig2.length()) {
        // Ищем значения после двоеточий
        size_t colon1 = sig1.find(':', pos1);
        size_t colon2 = sig2.find(':', pos2);

        if (colon1 == string::npos || colon2 == string::npos) break;

        // Извлекаем числа
        size_t semicolon1 = sig1.find(';', colon1);
        size_t semicolon2 = sig2.find(';', colon2);

        if (semicolon1 == string::npos || semicolon2 == string::npos) break;

        int val1 = stoi(sig1.substr(colon1 + 1, semicolon1 - colon1 - 1));
        int val2 = stoi(sig2.substr(colon2 + 1, semicolon2 - colon2 - 1));

        distance += abs(val1 - val2);

        pos1 = semicolon1 + 1;
        pos2 = semicolon2 + 1;
    }

    return distance;
}

// Главная функция для нахождения "лишнего" текста
int find_simple_outlier(string text1, string text2, string text3, string text4){

    string sig1 = analyze_text(text1);
    string sig2 = analyze_text(text2);
    string sig3 = analyze_text(text3);
    string sig4 = analyze_text(text4);

    // Сравниваем попарно и находим самый отличающийся
    int d12 = calculate_distance(sig1, sig2);
    int d13 = calculate_distance(sig1, sig3);
    int d14 = calculate_distance(sig1, sig4);
    int d23 = calculate_distance(sig2, sig3);
    int d24 = calculate_distance(sig2, sig4);
    int d34 = calculate_distance(sig3, sig4);

    // Вычисляем суммарные расстояния для каждого текста
    int total1 = d12 + d13 + d14;
    int total2 = d12 + d23 + d24;
    int total3 = d13 + d23 + d34;
    int total4 = d14 + d24 + d34;

    // Находим максимальное
    if (total1 > total2 && total1 > total3 && total1 > total4) return 1;
    if (total2 > total1 && total2 > total3 && total2 > total4) return 2;
    if (total3 > total1 && total3 > total2 && total3 > total4) return 3;
    if (total4 > total1 && total4 > total2 && total4 > total3) return 4;
}

int main() {
    // Загрузка текстов. нужно любым методом доступным методом сформировать 4 переменные : text1 ... text4
    setlocale(0, "");
    string text1 = "первый текст...";
    string text2 = "второй текст ппп пппппп пппп...";
    string text3 = "третий текст...";
    string text4 = "четвертый текса рра...";

    int outlier = find_simple_outlier(text1, text2, text3, text4);
    cout << "Текст другого писателя: " << outlier << endl;

    return 0;
}
