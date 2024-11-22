#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <limits>
#include <string>
#include <cmath>
#include <random>
#include <ctime>
#include <sstream>

using namespace std;
// Задание 1
// Метод для заполнения бинарного файла случайными числами
void fillBinaryFile(const string &filename, int numElements, int lowerBound, int upperBound) {
    ofstream outFile(filename, ios::binary);
    
    if (!outFile) {
        cerr << "Error when opening a file for writing.\n";
        return;
    }

    // Инициализируем генератор случайных чисел
    srand(static_cast<unsigned int>(time(0)));

    for (int i = 0; i < numElements; ++i) {
        int randomNumber = lowerBound + rand() % (upperBound - lowerBound + 1);
        outFile.write(reinterpret_cast<const char*>(&randomNumber), sizeof(int));
    }

    outFile.close();
}
// Метод для извлечения четных чисел из бинарного файла
void extractEvenNumbers(const string &inputFile, const string &outputFile) {
    ifstream inFile(inputFile, ios::binary);
    ofstream outFile(outputFile, ios::binary);

    if (!inFile) {
        cerr << "Error when opening a file for reading.\n";
        return;
    }
    if (!outFile) {
        cerr << "Error when opening a file for reading.\n";
        return;
    }

    int number;
    while (inFile.read(reinterpret_cast<char*>(&number), sizeof(int))) {
        if (number % 2 == 0) {  // Если число четное
            outFile.write(reinterpret_cast<const char*>(&number), sizeof(int));
        }
    }

    inFile.close();
    outFile.close();
}

// Задание 2
// Функция для заполнения матрицы элементами из бинарного файла
void fillMatrixFromFile(const string &filename, vector<vector<int>> &matrix, int n) {
    ifstream inFile(filename, ios::binary);
    
    if (!inFile) {
        cerr << "Error when opening a file for reading.\n";
        return;
    }

    int totalElements = n * n;
    vector<int> data(totalElements, 0);
    inFile.read(reinterpret_cast<char*>(&data[0]), totalElements * sizeof(int));

    int index = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (index < data.size()) {
                matrix[i][j] = data[index++];
            } else {
                matrix[i][j] = 0;
            }
        }
    }
    inFile.close();
}
// Функция для замены местами минимального и максимального элементов в строке
void swapMinMaxInRow(vector<int> &row) {
    int minIndex = 0, maxIndex = 0;
    int minValue = row[0], maxValue = row[0];
    
    for (int i = 1; i < row.size(); ++i) {
        if (row[i] < minValue) {
            minValue = row[i];
            minIndex = i;
        }
        if (row[i] > maxValue) {
            maxValue = row[i];
            maxIndex = i;
        }
    }

    if (minIndex != maxIndex) {
        swap(row[minIndex], row[maxIndex]);
    }
}
void processMatrix(vector<vector<int>> &matrix, int n) {
    for (int i = 0; i < n; ++i) {
        swapMinMaxInRow(matrix[i]);
    }
}
void writeMatrixToFile(const string &filename, const vector<vector<int>> &matrix, int n) {
    ofstream outFile(filename, ios::binary);
    
    if (!outFile) {
        cerr << "Error when opening a file for writing.\n";
        return;
    }

    for (int i = 0; i < n; ++i) {
        outFile.write(reinterpret_cast<const char*>(&matrix[i][0]), n * sizeof(int));
    }

    outFile.close();
}

// Задание 3 
struct Luggage 
{
    string name;   
    float weight;
};
// Функция для записи данных о багаже в бинарный файл
void writeToFile(const vector<vector<Luggage>>& allLuggage, const string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) 
    {
        cerr << "Error when opening a file for writing.\n";
        return;
    }

    // Запись данных о каждом пассажире (в виде списка багажа)
    for (const auto& luggageList : allLuggage) {
        size_t numBags = luggageList.size();
        outFile.write(reinterpret_cast<const char*>(&numBags), sizeof(numBags));
        for (const auto& luggage : luggageList) {
            size_t nameLength = luggage.name.size();
            outFile.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
            outFile.write(luggage.name.c_str(), nameLength);
            outFile.write(reinterpret_cast<const char*>(&luggage.weight), sizeof(luggage.weight));
        }
    }
}
// Функция для чтения данных о багаже из бинарного файла
vector<vector<Luggage>> readFromFile(const string& filename) {
    ifstream inFile(filename, ios::binary);
    vector<vector<Luggage>> allLuggage;

    if (!inFile) 
    {
        cerr << "Error when opening a file for reading.\n";
        return allLuggage;
    }

    while (inFile) 
    {
        size_t numBags;
        inFile.read(reinterpret_cast<char*>(&numBags), sizeof(numBags));
        if (inFile.eof()) break;
        vector<Luggage> luggageList(numBags);
        for (size_t i = 0; i < numBags; ++i) 
        {
            size_t nameLength;
            inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
            luggageList[i].name.resize(nameLength);
            inFile.read(&luggageList[i].name[0], nameLength);
            inFile.read(reinterpret_cast<char*>(&luggageList[i].weight), sizeof(luggageList[i].weight));
        }
        allLuggage.push_back(luggageList);
    }
    return allLuggage;
}
// Функция для вычисления средней массы багажа для списка багажа
float getAverageWeight(const vector<Luggage>& luggageList) 
{
    float totalWeight = 0.0f;
    for (const auto& luggage : luggageList) 
    {
        totalWeight += luggage.weight;
    }
    return (luggageList.empty()) ? 0.0f : totalWeight / luggageList.size();
}
// Функция для вычисления общей средней массы багажа
float getOverallAverageWeight(const vector<vector<Luggage>>& allLuggage) {
    float totalWeight = 0.0f;
    size_t totalBags = 0;
    for (const auto& luggageList : allLuggage) {
        for (const auto& luggage : luggageList) {
            totalWeight += luggage.weight;
        }
        totalBags += luggageList.size();
    }
    return (totalBags > 0) ? totalWeight / totalBags : 0.0f;
}
// Функция для поиска пассажиров, чья средняя масса багажа отличается не более чем на m кг от общей средней массы
vector<vector<Luggage>> findLuggageWithAverageWeightDifference(const vector<vector<Luggage>>& allLuggage, float m) 
{
    float overallAverage = getOverallAverageWeight(allLuggage);
    vector<vector<Luggage>> result;
    for (const auto& luggageList : allLuggage) {
        float avgWeight = getAverageWeight(luggageList);
        if (fabs(avgWeight - overallAverage) <= m) 
        {
            result.push_back(luggageList);
        }
    }
    return result;
}

// Задание 4
// Функция для заполнения файла случайными числами
void fillFileWithRandomNumbers1(const string& filename, size_t n)
{
    ofstream outFile(filename);
    if (!outFile) 
    {
        cerr << "Error when opening a file for writing.\n";
        return;
    }
    // Инициализация генератора случайных чисел
    random_device rd;  
    mt19937 gen(rd());
    uniform_int_distribution<> dist(-100, 100); // Диапазон от -100 до 100

    for (size_t i = 0; i < n; ++i) 
    {
        int randomNumber = dist(gen);
        outFile << randomNumber << endl;
    }
    outFile.close();
}
// Функция для вычисления разности суммы элементов первой и второй половины файла
int calculateDifference(const string& filename) 
{
    ifstream inFile(filename);
    if (!inFile) 
    {
        cerr << "Error when opening a file for reading.\n";
        return 0;
    }
    vector<int> numbers;
    int number;
    while (inFile >> number) 
    {
        numbers.push_back(number);
    }

    size_t n = numbers.size();
    if (n % 2 != 0) 
    {
        cerr << "The number of elements in the file must be even.\n";
        return 0;
    }

    int sumFirstHalf = 0;
    for (size_t i = 0; i < n / 2; ++i) {
        sumFirstHalf += numbers[i];
    }

    int sumSecondHalf = 0;
    for (size_t i = n / 2; i < n; ++i) {
        sumSecondHalf += numbers[i];
    }

    return sumFirstHalf - sumSecondHalf;
}

// Задание 5
void fillFileWithRandomNumbers2(const string& filename, size_t rows, size_t numbersPerRow) 
{
    ofstream outFile(filename);
    if (!outFile) 
    {
        cerr << "Error when opening a file for writing.\n";
        return;
    }
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(-100, 100);

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < numbersPerRow; ++j) {
            int randomNumber = dist(gen);
            outFile << randomNumber;
            if (j < numbersPerRow - 1) 
            {
                outFile << " ";
            }
        }
        outFile << endl;
    }

    outFile.close();
}
// Функция для вычисления суммы всех элементов в файле
int calculateSum(const string& filename) 
{
    ifstream inFile(filename);
    if (!inFile) 
    {
        cerr << "Error when opening a file for reading.\n";
        return 0;
    }
    int totalSum = 0;
    string line;
    while (getline(inFile, line)) 
    {
        stringstream ss(line);
        int number;
        while (ss >> number) 
        {
            totalSum += number;
        }
    }
    inFile.close();
    return totalSum;
}

//Задание 6
// Функция для поиска самой короткой и самой длинной строки в файле
void findAndWriteLongestAndShortest(const string& inputFilename, const string& outputFilename) 
{
    ifstream inFile(inputFilename);
    if (!inFile) 
    {
        cerr << "Error when opening input file.\n";
        return;
    }

    string line;
    string shortestLine;
    string longestLine;
    bool isFirstLine = true;

    // Чтение строк и нахождение самой короткой и самой длинной
    while (getline(inFile, line)) 
    {
        if (isFirstLine) 
        {
            shortestLine = line;
            longestLine = line;
            isFirstLine = false;
        } 
        else 
        {
            if (line.length() < shortestLine.length()) 
            {
                shortestLine = line; 
            }
            if (line.length() > longestLine.length()) 
            {
                longestLine = line; 
            }
        }
    }

    inFile.close();

    ofstream outFile(outputFilename);
    if (!outFile) {
        cerr << "Error when opening the output file.\n";
        return;
    }

    outFile << "The shortest line: " << shortestLine << "\n";
    outFile << "Longest line: " << longestLine << "\n";

    outFile.close();
}

int main() 
{
    //Значения для рандома
    int numElements = 100;  
    int lowerBound = 1;  
    int upperBound = 1000;  
    //Task 1
    cout << "\nTask 1\n";
    string inputFile1 = "input1.bin";
    string outputFile1 = "output1.bin";
    fillBinaryFile(inputFile1, numElements, lowerBound, upperBound);
    cout << "The source file 'input1.bin' is filled with random numbers.\n";
    extractEvenNumbers(inputFile1, outputFile1);
    cout << "Even numbers are saved to the file 'output1.bin'.\n";

    //Task 2
    cout << "\nTask 2\n";
    string inputFile2 = "input2.bin"; 
    string outputFile2 = "output2.bin"; 
    int n = 11;
    fillBinaryFile(inputFile2, numElements, lowerBound, upperBound);
    cout << "The source file 'input2.bin' is filled with random numbers.\n";  
    vector<vector<int>> matrix(n, vector<int>(n, 0));
    fillMatrixFromFile(inputFile2, matrix, n);
    processMatrix(matrix, n);
    writeMatrixToFile(outputFile2, matrix, n);
    cout << "Even numbers are saved to the file output2.bin\n";
    
    //Task 3
    cout << "\nTask 3\n";
    vector<vector<Luggage>> allLuggage = {
        {{"suitcase", 15.5f}, {"bag", 12.0f}},       
        {{"suitcase", 20.0f}, {"box", 4.0f}},      
        {{"bag", 8.5f}, {"suitcase",20.0f}},   
    };
    writeToFile(allLuggage, "luggage.bin");
    auto luggageFromFile = readFromFile("luggage.bin");
    float m = 1.0f;
    auto result = findLuggageWithAverageWeightDifference(luggageFromFile, m); 
    for (size_t i = 0; i < result.size(); ++i) 
    {
        float avgWeight = getAverageWeight(result[i]);
        cout << "Passenger " << i + 1 << " fits (average luggage weight: " << avgWeight << " kg)\n";
    }

    //Task 4
    cout << "\nTask 4\n";
    string filename_t4 = "numbers1.txt";
    size_t N = 4;
    fillFileWithRandomNumbers1(filename_t4, N);
    int difference = calculateDifference(filename_t4);
    cout << "The difference of the sum of the first and second halves: " << difference << "\n";

    //Task 5
    cout << "\nTask 5\n";
    string filename_t5 = "numbers2.txt";
    // Количество строк и количество чисел в каждой строке
    size_t rows = 3;         
    size_t numbersPerRow = 2;
    fillFileWithRandomNumbers2(filename_t5, rows, numbersPerRow);
    int totalSum = calculateSum(filename_t5);
    cout << "The sum of all elements in the file: " << totalSum << "\n";

    //Task 6
    cout << "\nTask 6\n";
    string inputFilename = "input_t6.txt";
    string outputFilename = "output_t6.txt";
    findAndWriteLongestAndShortest(inputFilename, outputFilename);
    cout << "The results are recorded in a file " << outputFilename << "\n";
    return 0;
}
