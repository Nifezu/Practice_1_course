#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <windows.h>

//Структура узла бинарного дерева
struct TreeNode {
    int data; //значение узла
    struct TreeNode* left;   //указатель на левое поддерево
    struct TreeNode* right;   //указатель на правое поддерево
};

long long compareCount = 0;  //кол-во сравнений
long long swapCount = 0;  //кол-во операций вставки
double timeSec = 0;   //время сортировки

//Функция создания нового узла
struct TreeNode* createNode(int value) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    if (newNode == NULL) {
        printf("Ошибка выделения памяти!\n");
        exit(1);
    }
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}
//Итеративная вставка элемента в дерево
struct TreeNode* insert(struct TreeNode* root, int value) {
    // Если дерево пустое, создаём корень
    if (root == NULL) {
        swapCount++;
        return createNode(value);
    }

    struct TreeNode* current = root;

    while (1) {
        compareCount++; //каждое сравнение с текущим узлом
        if (value < current->data) {
            //в левое поддерево
            if (current->left == NULL) {
                current->left = createNode(value);
                swapCount++;  //вставка левого потомка
                break;
            }
            else {
                current = current->left;
            }
        }
        else {
            //в правое поддерево
            if (current->right == NULL) {
                current->right = createNode(value);
                swapCount++; //вставка правого потомка
                break;
            }
            else {
                current = current->right;
            }
        }
    }
    return root;   //корень не меняется
}
//Рекурсивный симметричный обход
void inorder(struct TreeNode* root, int* result, int* index) {
    if (root == NULL) return;
    inorder(root->left, result, index);  //обход левого поддерева
    result[(*index)++] = root->data;   //запись значения текущего узла
    swapCount++;  //извлечение элемента
    inorder(root->right, result, index);     //обход правого поддерева
}
//Рекурсивное освобождение памяти дерева
void freeTree(struct TreeNode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}
//Основная функция сортировки
void binaryTreeSort(int* A, int n) {
    struct TreeNode* root = NULL;
    compareCount = 0;
    swapCount = 0;

    // Построение дерева
    for (int i = 0; i < n; i++) {
        root = insert(root, A[i]);
    }
    //Получение отсортированного массива через обход
    int index = 0;
    inorder(root, A, &index);
    freeTree(root);
}
//Генерация случайного массива (диапазон -1000..1000)
void generateArray(int* A, int n) {
    for (int i = 0; i < n; i++) {
        A[i] = rand() % 2001 - 1000;
    }
}

// Запись массива в CSV-файл (числа через запятую)
void writeToFile(int* A, int n, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (f == NULL) {
        printf("Ошибка открытия файла %s!\n", filename);
        return;
    }
    for (int i = 0; i < n; i++) {
        fprintf(f, "%d", A[i]);
        if (i != n - 1) fprintf(f, ",");
    }
    fclose(f);
    printf("Данные записаны в файл: %s\n", filename);
}

// Чтение массива из CSV-файла
int readFromFile(int* A, int maxSize, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("Ошибка открытия файла %s!\n", filename);
        return -1;
    }
    int count = 0;
    char buffer[100];
    // Читаем числа, разделённые запятой или переводом строки
    while (fscanf(f, "%[^,\n]%*c", buffer) != EOF && count < maxSize) {
        int val;
        if (sscanf(buffer, "%d", &val) == 1) {
            A[count++] = val;
        }
    }
    fclose(f);
    return count;
}
//Вывод массива на экран в отформатированном виде
void printArray(int* A, int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", A[i]);
        if (i != n - 1) printf(", ");
    }
    printf("]\n");
}
//Сортировка вставками для сравнения производительности
void insertionSort(int* A, int n) {
    for (int i = 1; i < n; i++) {
        int key = A[i];
        int j = i - 1;
        while (j >= 0 && A[j] > key) {
            A[j + 1] = A[j];
            j--;
        }
        A[j + 1] = key;
    }
}

//меню
void showMenu() {
    printf("\nДВОИЧНАЯ СОРТИРОВКА\n");
    printf("1 - Загрузить данные из CSV-файла\n");
    printf("2 - Сгенерировать случайный массив\n");
    printf("3 - Выполнить сортировку\n");
    printf("4 - Сохранить результат в CSV-файл\n");
    printf("5 - Показать статистику\n");
    printf("6 - Сравнить с сортировкой вставками\n");
    printf("Для выхода закройте окно консоли.\n");
    printf("Ваш вариант: ");
}

int main() {
    //Настройка кодировки для корректного отображения русского языка
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");
    //Инициализация генератора случайных чисел
    srand((unsigned)time(NULL));

    int A[10000];   //основной массив
    int sorted[10000];  //копия для сортировки
    int size = 0;     //текущий размер
    int choice;             //выбор пункта меню
    clock_t start, end;     //для замера времени
    char filename[100];     // имя файла

    printf("Программа сортировки массива методом двоичного дерева.\n");
    printf("Для завершения работы закройте окно.\n");

    // Бесконечный цикл меню
    do {
        showMenu();
        scanf("%d", &choice);

        switch (choice) {
        case 1: {
            //Загрузка данных из CSV файла
            printf("Введите имя файла (например, data.csv): ");
            scanf("%s", filename);
            size = readFromFile(A, 10000, filename);
            if (size > 0) {
                printf("Загружено %d элементов:\n", size);
                printArray(A, size < 20 ? size : 20);
                if (size > 20) printf("... (показано первые 20)\n");
            }
            else {
                printf("Не удалось загрузить данные из файла!\n");
            }
            break;
        }
        case 2: {
            // Генерация случайного массива
            printf("Введите количество элементов: ");
            scanf("%d", &size);
            if (size > 10000) {
                printf("Максимальный размер: 10000!\n");
                break;
            }
            if (size <= 0) {
                printf("Размер должен быть больше 0!\n");
                break;
            }
            generateArray(A, size);
            printf("Сгенерирован массив из %d элементов:\n", size);
            printArray(A, size < 20 ? size : 20);
            if (size > 20) printf("... (показано первые 20)\n");
            break;
        }
        case 3: {
            //Сортировка
            if (size == 0) {
                printf("Нет данных для сортировки!\n");
                break;
            }
            //Копируем исходный массив
            for (int i = 0; i < size; i++) sorted[i] = A[i];

            printf("Выполняется сортировка...\n");
            start = clock();
            binaryTreeSort(sorted, size);
            end = clock();
            timeSec = (double)(end - start) / CLOCKS_PER_SEC;

            printf("Отсортированный массив:\n");
            printArray(sorted, size < 20 ? size : 20);
            if (size > 20) printf("... (показано первые 20)\n");

            //Сохраняем результат обратно в A
            for (int i = 0; i < size; i++) A[i] = sorted[i];
            break;
        }
        case 4: {
            //Сохранение в CSV
            if (size == 0) {
                printf("Нет данных для сохранения!\n");
                break;
            }
            printf("Введите имя файла (например, result.csv): ");
            scanf("%s", filename);
            writeToFile(A, size, filename);
            break;
        }
        case 5: {
            //Показ статистики
            if (size == 0) {
                printf("Нет данных!\n");
                break;
            }
            printf("Статистика:\n");
            printf("Размер: %d\n", size);
            printf("Сравнений: %lld\n", compareCount);
            printf("Перестановок: %lld\n", swapCount);
            printf("Время: %.6f сек\n", timeSec);
            break;
        }
        case 6: {
            //Сравнение с сортировкой вставками
            if (size == 0) {
                printf("Нет данных для сравнения!\n");
                break;
            }
            //Копируем данные для обеих сортировок
            int ourSorted[10000], stdSorted[10000];
            for (int i = 0; i < size; i++) ourSorted[i] = A[i];
            for (int i = 0; i < size; i++) stdSorted[i] = A[i];

            clock_t t1, t2;
            t1 = clock();
            binaryTreeSort(ourSorted, size);
            t1 = clock() - t1;
            double ourTime = (double)t1 / CLOCKS_PER_SEC;

            t2 = clock();
            insertionSort(stdSorted, size);
            t2 = clock() - t2;
            double stdTime = (double)t2 / CLOCKS_PER_SEC;

            printf("Сравнение:\n");
            printf("Двоичная сортировка: %.6f сек\n", ourTime);
            printf("Сортировка вставками: %.6f сек\n", stdTime);
            if (ourTime < stdTime && stdTime > 0)
                printf("Двоичная БЫСТРЕЕ в %.2f раз\n", stdTime / ourTime);
            else if (ourTime > stdTime && ourTime > 0)
                printf("Сортировка вставками БЫСТРЕЕ в %.2f раз\n", ourTime / stdTime);
            else
                printf("Время одинаковое.\n");
            break;
        }
        default:
            printf("Неверный выбор! Введите 1-6.\n");
        }

        printf("\n"); //разделитель между операциями

    } while (1);  //выход только при закрытии окна

    return 0;
}