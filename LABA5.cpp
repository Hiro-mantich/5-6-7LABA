#define _CRT_SECURE_NO_WARNINGS  // Отключаем предупреждения безопасности CRT
#include <iostream>
#include <cstring>
#include <Windows.h>  // Для работы с консолью Windows
#include <cstdlib>
#include <string>
#include <vector>
#include <memory> // Для умных указателей
#include <algorithm> // Для алгоритмов STL
#include <map> // Для ассоциативного массива
#include <ctime> // Для работы с датами

// Константы для ограничения размеров массивов (оставлены для совместимости)
const int MAX_BORROWED_BOOKS = 100;
const int MAX_NAME_LENGTH = 30;

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

    // Оператор сравнения для сортировки
    bool operator<(const Author& other) const {
        return this->fio < other.fio;
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
    std::shared_ptr<Author> author; // Умный указатель на автора
    int pub_year; // Год публикации
    int copies; // Количество экземпляров
    std::string isbn; // Уникальный идентификатор книги

public:
    // Конструктор по умолчанию
    Book() : pub_year(0), copies(0) {}

    // Геттер для названия книги
    const std::string& get_title() const {
        return title;
    }

    // Геттер для ISBN
    const std::string& get_isbn() const {
        return isbn;
    }

    // Метод для добавления книги
    void add_Book(const std::vector<std::shared_ptr<Author>>& authors) {
        printf("Введите название книги: \n");
        while (getchar() != '\n'); // Очистка буфера
        std::getline(std::cin, this->title);

        if (this->title.empty()) {
            throw std::invalid_argument("Название книги не может быть пустым.");
        }

        printf("Введите ISBN книги: ");
        std::getline(std::cin, this->isbn);

        printf("Выберите автора (введите номер): ");
        for (size_t i = 0; i < authors.size(); i++) {
            if (authors[i] != nullptr) {
                printf("%zu. %s\n", i + 1, authors[i]->get_fio().c_str());
            }
        }

        int author_index;
        scanf("%d", &author_index);
        if (author_index < 1 || author_index > static_cast<int>(authors.size()) || authors[author_index - 1] == nullptr) {
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
        printf("ISBN: %s\n", this->isbn.c_str());
        printf("Автор книги: %s\n", this->author->get_fio().c_str());
        printf("Дата публикации: %d\n", this->pub_year);
        printf("Количество экземпляров: %d\n", this->copies);
    }

    // Не виртуальная функция, вызывающая виртуальную
    void displayBookInfo() const {
        print_Book();
    }

    // Оператор сравнения для сортировки книг по названию
    bool operator<(const Book& other) const {
        return this->title < other.title;
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
        << "\nISBN: " << book.isbn
        << "\nАвтор: " << book.author->get_fio()
        << "\nГод публикации: " << book.pub_year
        << "\nЭкземпляров: " << book.copies;
    return os;
}

// Класс Reader для представления читателей
class Reader : public AbstractPerson {
    std::string fio; // ФИО читателя
    int card_number; // Номер читательского билета
    std::vector<std::shared_ptr<Book>> borrowed_books; // Вектор взятых книг
    std::map<std::string, std::string> borrowed_dates; // Дата взятия каждой книги (ISBN -> дата)

public:
    // Конструктор по умолчанию
    Reader() : card_number(0) {}

    // Установка количества взятых книг
    void set_borrowed_count() {
        borrowed_books.push_back(nullptr); // Просто для демонстрации
    }

    // Геттер для ФИО
    const std::string& get_fio() const {
        return fio;
    }

    // Геттер для номера билета
    int get_card_number() const {
        return card_number;
    }

    // Получение количества взятых книг
    size_t get_borrowed_count() const {
        return borrowed_books.size();
    }

    // Метод для добавления читателя
    void add_Reader() {
        printf("Введите ФИО читателя: \n");
        while (getchar() != '\n'); // Очистка буфера
        std::getline(std::cin, this->fio);

        printf("Введите номер читательского билета: ");
        scanf("%d", &this->card_number);
    }

    // Метод для добавления взятой книги
    void add_borrowed_book(const std::shared_ptr<Book>& book, const std::string& date) {
        borrowed_books.push_back(book);
        borrowed_dates[book->get_isbn()] = date;
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
        printf("Количество взятых книг: %zu\n", this->borrowed_books.size());

        // Вывод информации о взятых книгах
        for (const auto& book : borrowed_books) {
            if (book) {
                auto it = borrowed_dates.find(book->get_isbn());
                std::string date = (it != borrowed_dates.end()) ? it->second : "неизвестно";
                printf(" - %s (взято: %s)\n", book->get_title().c_str(), date.c_str());
            }
        }
    }

    // Оператор сравнения для сортировки читателей по ФИО
    bool operator<(const Reader& other) const {
        return this->fio < other.fio;
    }

    // Дружественная функция для перегрузки оператора вывода
    friend std::ostream& operator<<(std::ostream& os, const Reader& reader);
};

// Перегрузка оператора вывода для Reader
std::ostream& operator<<(std::ostream& os, const Reader& reader) {
    os << "Читатель: " << reader.fio
        << "\nНомер билета: " << reader.card_number
        << "\nВзято книг: " << reader.borrowed_books.size();
    return os;
}

// Класс Loan для представления выдачи книг
class Loan {
    std::shared_ptr<Book> book; // Умный указатель на книгу
    std::shared_ptr<Reader> reader; // Умный указатель на читателя
    std::string issue_date; // Дата выдачи
    std::string return_date; // Дата возврата

public:
    // Конструктор по умолчанию
    Loan() {}

    // Конструктор с параметрами
    Loan(const std::shared_ptr<Book>& book, const std::shared_ptr<Reader>& reader,
        const std::string& issue_date, const std::string& return_date)
        : book(book), reader(reader), issue_date(issue_date), return_date(return_date) {
    }

    // Геттер для даты выдачи
    const std::string& get_issue_date() const {
        return issue_date;
    }

    // Геттер для даты возврата
    const std::string& get_return_date() const {
        return return_date;
    }

    // Метод для вывода информации о выдаче
    void print_Loan() const {
        if (!book || !reader || book->get_title().empty() || reader->get_fio().empty()) {
            printf("Ошибка: некорректные данные о выдаче.\n");
            return;
        }
        printf("Название выданной книги: %s\n", book->get_title().c_str());
        printf("Читатель: %s\n", reader->get_fio().c_str());
        printf("Дата выдачи книги: %s\n", issue_date.c_str());
        printf("Дата возврата книги: %s\n", return_date.c_str());
    }

    // Оператор сравнения для сортировки выдач по дате
    bool operator<(const Loan& other) const {
        return this->issue_date < other.issue_date;
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
    std::vector<std::shared_ptr<Author>> authors; // Вектор авторов
    std::vector<std::shared_ptr<Book>> books; // Вектор книг
    std::vector<std::shared_ptr<Reader>> readers; // Вектор читателей
    std::vector<std::shared_ptr<Loan>> loans; // Вектор выдач
    std::map<std::string, std::shared_ptr<Book>> isbn_index; // Индекс книг по ISBN для быстрого поиска

public:
    // Метод для сортировки книг по названию
    void sort_books_by_title() {
        std::sort(books.begin(), books.end(),
            [](const std::shared_ptr<Book>& a, const std::shared_ptr<Book>& b) {
                return *a < *b;
            });
    }

    // Метод для сортировки читателей по ФИО
    void sort_readers_by_name() {
        std::sort(readers.begin(), readers.end(),
            [](const std::shared_ptr<Reader>& a, const std::shared_ptr<Reader>& b) {
                return *a < *b;
            });
    }

    // Метод для сортировки выдач по дате
    void sort_loans_by_date() {
        std::sort(loans.begin(), loans.end(),
            [](const std::shared_ptr<Loan>& a, const std::shared_ptr<Loan>& b) {
                return *a < *b;
            });
    }

    // Метод для поиска книги по названию (бинарный поиск после сортировки)
    std::shared_ptr<Book> find_book_by_title(const std::string& title) {
        sort_books_by_title(); // Сначала сортируем
        auto it = std::lower_bound(books.begin(), books.end(), title,
            [](const std::shared_ptr<Book>& book, const std::string& title) {
                return book->get_title() < title;
            });

        if (it != books.end() && (*it)->get_title() == title) {
            return *it;
        }
        return nullptr;
    }

    // Метод для поиска книги по ISBN (используем map для быстрого поиска)
    std::shared_ptr<Book> find_book_by_isbn(const std::string& isbn) {
        auto it = isbn_index.find(isbn);
        if (it != isbn_index.end()) {
            return it->second;
        }
        return nullptr;
    }

    // Метод для поиска читателя по номеру билета
    std::shared_ptr<Reader> find_reader_by_card(int card_number) {
        auto it = std::find_if(readers.begin(), readers.end(),
            [card_number](const std::shared_ptr<Reader>& reader) {
                return reader->get_card_number() == card_number;
            });

        if (it != readers.end()) {
            return *it;
        }
        return nullptr;
    }

    // Метод для вывода всей информации о библиотеке
    void print_Library() {
        printf("Общее количество авторов: %zu\n", authors.size());

        printf("\nСписок авторов (отсортированный по ФИО):\n");
        for (const auto& author : authors) {
            if (author != nullptr) {
                author->displayInfo(); // Вызов через виртуальную функцию
                std::cout << std::endl;
            }
        }

        printf("\nСписок книг (отсортированный по названию):\n");
        sort_books_by_title();
        for (const auto& book : books) {
            if (book != nullptr) {
                std::cout << *book << "\n\n"; // Использование оператора <<
            }
        }

        printf("\nСписок читателей (отсортированный по ФИО):\n");
        sort_readers_by_name();
        for (const auto& reader : readers) {
            if (reader != nullptr) {
                std::cout << *reader << "\n\n"; // Использование оператора <<
            }
        }

        printf("\nСписок выдач (отсортированный по дате):\n");
        sort_loans_by_date();
        for (const auto& loan : loans) {
            if (loan != nullptr) {
                std::cout << *loan << "\n\n"; // Использование оператора <<
            }
        }
    }

    // Метод для добавления автора
    void add_Author() {
        // Создаем FamousAuthor вместо обычного Author для демонстрации
        auto newAuthor = std::make_shared<FamousAuthor>("", 0, "", 0);
        newAuthor->add_Author();
        authors.push_back(newAuthor);
    }

    // Метод для добавления книги
    void add_Book() {
        if (authors.empty()) {
            printf("Ошибка: сначала добавьте хотя бы одного автора.\n");
            return;
        }

        auto newBook = std::make_shared<Book>();
        newBook->add_Book(authors);
        books.push_back(newBook);
        isbn_index[newBook->get_isbn()] = newBook; // Добавляем в индекс для быстрого поиска
    }

    // Метод для добавления читателя
    void add_Reader() {
        auto newReader = std::make_shared<Reader>();
        newReader->add_Reader();
        readers.push_back(newReader);
    }

    // Метод для добавления выдачи книги
    void add_Loan() {
        if (books.empty()) {
            printf("Ошибка: нет книг в библиотеке.\n");
            return;
        }

        if (readers.empty()) {
            printf("Ошибка: нет читателей в библиотеке.\n");
            return;
        }

        try {
            printf("Выберите книгу (введите номер): ");
            for (size_t i = 0; i < books.size(); i++) {
                if (books[i] != nullptr) {
                    printf("%zu. %s\n", i + 1, books[i]->get_title().c_str());
                }
            }
            int book_index;
            scanf("%d", &book_index);
            if (book_index < 1 || book_index > static_cast<int>(books.size()) || !books[book_index - 1])
                throw std::out_of_range("Некорректный номер книги.");

            printf("Выберите читателя (введите номер): ");
            for (size_t i = 0; i < readers.size(); i++) {
                if (readers[i] != nullptr) {
                    printf("%zu. %s\n", i + 1, readers[i]->get_fio().c_str());
                }
            }
            int reader_index;
            scanf("%d", &reader_index);
            if (reader_index < 1 || reader_index > static_cast<int>(readers.size()) || !readers[reader_index - 1])
                throw std::out_of_range("Некорректный номер читателя.");

            if (!is_book_available(*books[book_index - 1], 1)) {
                throw std::runtime_error("Недостаточно экземпляров книги.");
            }

            std::string issue_date, return_date;
            printf("Введите дату выдачи книги (дд.мм.гггг): ");
            std::cin >> issue_date;
            printf("Введите дату возврата книги (дд.мм.гггг): ");
            std::cin >> return_date;

            auto newLoan = std::make_shared<Loan>(books[book_index - 1], readers[reader_index - 1], issue_date, return_date);
            loans.push_back(newLoan);
            readers[reader_index - 1]->add_borrowed_book(books[book_index - 1], issue_date);
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка при создании выдачи: " << e.what() << std::endl;
        }
    }

    // Метод для поиска и вывода информации о книге
    void search_and_print_book() {
        printf("Выберите тип поиска:\n");
        printf("1. По названию\n");
        printf("2. По ISBN\n");
        int choice;
        scanf("%d", &choice);

        std::string search_term;
        printf("Введите поисковый запрос: ");
        while (getchar() != '\n'); // Очистка буфера
        std::getline(std::cin, search_term);

        std::shared_ptr<Book> found_book;
        switch (choice) {
        case 1:
            found_book = find_book_by_title(search_term);
            break;
        case 2:
            found_book = find_book_by_isbn(search_term);
            break;
        default:
            printf("Неверный выбор.\n");
            return;
        }

        if (found_book) {
            std::cout << "\nНайдена книга:\n" << *found_book << std::endl;
        }
        else {
            printf("Книга не найдена.\n");
        }
    }

    // Метод для поиска и вывода информации о читателе
    void search_and_print_reader() {
        printf("Выберите тип поиска:\n");
        printf("1. По номеру билета\n");
        printf("2. По ФИО\n");
        int choice;
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Введите номер читательского билета: ");
            int card_number;
            scanf("%d", &card_number);
            auto reader = find_reader_by_card(card_number);
            if (reader) {
                std::cout << "\nНайден читатель:\n" << *reader << std::endl;
            }
            else {
                printf("Читатель не найден.\n");
            }
        }
        else if (choice == 2) {
            printf("Введите ФИО читателя: ");
            std::string name;
            while (getchar() != '\n'); // Очистка буфера
            std::getline(std::cin, name);

            // Линейный поиск по ФИО (можно было бы использовать сортировку и binary_search)
            bool found = false;
            for (const auto& reader : readers) {
                if (reader && reader->get_fio() == name) {
                    std::cout << "\nНайден читатель:\n" << *reader << std::endl;
                    found = true;
                    break;
                }
            }
            if (!found) {
                printf("Читатель не найден.\n");
            }
        }
        else {
            printf("Неверный выбор.\n");
        }
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
        printf("6. Поиск книги\n");
        printf("7. Поиск читателя\n");
        printf("8. Выход\n");
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
            library.search_and_print_book();
            break;
        case 7:
            library.search_and_print_reader();
            break;
        case 8:
            printf("Выход из программы.\n");
            break;
        default:
            printf("Неверный выбор. Попробуйте снова.\n");
        }
    } while (choice != 8);

    return 0;
}