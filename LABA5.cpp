#define _CRT_SECURE_NO_WARNINGS  // Отключаем предупреждения безопасности CRT
#include <iostream>
#include <cstring>
#include <Windows.h>  // Для работы с консолью Windows
#include <cstdlib>
#include <string>
#include <vector>

// Константы для ограничений размеров массивов
const int MAX_BORROWED_BOOKS = 100;
const int MAX_BOOKS = 100;
const int MAX_READERS = 100;
const int MAX_LOANS = 100;
const int MAX_NAME_LENGTH = 30;
const int MAX_AUTHORS = 100;

// Класс Author - представляет автора книг
class Author {
    std::string fio;       // ФИО автора
    int birth_year;         // Год рождения
    static int total_authors; // Статическое поле - общее количество авторов

public:
    Author() : birth_year(0) {
        total_authors++;    // Увеличиваем счетчик при создании нового автора
    }

    // Статический метод для получения общего количества авторов
    static int get_total_authors() {
        return total_authors;
    }

    // Геттер для ФИО автора
    const std::string& get_fio() const {
        return fio;
    }

    // Метод для добавления информации об авторе
    void add_Author() {
        // Ввод ФИО
        printf("Введите ФИО автора: ");
        while (getchar() != '\n'); // Очистка буфера ввода
        std::getline(std::cin, this->fio);

        // Ввод года рождения
        printf("Введите год рождения автора: ");
        std::string year_str;
        std::getline(std::cin, year_str);
        this->birth_year = std::stoi(year_str); // Преобразование строки в число
    }

    // Метод для вывода информации об авторе
    void print_Author() {
        printf("ФИО автора: %s\n", this->fio.c_str());
        printf("Год рождения автора: %d\n", this->birth_year);
    }

    // Перегрузка оператора + для объединения авторов
    Author operator+(const Author& other) const {
        Author result;
        result.fio = this->fio + " & " + other.fio;
        result.birth_year = (this->birth_year + other.birth_year) / 2;
        return result;
    }

    // Префиксный инкремент (увеличение года рождения)
    Author& operator++() {
        ++this->birth_year;
        return *this;
    }

    // Постфиксный инкремент
    Author operator++(int) {
        Author temp = *this;
        ++(*this);
        return temp;
    }
};

// Инициализация статического поля
int Author::total_authors = 0;

// Класс Book - представляет книгу в библиотеке
class Book {
    std::string title;  // Название книги
    Author* author;     // Указатель на автора
    int pub_year;       // Год публикации
    int copies;         // Количество экземпляров

public:
    Book() : author(nullptr), pub_year(0), copies(0) {}

    // Геттер для названия книги
    const std::string& get_title() const {
        return title;
    }

    // Метод для добавления книги
    void add_Book(Author** authors, int num_authors) {
        printf("Введите название книги: \n");
        while (getchar() != '\n'); // Очистка буфера
        std::getline(std::cin, this->title);

        // Проверка на пустое название
        if (this->title.empty()) {
            throw std::invalid_argument("Название книги не может быть пустым.");
        }

        // Выбор автора из списка
        printf("Выберите автора (введите номер): ");
        for (int i = 0; i < num_authors; i++) {
            if (authors[i] != nullptr) {
                printf("%d. %s\n", i + 1, authors[i]->get_fio().c_str());
            }
        }

        int author_index;
        scanf("%d", &author_index);
        // Проверка корректности выбора автора
        if (author_index < 1 || author_index > num_authors || authors[author_index - 1] == nullptr) {
            throw std::out_of_range("Некорректный номер автора.");
        }
        this->author = authors[author_index - 1];

        // Ввод остальных данных о книге
        printf("Введите дату публикации: ");
        scanf("%d", &this->pub_year);
        printf("Введите количество экземпляров: ");
        scanf("%d", &this->copies);
    }

    // Метод для вывода информации о книге
    void print_Book() {
        if (this->title.empty() || this->author == nullptr) {
            printf("Ошибка: некорректные данные о книге.\n");
            return;
        }
        printf("Название книги: %s\n", this->title.c_str());
        printf("Автор книги: %s\n", this->author->get_fio().c_str());
        printf("Дата публикации: %d\n", this->pub_year);
        printf("Количество экземпляров: %d\n", this->copies);
    }

    // Дружественная функция для проверки доступности книги
    friend bool is_book_available(const Book& book, int required_copies);
};

// Проверка доступности книги (достаточно ли экземпляров)
bool is_book_available(const Book& book, int required_copies) {
    return book.copies >= required_copies;
}

// Класс Reader - представляет читателя библиотеки
class Reader {
    std::string fio;           // ФИО читателя
    int card_number;           // Номер читательского билета
    Book* borrowed_books[MAX_BORROWED_BOOKS];  // Массив взятых книг
    int borrowed_count;        // Количество взятых книг

public:
    Reader() : card_number(0), borrowed_count(0) {}

    // Увеличение счетчика взятых книг
    void set_borrowed_count() {
        borrowed_count++;
    }

    // Геттер для ФИО
    std::string& get_fio() {
        return fio;
    }

    // Геттер для номера билета
    int get_card_number() {
        return card_number;
    }

    // Получение количества взятых книг через указатель
    int* get_borrowed_count_ptr() {
        return &borrowed_count;
    }

    // Получение количества взятых книг через ссылку
    int& get_borrowed_count_ref() {
        return borrowed_count;
    }

    // Метод для добавления читателя
    void add_Reader() {
        printf("Введите ФИО читателя: \n");
        while (getchar() != '\n'); // Очистка буфера
        std::getline(std::cin, this->fio);

        printf("Введите номер читательского билета: ");
        scanf("%d", &this->card_number);
        this->borrowed_count = 0;
    }

    // Метод для вывода информации о читателе
    void print_Reader() {
        if (this->fio.empty()) {
            printf("Ошибка: некорректные данные о читателе.\n");
            return;
        }
        printf("ФИО читателя: %s\n", this->fio.c_str());
        printf("Номер читательского билета: %d\n", this->card_number);
        printf("Количество взятых книг: %d\n", this->borrowed_count);
    }
};

// Класс Loan - представляет выдачу книги читателю
class Loan {
    Book* book;             // Указатель на книгу
    Reader* reader;         // Указатель на читателя
    std::string issue_date; // Дата выдачи
    std::string return_date;// Дата возврата

public:
    Loan() : book(nullptr), reader(nullptr) {}

    // Конструктор с параметрами
    Loan(Book* book, Reader* reader, const std::string& issue_date, const std::string& return_date)
        : book(book), reader(reader), issue_date(issue_date), return_date(return_date) {
    }

    // Геттер для даты выдачи
    std::string get_issue_date() {
        return issue_date;
    }

    // Геттер для даты возврата
    std::string get_return_date() {
        return return_date;
    }

    // Метод для вывода информации о выдаче
    void print_Loan() {
        if (book == nullptr || reader == nullptr || book->get_title().empty() || reader->get_fio().empty()) {
            printf("Ошибка: некорректные данные о выдаче.\n");
            return;
        }
        printf("Название выданной книги: %s\n", book->get_title().c_str());
        printf("Читатель: %s\n", reader->get_fio().c_str());
        printf("Дата выдачи книги: %s\n", this->issue_date.c_str());
        printf("Дата возврата книги: %s\n", this->return_date.c_str());
    }
};

// Класс Library - представляет библиотеку и управляет всеми данными
class Library {
    Author* authors[MAX_AUTHORS];  // Массив авторов
    Book* books[MAX_BOOKS];        // Массив книг
    int book_count;                // Текущее количество книг
    Reader* readers[MAX_READERS];  // Массив читателей
    int reader_count;              // Текущее количество читателей
    Loan* loans[MAX_LOANS];        // Массив выдач
    int loan_count;                // Текущее количество выдач

public:
    // Конструктор - инициализация данных
    Library() : book_count(0), reader_count(0), loan_count(0) {
        // Инициализация массивов нулевыми указателями
        for (int i = 0; i < MAX_AUTHORS; i++) {
            authors[i] = nullptr;
        }
        for (int i = 0; i < MAX_BOOKS; i++) {
            books[i] = nullptr;
        }
        for (int i = 0; i < MAX_READERS; i++) {
            readers[i] = nullptr;
        }
        for (int i = 0; i < MAX_LOANS; i++) {
            loans[i] = nullptr;
        }
    }

    // Геттеры для количества книг и читателей
    int get_book_count() const {
        return book_count;
    }

    int get_reader_count() const {
        return reader_count;
    }

    // Получение книги по индексу
    Book* get_books(int i) {
        return books[i];
    }

    // Получение читателя по индексу
    Reader* get_reader(int i) {
        return readers[i];
    }

    // Метод для вывода всей информации о библиотеке
    void print_Library() {
        printf("Общее количество авторов: %d\n", Author::get_total_authors());

        printf("Список авторов:\n");
        for (int i = 0; i < MAX_AUTHORS; i++) {
            if (authors[i] != nullptr) {
                authors[i]->print_Author();
            }
        }

        printf("Список книг:\n");
        for (int i = 0; i < book_count; i++) {
            if (books[i] != nullptr) {
                books[i]->print_Book();
            }
        }

        printf("Список читателей:\n");
        for (int i = 0; i < reader_count; i++) {
            if (readers[i] != nullptr) {
                readers[i]->print_Reader();
            }
        }

        printf("Список выдач:\n");
        for (int i = 0; i < loan_count; i++) {
            if (loans[i] != nullptr) {
                loans[i]->print_Loan();
            }
        }
    }

    // Метод для добавления автора
    void add_Author() {
        if (book_count < MAX_AUTHORS) {
            authors[book_count] = new Author();
            authors[book_count]->add_Author();
            book_count++;
        }
        else {
            printf("Ошибка: достигнуто максимальное количество авторов.\n");
        }
    }

    // Метод для добавления книги
    void add_Book() {
        if (book_count < MAX_BOOKS) {
            books[book_count] = new Book();
            books[book_count]->add_Book(authors, book_count);
            book_count++;
        }
        else {
            printf("Ошибка: достигнуто максимальное количество книг.\n");
        }
    }

    // Метод для добавления читателя
    void add_Reader() {
        if (reader_count < MAX_READERS) {
            readers[reader_count] = new Reader();
            readers[reader_count]->add_Reader();
            reader_count++;
        }
        else {
            printf("Ошибка: достигнуто максимальное количество читателей.\n");
        }
    }

    // Метод для выдачи книги читателю
    void add_Loan() {
        if (loan_count < MAX_LOANS) {
            if (book_count == 0) {
                printf("Ошибка: нет книг в библиотеке.\n");
                return;
            }

            int book_index, reader_index;
            try {
                // Выбор книги
                printf("Выберите книгу (введите номер): ");
                for (int i = 0; i < book_count; i++) {
                    if (books[i] != nullptr) {
                        printf("%d. %s\n", i + 1, books[i]->get_title().c_str());
                    }
                }
                scanf("%d", &book_index);
                if (book_index < 1 || book_index > book_count || books[book_index - 1] == nullptr)
                    throw std::out_of_range("Некорректный номер книги.");

                // Выбор читателя
                printf("Выберите читателя (введите номер): ");
                for (int i = 0; i < reader_count; i++) {
                    if (readers[i] != nullptr) {
                        printf("%d. %s\n", i + 1, readers[i]->get_fio().c_str());
                    }
                }
                scanf("%d", &reader_index);
                if (reader_index < 1 || reader_index > reader_count || readers[reader_index - 1] == nullptr)
                    throw std::out_of_range("Некорректный номер читателя.");

                // Проверка доступности книги
                if (!is_book_available(*books[book_index - 1], 1)) {
                    throw std::runtime_error("Недостаточно экземпляров книги.");
                }

                // Ввод дат выдачи и возврата
                std::string issue_date, return_date;
                printf("Введите дату выдачи книги (дд.мм.гггг): ");
                std::cin >> issue_date;
                printf("Введите дату возврата книги (дд.мм.гггг): ");
                std::cin >> return_date;

                // Создание новой выдачи
                loans[loan_count] = new Loan(books[book_index - 1], readers[reader_index - 1], issue_date, return_date);
                loan_count++;

                // Увеличение счетчика взятых книг у читателя (демонстрация работы с указателем и ссылкой)
                int* count_ptr = readers[reader_index - 1]->get_borrowed_count_ptr();
                (*count_ptr)++;

                int& count_ref = readers[reader_index - 1]->get_borrowed_count_ref();
                count_ref++;
            }
            catch (const std::exception& e) {
                std::cerr << "Ошибка при создании выдачи: " << e.what() << std::endl;
            }
        }
    }

    // Деструктор - освобождение памяти
    ~Library() {
        for (int i = 0; i < MAX_AUTHORS; i++) {
            delete authors[i];
        }
        for (int i = 0; i < book_count; i++) {
            delete books[i];
        }
        for (int i = 0; i < reader_count; i++) {
            delete readers[i];
        }
        for (int i = 0; i < loan_count; i++) {
            delete loans[i];
        }
    }
};

// Главная функция программы
int main() {
    // Установка кодировки для работы с русским языком
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Library library;  // Создание объекта библиотеки

    Author* author_matrix[2][2];
    std::cout << "\nДвумерный массив авторов:\n";
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            author_matrix[i][j] = new Author();
            std::cout << "Введите автора [" << i << "][" << j << "]:\n";
            author_matrix[i][j]->add_Author();
        }
    }
    std::cout << "\nВывод авторов из двумерного массива:\n";
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            std::cout << "Автор [" << i << "][" << j << "]: ";
            author_matrix[i][j]->print_Author();
        }
    }

    printf("БИБЛИОТЕЧНЫЙ УЧЁТ\n");

    int choice;
    do {
        // Вывод меню
        printf("\nМеню:\n");
        printf("1. Добавить автора\n");
        printf("2. Добавить книгу\n");
        printf("3. Добавить читателя\n");
        printf("4. Выдать книгу\n");
        printf("5. Просмотреть все данные\n");
        printf("6. Выход\n");
        printf("Выберите действие: ");
        scanf("%d", &choice);

        // Обработка выбора пользователя
        switch (choice) {
        case 1:
            library.add_Author();
            break;
        case 2:
            library.add_Book();
            break;
        case 3:
            library.add_Reader();
            break;
        case 4:
            library.add_Loan();
            break;
        case 5:
            library.print_Library();
            break;
        case 6:
            printf("Выход из программы.\n");
            break;
        default:
            printf("Неверный выбор. Попробуйте снова.\n");
        }
    } while (choice != 6);

    return 0;
}