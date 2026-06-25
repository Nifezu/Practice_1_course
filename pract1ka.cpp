#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <windows.h>
#include <string.h>

//структура узла дерева
struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
};
//глобальные переменные
long long compareCount = 0;
long long swapCount = 0;
double timeSec = 0;

//создание нового узла
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

//вставка элемента в дерево
struct TreeNode* insert(struct TreeNode* root, int value) {
    if (root == NULL) {
        swapCount++;
        return createNode(value);
    }
    struct TreeNode* current = root;
    while (1) {
        compareCount++;
        if (value < current->data) {
            if (current->left == NULL) {
                current->left = createNode(value);
                swapCount++;
                break;
            }
            else {
                current = current->left;
            }
        }
        else {
            if (current->right == NULL) {
                current->right = createNode(value);
                swapCount++;
                break;
            }
            else {
                current = current->right;
            }
        }
    }
    return root;
}
//симметричный обход
void inorder(struct TreeNode* root, int* result, int* index) {
    if (root == NULL) return;
    inorder(root->left, result, index);
    result[(*index)++] = root->data;
    swapCount++;
    inorder(root->right, result, index);
}

//освобождение памяти
void freeTree(struct TreeNode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

//основная сортировка
void binaryTreeSort(int* A, int n) {
    struct TreeNode* root = NULL;
    compareCount = 0;
    swapCount = 0;
    for (int i = 0; i < n; i++) {
        root = insert(root, A[i]);
    }
    int index = 0;
    inorder(root, A, &index);
    freeTree(root);
}
//генерация массива
void generateArray(int* A, int n, int minVal, int maxVal) {
    for (int i = 0; i < n; i++) {
        A[i] = rand() % (maxVal - minVal + 1) + minVal;
    }
}
//запись в csv-файл
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
//чтение из csv-файла
int readFromFile(int* A, int maxSize, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("Ошибка открытия файла %s!\n", filename);
        return -1;
    }
    int count = 0;
    char buffer[100];
    int lineNum = 0;
    while (fscanf(f, "%[^,\n]%*c", buffer) != EOF && count < maxSize) {
        lineNum++;
        char* p = buffer;
        while (*p == ' ' || *p == '\t') p++;
        int len = strlen(p);
        while (len > 0 && (p[len - 1] == ' ' || p[len - 1] == '\t' || p[len - 1] == '\n' || p[len - 1] == '\r')) {
            p[--len] = '\0';
        }
        if (strlen(p) == 0) {
            printf("Предупреждение: пустое значение в строке %d, пропущено.\n", lineNum);
            continue;
        }
        int val;
        if (sscanf(p, "%d", &val) == 1) {
            A[count++] = val;
        }
        else {
            printf("Предупреждение: некорректное значение '%s' в строке %d, пропущено.\n", p, lineNum);
        }
    }
    fclose(f);
    return count;
}
//вывод массива
void printArray(int* A, int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", A[i]);
        if (i != n - 1) printf(", ");
    }
    printf("]\n");
}
//сортировка вставками
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

//очистка буфера
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
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
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");
    srand((unsigned)time(NULL));

    int A[10000];
    int sorted[10000];
    int size = 0;
    int choice;
    clock_t start, end;
    char filename[100];

    printf("Программа сортировки массива методом двоичного дерева.\n");
    printf("Для завершения работы закройте окно.\n");

    do {
        showMenu();
        if (scanf("%d", &choice) != 1) {
            printf("Ошибка: введите число от 1 до 6!\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
        case 1: {
            printf("Введите имя файла (например, data.csv): ");
            scanf("%s", filename);
            size = readFromFile(A, 10000, filename);
            if (size > 0) {
                printf("Загружено %d элементов:\n", size);
                printArray(A, size < 20 ? size : 20);
                if (size > 20) printf("... (показано первые 20)\n");
            }
            else {
                printf("Не удалось загрузить данные из файла или файл пуст!\n");
            }
            break;
        }

        case 2: {
            int minVal, maxVal;
            int n;  // ВРЕМЕННАЯ ПЕРЕМЕННАЯ для размера

            printf("Введите минимальное значение: ");
            if (scanf("%d", &minVal) != 1) {
                printf("Ошибка: необходимо ввести целое число!\n");
                clearInputBuffer();
                break;
            }

            printf("Введите максимальное значение: ");
            if (scanf("%d", &maxVal) != 1) {
                printf("Ошибка: необходимо ввести целое число!\n");
                clearInputBuffer();
                break;
            }

            if (minVal > maxVal) {
                printf("Ошибка: минимальное значение не может быть больше максимального!\n");
                break;
            }

            printf("Введите количество элементов: ");
            if (scanf("%d", &n) != 1) {
                printf("Ошибка: необходимо ввести целое число!\n");
                clearInputBuffer();
                break;
            }

            // ПРОВЕРКИ с использованием n (НЕ size!)
            if (n > 10000) {
                printf("Максимальный размер: 10000!\n");
                break;
            }
            if (n <= 0) {
                printf("Размер должен быть больше 0!\n");
                break;
            }

            // Все проверки пройдены – присваиваем size
            size = n;

            generateArray(A, size, minVal, maxVal);
            printf("Сгенерирован массив из %d элементов в диапазоне [%d, %d]:\n", size, minVal, maxVal);
            printArray(A, size < 20 ? size : 20);
            if (size > 20) printf("... (показано первые 20)\n");
            break;
        }

        case 3: {
            if (size == 0) {
                printf("Нет данных для сортировки!\n");
                break;
            }
            for (int i = 0; i < size; i++) sorted[i] = A[i];

            printf("Выполняется сортировка...\n");
            start = clock();
            binaryTreeSort(sorted, size);
            end = clock();
            timeSec = (double)(end - start) / CLOCKS_PER_SEC;

            printf("Отсортированный массив:\n");
            printArray(sorted, size < 20 ? size : 20);
            if (size > 20) printf("... (показано первые 20)\n");

            for (int i = 0; i < size; i++) A[i] = sorted[i];
            break;
        }

        case 4: {
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
            if (size == 0) {
                printf("Нет данных для сравнения!\n");
                break;
            }
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

        printf("\n");

    } while (1);

    return 0;
}