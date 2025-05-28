#define _CRT_SECURE_NO_WARNINGS  // Отключаем предупреждения безопасности CRT
#include <iostream>
#include <cstring>
#include <Windows.h>  // Для работы с консолью Windows
#include <cstdlib>
#include <string>
#include <vector>
#include <memory> // Для умных указателей

// Константы для ограничения размеров массивов
const int MAX_BORROWED_BOOKS = 100;
const int MAX_BOOKS = 100;
const int MAX_READERS = 100;
const int MAX_LOANS = 100;
const int MAX_NAME_LENGTH = 30;
const int MAX_AUTHORS = 100;

// Абстрактный базовый класс для персоналий
class AbstractPerson {
public:
    // Чисто виртуальная функция (делает класс абстрактным)
    virtual void displayInfo() const = 0;
    // Виртуальный деструктор для корректного удаления объектов
    virtual ~AbstractPerson() {}
};

// Базовый класс Author с наследованием от абстрактного класса
class Author : public AbstractPerson {
protected: // Модификатор protected для демонстрации
    std::string fio; // ФИО автора
    int birth_year; // Год рождения
    static int total_authors; // Статическая переменная для подсчета авторов

public:
    // Конструктор по умолчанию
    Author() : birth_year(0) {
        total_authors++;
    }

    // Конструктор с параметрами
    Author(const std::string& name, int year) : fio(name), birth_year(year) {
        total_authors++;
    }

    // Статический метод для получения общего количества авторов
    static int get_total_authors() {
        return total_authors;
    }

    // Геттер для ФИО
    const std::string& get_fio() const {
        return fio;
    }

    // Геттер для года рождения
    int get_birth_year() const {
        return birth_year;
    }

    // Метод для добавления информации об авторе
    void add_Author() {
        printf("Введите ФИО автора: ");
        while (getchar() != '\n'); // Очистка буфера
        std::getline(std::cin, this->fio);

        printf("Введите год рождения автора: ");
        std::string year_str;
        std::getline(std::cin, year_str);
        this->birth_year = std::stoi(year_str);
    }

    // Реализация чисто виртуальной функции из AbstractPerson
    void displayInfo() const override {
        print_Author();
    }

    // Виртуальный метод для вывода информации об авторе
    virtual void print_Author() const {
        printf("ФИО автора: %s\n", this->get_fio().c_str());
        printf("Год рождения автора: %d\n", this->get_birth_year());
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

    // Перегрузка оператора вывода
    friend std::ostream& operator<<(std::ostream& os, const Author& author) {
        os << "Автор: " << author.fio << " (род. " << author.birth_year << ")";
        return os;
    }
};

// Инициализация статической переменной
int Author::total_authors = 0;

// Производный класс FamousAuthor (демонстрация наследования)
class FamousAuthor : public Author {
    std::string most_famous_work; // Самое известное произведение
    int awards_count; // Количество наград

public:
    // Конструктор с вызовом конструктора базового класса
    FamousAuthor(const std::string& name, int year, const std::string& work, int awards)
        : Author(name, year), most_famous_work(work), awards_count(awards) {
    }

    // Перегрузка метода print_Author с вызовом базового метода
    void print_Author() const override {
        Author::print_Author(); // Вызов метода базового класса
        printf("Самое известное произведение: %s\n", most_famous_work.c_str());
        printf("Количество наград: %d\n", awards_count);
    }

    // Перегрузка метода print_Author без вызова базового метода
    void print_Author(bool short_version) const {
        if (short_version) {
            printf("Известный автор: %s, наград: %d\n", fio.c_str(), awards_count);
        }
    }

    // Перегрузка оператора присваивания для базового класса
    FamousAuthor& operator=(const Author& other) {
        if (this != &other) {
            this->fio = other.get_fio();
            this->birth_year = other.get_birth_year();
            this->most_famous_work = "Не указано";
            this->awards_count = 0;
        }
        return *this;
    }

    // Перегрузка оператора вывода
    friend std::ostream& operator<<(std::ostream& os, const FamousAuthor& author) {
        os << static_cast<const Author&>(author)
            << "\nИзвестное произведение: " << author.most_famous_work
            << "\nНаград: " << author.awards_count;
        return os;
    }
};

// Шаблон класса для библиотечной карточки
template <typename T>
class LibraryCard {
    T id; // Идентификатор карточки (может быть разного типа)
    std::string issue_date; // Дата выдачи
    std::string expire_date; // Дата истечения
public:
    // Конструктор
    LibraryCard(T id, const std::string& issue, const std::string& expire)
        : id(id), issue_date(issue), expire_date(expire) {
    }

    // Метод для отображения информации о карточке
    void display() const {
        std::cout << "Карточка #" << id
            << "\nВыдана: " << issue_date
            << "\nДействительна до: " << expire_date << std::endl;
    }
};

// Класс Book для представления книг в библиотеке
class Book {
    std::string title; // Название книги
    Author* author; // Указатель на автора
    int pub_year; // Год публикации
    int copies; // Количество экземпляров

public:
    // Конструктор по умолчанию
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

        if (this->title.empty()) {
            throw std::invalid_argument("Название книги не может быть пустым.");
        }

        printf("Выберите автора (введите номер): ");
        for (int i = 0; i < num_authors; i++) {
            if (authors[i] != nullptr) {
                printf("%d. %s\n", i + 1, authors[i]->get_fio().c_str());
            }
        }

        int author_index;
        scanf("%d", &author_index);
        if (author_index < 1 || author_index > num_authors || authors[author_index - 1] == nullptr) {
            throw std::out_of_range("Некорректный номер автора.");
        }
        this->author = authors[author_index - 1];

        printf("Введите дату публикации: ");
        scanf("%d", &this->pub_year);
        printf("Введите количество экземпляров: ");
        scanf("%d", &this->copies);
    }

    // Виртуальный метод для вывода информации о книге
    virtual void print_Book() const {
        if (this->title.empty() || this->author == nullptr) {
            printf("Ошибка: некорректные данные о книге.\n");
            return;
        }
        printf("Название книги: %s\n", this->title.c_str());
        printf("Автор книги: %s\n", this->author->get_fio().c_str());
        printf("Дата публикации: %d\n", this->pub_year);
        printf("Количество экземпляров: %d\n", this->copies);
    }

    // Не виртуальная функция, вызывающая виртуальную
    void displayBookInfo() const {
        print_Book();
    }

    // Дружественная функция для проверки доступности книги
    friend bool is_book_available(const Book& book, int required_copies);
    // Дружественная функция для перегрузки оператора вывода
    friend std::ostream& operator<<(std::ostream& os, const Book& book);
};

// Функция проверки доступности книги
bool is_book_available(const Book& book, int required_copies) {
    return book.copies >= required_copies;
}

// Перегрузка оператора вывода для Book
std::ostream& operator<<(std::ostream& os, const Book& book) {
    os << "Книга: " << book.title
        << "\nАвтор: " << book.author->get_fio()
        << "\nГод публикации: " << book.pub_year
        << "\nЭкземпляров: " << book.copies;
    return os;
}

// Класс Reader для представления читателей
class Reader : public AbstractPerson {
    std::string fio; // ФИО читателя
    int card_number; // Номер читательского билета
    Book* borrowed_books[MAX_BORROWED_BOOKS]; // Массив взятых книг
    int borrowed_count; // Количество взятых книг

public:
    // Конструктор по умолчанию
    Reader() : card_number(0), borrowed_count(0) {}

    // Установка количества взятых книг
    void set_borrowed_count() {
        borrowed_count++;
    }

    // Геттер для ФИО
    std::string& get_fio() {
        return fio;
    }

    // Геттер для номера билета
    int get_card_number() const {
        return card_number;
    }

    // Получение указателя на borrowed_count
    int* get_borrowed_count_ptr() {
        return &borrowed_count;
    }

    // Получение ссылки на borrowed_count
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

    // Реализация чисто виртуальной функции из AbstractPerson
    void displayInfo() const override {
        print_Reader();
    }

    // Виртуальный метод для вывода информации о читателе
    virtual void print_Reader() const {
        if (this->fio.empty()) {
            printf("Ошибка: некорректные данные о читателе.\n");
            return;
        }
        printf("ФИО читателя: %s\n", this->fio.c_str());
        printf("Номер читательского билета: %d\n", this->card_number);
        printf("Количество взятых книг: %d\n", this->borrowed_count);
    }

    // Дружественная функция для перегрузки оператора вывода
    friend std::ostream& operator<<(std::ostream& os, const Reader& reader);
};

// Перегрузка оператора вывода для Reader
std::ostream& operator<<(std::ostream& os, const Reader& reader) {
    os << "Читатель: " << reader.fio
        << "\nНомер билета: " << reader.card_number
        << "\nВзято книг: " << reader.borrowed_count;
    return os;
}

// Класс Loan для представления выдачи книг
class Loan {
    Book* book; // Указатель на книгу
    Reader* reader; // Указатель на читателя
    std::string issue_date; // Дата выдачи
    std::string return_date; // Дата возврата

public:
    // Конструктор по умолчанию
    Loan() : book(nullptr), reader(nullptr) {}

    // Конструктор с параметрами
    Loan(Book* book, Reader* reader, const std::string& issue_date, const std::string& return_date)
        : book(book), reader(reader), issue_date(issue_date), return_date(return_date) {
    }

    // Геттер для даты выдачи
    std::string get_issue_date() const {
        return issue_date;
    }

    // Геттер для даты возврата
    std::string get_return_date() const {
        return return_date;
    }

    // Метод для вывода информации о выдаче
    void print_Loan() const {
        if (book == nullptr || reader == nullptr || book->get_title().empty() || reader->get_fio().empty()) {
            printf("Ошибка: некорректные данные о выдаче.\n");
            return;
        }
        printf("Название выданной книги: %s\n", book->get_title().c_str());
        printf("Читатель: %s\n", reader->get_fio().c_str());
        printf("Дата выдачи книги: %s\n", issue_date.c_str());
        printf("Дата возврата книги: %s\n", return_date.c_str());
    }

    // Дружественная функция для перегрузки оператора вывода
    friend std::ostream& operator<<(std::ostream& os, const Loan& loan);
};

// Перегрузка оператора вывода для Loan
std::ostream& operator<<(std::ostream& os, const Loan& loan) {
    os << "Выдача книги:\n"
        << "Книга: " << loan.book->get_title()
        << "\nЧитатель: " << loan.reader->get_fio()
        << "\nДата выдачи: " << loan.issue_date
        << "\nДата возврата: " << loan.return_date;
    return os;
}

// Класс Library - основной класс библиотеки
class Library {
    Author* authors[MAX_AUTHORS]; // Массив авторов
    Book* books[MAX_BOOKS]; // Массив книг
    int book_count; // Количество книг
    Reader* readers[MAX_READERS]; // Массив читателей
    int reader_count; // Количество читателей
    Loan* loans[MAX_LOANS]; // Массив выдач
    int loan_count; // Количество выдач

public:
    // Конструктор
    Library() : book_count(0), reader_count(0), loan_count(0) {
        // Инициализация указателей нулевыми значениями
        for (int i = 0; i < MAX_AUTHORS; i++) authors[i] = nullptr;
        for (int i = 0; i < MAX_BOOKS; i++) books[i] = nullptr;
        for (int i = 0; i < MAX_READERS; i++) readers[i] = nullptr;
        for (int i = 0; i < MAX_LOANS; i++) loans[i] = nullptr;
    }

    // Геттер для количества книг
    int get_book_count() const {
        return book_count;
    }

    // Геттер для количества читателей
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
                authors[i]->displayInfo(); // Вызов через виртуальную функцию
            }
        }

        printf("\nСписок книг:\n");
        for (int i = 0; i < book_count; i++) {
            if (books[i] != nullptr) {
                std::cout << *books[i] << "\n\n"; // Использование оператора <<
            }
        }

        printf("Список читателей:\n");
        for (int i = 0; i < reader_count; i++) {
            if (readers[i] != nullptr) {
                std::cout << *readers[i] << "\n\n"; // Использование оператора <<
            }
        }

        printf("Список выдач:\n");
        for (int i = 0; i < loan_count; i++) {
            if (loans[i] != nullptr) {
                std::cout << *loans[i] << "\n\n"; // Использование оператора <<
            }
        }
    }

    // Метод для добавления автора
    void add_Author() {
        if (book_count < MAX_AUTHORS) {
            // Создаем FamousAuthor вместо обычного Author для демонстрации
            authors[book_count] = new FamousAuthor("", 0, "", 0);
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

    // Метод для добавления выдачи книги
    void add_Loan() {
        if (loan_count < MAX_LOANS) {
            if (book_count == 0) {
                printf("Ошибка: нет книг в библиотеке.\n");
                return;
            }

            int book_index, reader_index;
            try {
                printf("Выберите книгу (введите номер): ");
                for (int i = 0; i < book_count; i++) {
                    if (books[i] != nullptr) {
                        printf("%d. %s\n", i + 1, books[i]->get_title().c_str());
                    }
                }
                scanf("%d", &book_index);
                if (book_index < 1 || book_index > book_count || books[book_index - 1] == nullptr)
                    throw std::out_of_range("Некорректный номер книги.");

                printf("Выберите читателя (введите номер): ");
                for (int i = 0; i < reader_count; i++) {
                    if (readers[i] != nullptr) {
                        printf("%d. %s\n", i + 1, readers[i]->get_fio().c_str());
                    }
                }
                scanf("%d", &reader_index);
                if (reader_index < 1 || reader_index > reader_count || readers[reader_index - 1] == nullptr)
                    throw std::out_of_range("Некорректный номер читателя.");

                if (!is_book_available(*books[book_index - 1], 1)) {
                    throw std::runtime_error("Недостаточно экземпляров книги.");
                }

                std::string issue_date, return_date;
                printf("Введите дату выдачи книги (дд.мм.гггг): ");
                std::cin >> issue_date;
                printf("Введите дату возврата книги (дд.мм.гггг): ");
                std::cin >> return_date;

                loans[loan_count] = new Loan(books[book_index - 1], readers[reader_index - 1], issue_date, return_date);
                loan_count++;

                // Демонстрация работы с указателями и ссылками
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

    // Деструктор для освобождения памяти
    ~Library() {
        for (int i = 0; i < MAX_AUTHORS; i++) delete authors[i];
        for (int i = 0; i < book_count; i++) delete books[i];
        for (int i = 0; i < reader_count; i++) delete readers[i];
        for (int i = 0; i < loan_count; i++) delete loans[i];
    }
};

// Функция для демонстрации виртуальных функций
void demonstrateVirtualFunctions() {
    // Создаем объекты базового и производного классов
    Author* author = new Author("Лев Толстой", 1828);
    FamousAuthor* famousAuthor = new FamousAuthor("Фёдор Достоевский", 1821, "Преступление и наказание", 5);

    // Демонстрация работы виртуальной функции
    std::cout << "\nДемонстрация виртуальных функций:\n";
    Author* ptr = author;
    ptr->displayInfo(); // Вызовется Author::print_Author()

    ptr = famousAuthor;
    ptr->displayInfo(); // Вызовется FamousAuthor::print_Author()

    // Демонстрация без виртуальной функции
    std::cout << "\nДемонстрация без виртуальной функции:\n";
    ptr = author;
    ptr->print_Author(); // Вызовется Author::print_Author()

    ptr = famousAuthor;
    ptr->print_Author(); // Вызовется FamousAuthor::print_Author()

    // Освобождение памяти
    delete author;
    delete famousAuthor;
}

// Функция для демонстрации абстрактного класса
void demonstrateAbstractClass() {
    // AbstractPerson* person = new AbstractPerson(); // Ошибка - нельзя создать экземпляр абстрактного класса
    AbstractPerson* person = new Reader();
    person->displayInfo(); // Вызовется Reader::print_Reader()
    delete person;
}

// Функция для демонстрации шаблона класса
void demonstrateTemplateClass() {
    // Создаем карточки с разными типами идентификаторов
    LibraryCard<int> intCard(12345, "01.01.2023", "31.12.2025");
    LibraryCard<std::string> stringCard("ABC-123", "01.01.2023", "31.12.2025");

    std::cout << "\nДемонстрация шаблонного класса:\n";
    std::cout << "Карточка с числовым ID:\n";
    intCard.display();
    std::cout << "\nКарточка со строковым ID:\n";
    stringCard.display();
}

int main() {
    // Установка кодировки для корректного отображения кириллицы
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Демонстрационные функции
    demonstrateVirtualFunctions();
    demonstrateAbstractClass();
    demonstrateTemplateClass();

    // Создание объекта библиотеки
    Library library;


    printf("БИБЛИОТЕЧНЫЙ УЧЁТ\n");

    // Основное меню программы
    int choice;
    do {
        printf("\nМеню:\n");
        printf("1. Добавить автора\n");
        printf("2. Добавить книгу\n");
        printf("3. Добавить читателя\n");
        printf("4. Выдать книгу\n");
        printf("5. Просмотреть все данные\n");
        printf("6. Выход\n");
        printf("Выберите действие: ");
        scanf("%d", &choice);

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

    // Очистка памяти
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            delete author_matrix[i][j];
        }
    }

    return 0;
}