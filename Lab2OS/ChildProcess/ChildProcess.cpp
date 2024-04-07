#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream> 

void shellSort(std::vector<double>& arr) {
    int n = arr.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            double temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

int main(int argc, wchar_t* argv[]) {
    if (argc < 2) {
        std::cerr << "not enough arguments" << std::endl;
        return 1;
    }

    int arraySize = std::stoi(argv[1]);
    std::vector<double> input(arraySize);
    for (size_t i = 0; i < arraySize; i++) {
        std::wistringstream iss(argv[i + 2]);
        if (!(iss >> input[i])) {
            std::cerr << "Invalid number: " << argv[i + 2] << std::endl;
            return 1;
        }
    }

    shellSort(input); 

    std::cout << "Sorted array: ";
    for (int i = 0; i < arraySize; ++i)
        std::cout << input[i] << " ";
    std::cout << std::endl;

    Sleep(8000);

    return 0;
}