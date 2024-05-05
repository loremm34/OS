#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <random>
#include <future>

std::mutex mtx1;
std::mutex mtx2;
std::condition_variable_any cv;
std::vector<char> array;
int k = 0;
double result = 0;
bool workFinished = false;

void workThread(std::promise<void>& promiseObj) {
    std::unique_lock<std::mutex> lock(mtx1);
    std::cout << "Enter interval\n";
    int restInterval;
    std::cin >> restInterval;

    for (size_t i = 0; i < array.size(); ++i) {
        if (std::isdigit(array[i])) {
            std::cout << "Processing digit: " << array[i] << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(restInterval));
        }
        else {
            std::cout << "Skipping non-digit: " << array[i] << std::endl;
        }
    }

    std::cout << "Work thread has finished." << std::endl;
    workFinished = true;
    cv.notify_one();
    promiseObj.set_value();
}

double sumElementThread() {
    std::unique_lock<std::mutex> lock(mtx2);
    cv.wait(lock, [] { return workFinished; });

    std::cout << "SumElement thread is waiting to start summing." << std::endl;

    int sum = 0;
    int count = 0;
    for (size_t i = 0; i < k && i < array.size(); ++i) {
        if (std::isdigit(array[i])) {
            sum += (array[i] - '0');
            ++count;
        }
    }
    return static_cast<double>(sum) / count;
}

int main() {
    std::cout << "Input array size\n";
    int arraySize;
    std::cin >> arraySize;

    std::cout << "Input or random numbers\n";
    std::string arrayType;
    std::cin >> arrayType;

    if (arrayType == "input") {
        for (int i = 0; i < arraySize; ++i) {
            std::cout << "Input numbers";
            char element;
            std::cin >> element;
            array.push_back(element);
        }
    }
    else if (arrayType == "random") {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 9);

        for (int i = 0; i < arraySize; ++i) {
            array.push_back(dis(gen) + '0');
        }
    }

    for (char c : array) {
        std::cout << c << " ";
    }
    std::cout << std::endl;

    std::promise<void> promiseObj;
    std::future<void> futureObj = promiseObj.get_future();
    std::thread workThreadObj(workThread, std::ref(promiseObj));

    std::cout << "Enter k\n";
    std::cin >> k;

    double result = sumElementThread();

    futureObj.get(); 
    workThreadObj.join();

    std::cout << "Final result: " << result << std::endl;

    return 0;
}
