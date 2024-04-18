#include <iostream>
#include <random>
#include <cmath>

static double function(double x, double y) {
    //return sin(x) * cos(y);
    //if (x / y != -INFINITY) {
    //    if (y > 0) return x / y;
    //    if (y < 0) return x / y + abs(x);
    //}
    //return INFINITY;
    return 100 * pow((y - pow(x, 2)), 2) + pow((1 - x), 2);
}

void main(void) {
    std::pair<double, double> dx(-10.0, 10.0), dy(-10.0, 10.0);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-10.0, 10.0); // Диапазон для случайных точек

    std::pair<double, double> currentPoint = { dis(gen), dis(gen) };
    auto currentValue = function(currentPoint.first, currentPoint.second);
    auto minValue = currentValue;

    auto iterations = 10000000; // Количество итераций
    auto searchRadius = 5.0; // Исходный радиус поиска
    auto reductionFactor = 0.99; // Фактор уменьшения радиуса поиска

#pragma omp parallel for num_threads(20)
    for (auto i = 0; i < iterations; ++i) {

        std::pair<double, double> newPoint = { currentPoint.first + dis(gen) * searchRadius, currentPoint.second + dis(gen) * searchRadius };

        if (newPoint.first >= dx.first and newPoint.first <= dy.second and newPoint.second >= dy.first and newPoint.second <= dy.second) {

            auto newValue = function(newPoint.first, newPoint.second);
            if (newValue < minValue) {
                minValue = newValue;
                currentPoint = newPoint;
            }
            searchRadius *= reductionFactor; // Уменьшаем радиус поиска
        }
    }

    std::cout << "Minimum in point (" << currentPoint.first << ", " << currentPoint.second << ") "
        << "with value: " << minValue << std::endl;
}
