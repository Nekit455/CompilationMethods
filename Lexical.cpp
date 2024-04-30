#include <iostream>
#include <string>
#include <cctype>

using namespace std;
int numStr = 0;
size_t pos = 0;

// Типы лексем
enum class TokenType {
    COUNT,
    REAL,
    ASSIGN,
    OPERATOR,
    CMP,
    ID,
    WORD,
    GRP,
    POINT
};

// Структура для хранения лексемы (за исключением чисел)
struct Token {
    TokenType type;
    string lexeme;
};

// Хранение целого числа
struct Number {
    int Num;
};

// Хранение вещ. числа
struct RealNumber {
    double realNum;
};

// Функция для определения типа лексемы
TokenType getTokenType(const string& lexeme) {
    if (lexeme == "=") return TokenType::ASSIGN;
    else if (lexeme == "*" || lexeme == "/" || lexeme == "%" || lexeme == "-" || lexeme == "+") return TokenType::OPERATOR;
    else if (lexeme == "<" || lexeme == ">" || lexeme == "<=" || lexeme == "==" || lexeme == ">=" || lexeme == "#") return TokenType::CMP;
    else if (lexeme == "(" || lexeme == ")" || lexeme == "[" || lexeme == "]" || lexeme == "{" || lexeme == "}") return TokenType::GRP;
    else if (lexeme == ".") return TokenType::POINT;
    else if (lexeme == "input" || lexeme == "print" || lexeme == "if" || lexeme == "else" || lexeme == "while" || lexeme == "do" || lexeme == "string" || lexeme == "int" || lexeme == "double") return TokenType::WORD;
    else if (isdigit(lexeme[0])) {
        bool hasDot = false;
        for (char c : lexeme) {
            if (c == '.') hasDot = true;                                       
        }
        return hasDot ? TokenType::REAL : TokenType::COUNT;
    }
    else if (isalpha(lexeme[0]) || lexeme[0] == '_') {
        return TokenType::ID;
    }
    else { 
        cout << "Ошибка. Строка: " << numStr << ", позиция: " << pos;
        throw invalid_argument("Не символ языка"); 
    }
}

// Сохраняем целое число
Number getNumber(int Num) {
    return { Num };
}

// Сохраняем вещ. число
RealNumber getRealNumber(double realNum) {
    return { realNum };
}

// Функция лексического анализа
Token getNextToken(const string& inputString) {
    string lexeme; 
    
    // Пропускаем пробелы и символы перевода строки
    while (pos < inputString.length() && (isspace(inputString[pos]) || inputString[pos] == '\n')) {
        if (inputString[pos] == '\n') {
            numStr++;
        }
        pos++;
    }

    // Собираем лексему
    lexeme += inputString[pos];

    // Если текущий символ - оператор или символ группировки, возвращаем его как отдельную лексему
    if (inputString[pos] == inputString[pos] == '*' || inputString[pos] == '/' || inputString[pos] == '-' || inputString[pos] == '+' || inputString[pos] == '%' || inputString[pos] == '#' 
        || inputString[pos] == '[' || inputString[pos] == ']' || inputString[pos] == '{' || inputString[pos] == '}' || inputString[pos] == '(' || inputString[pos] == ')') {
        pos++;
        return { getTokenType(lexeme), lexeme };
    }

    // Собираем операторы сравнения
    if (inputString[pos] == '<' || inputString[pos] == '>' || inputString[pos] == '=') {
        pos++;
        if (inputString[pos] == '=') { lexeme += inputString[pos]; } 
        else return { getTokenType(lexeme), lexeme };
    }

    // Если текущий символ буква или '_', собираем идентификатор
    if (isalpha(inputString[pos]) || inputString[pos] == '_') {
        pos++;
        while (pos < inputString.length() && (isalnum(inputString[pos]) || inputString[pos] == '_')) {
            lexeme += inputString[pos];
            pos++;
        }
        return { getTokenType(lexeme), lexeme };
    }

    // Собираем число (целое/вещественное)
    if (isdigit(inputString[pos])) {
        pos++;
        bool hasDot = false;
        while (pos < inputString.length() && ((isdigit(inputString[pos])) || inputString[pos] == '.')) {
            if (inputString[pos] == '.' && hasDot == true) { 
                cout << "Ошибка. Строка: " << numStr << ", позиция: " << pos;
                throw invalid_argument("Некорректный ввод вещ. числа"); 
            }
            if (inputString[pos] == '.') hasDot = true;
            lexeme += inputString[pos];
            pos++;
            int pos1 = pos;
            pos1--;
            if (inputString[pos1] == '.' && !isdigit(inputString[pos])) { 
                cout << "Ошибка. Строка: " << numStr << ", позиция: " << pos;
                throw invalid_argument("Некорректный ввод вещ. числа"); 
            }
        }
        if (isalpha(inputString[pos]))
        {
            cout << "Ошибка. Строка: " << numStr << ", ";
            throw invalid_argument("Некорректный ввод числа");
        }
        else if (hasDot == false) {         // Преобразуем целое число
            int Num = 0;

            for (char p : lexeme) {
                Num = Num * 10 + (p - '0');
            }
            getNumber(Num);
        }
        else {                              // Преобразуем вещ. число
            double realNum = 0;
            double fraction = 0.1; 

            bool decimalPart = false; 

            for (char k : lexeme) {
                if (k == '.') {
                    decimalPart = true; 
                }
                else {
                    if (decimalPart) {
                        realNum += (k - '0') * fraction; // Дробная часть
                        fraction /= 10.0;
                    }
                    else {
                        realNum = realNum * 10 + (k - '0'); // Целая часть
                    }
                }
            }
            getRealNumber(realNum);
        }
        
    }

    if (inputString[pos] == '.') { 
        cout << "Ошибка. Строка: " << numStr << ", позиция: " << pos;
        throw invalid_argument("Точка не может быть самостоятельной"); 
    }
    pos++;
    
    return { getTokenType(lexeme), lexeme };
}

int main() {
    setlocale(LC_ALL, "rus");
    string inputString = "w \n53 5.3\n  dmjwdj";
    
    Token token;

    cout << "Лексический анализ:\n";
    do {
        token = getNextToken(inputString);
        cout << "Лексема: " << token.lexeme << ", Тип: ";
        switch (token.type) {
        case TokenType::COUNT:
            cout << "Целое число\n";
            break;
        case TokenType::REAL:
            cout << "Вещественное число\n";
            break;
        case TokenType::ASSIGN:
            cout << "Оператор присваивания\n";
            break;
        case TokenType::OPERATOR:
            cout << "Оператор\n";
            break;
        case TokenType::CMP:
            cout << "Оператор сравнения\n";
            break;
        case TokenType::ID:
            cout << "Идентификатор\n";
            break;
        case TokenType::WORD:
            cout << "Служебное слово\n";
            break;
        case TokenType::GRP:
            cout << "Символ группировки\n";
            break;
        case TokenType::POINT:
            cout << "Точка\n";
            break;
        }
    } while (pos < inputString.length());

    return 0;
}
