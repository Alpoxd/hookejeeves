#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

float Rosenbrok_function(std::vector <float> X0) {
	return (100 * pow(X0[1] - pow(X0[0], 2), 2) + pow(1 - X0[0], 2));
}

float example_function(std::vector <float> X0) {
	return (3 * pow(X0[0], 4) - 6 * pow(X0[0], 2) + 4 * X0[0] * X0[1] + pow(X0[1], 2));
}

float manage_function(int functionId, std::vector <float> X0) {
	switch (functionId) {
	case 0:
		return Rosenbrok_function(X0);
		break;
	case 1:
		return example_function(X0);
		break;
	}
}

void print_function(int functionId) {
	switch (functionId) {
	case 0:
		std::cout << "\nRosenbrok Function: Q(x1,x2) = 100 * (x2 - x1^2)^2 + (1 - x1)^2 -> min \n" << std::endl;
		break;
	case 1:
		std::cout << "\nExample Function: Q(x1,x2) -> min \n" << std::endl;
		break;
	}
}

std::vector <float> neighborhood_search(std::vector <float> X0, float delta, int functionId) {
	std::vector <float> point_x2_fixed = { X0[0] + delta, X0[1] };
	std::vector <float> newPoint;

	if (manage_function(functionId, point_x2_fixed) < manage_function(functionId, X0))
		newPoint.push_back(point_x2_fixed[0]);
	else {
		point_x2_fixed[0] = X0[0] - delta;
		if (manage_function(functionId, point_x2_fixed) < manage_function(functionId, X0))
			newPoint.push_back(point_x2_fixed[0]);
		else
			newPoint.push_back(X0[0]);
	}

	std::vector <float> point_x1_fixed = { newPoint[0], X0[1] + delta };

	if (manage_function(functionId, point_x1_fixed) < manage_function(functionId, X0)) {
		newPoint.push_back(point_x1_fixed[1]);
	}
	else {
		point_x1_fixed[1] = X0[1] - delta;
		if (manage_function(functionId, point_x1_fixed) < manage_function(functionId, X0))
			newPoint.push_back(point_x1_fixed[1]);
		else
			newPoint.push_back(X0[1]);
	}

	return newPoint;
}

void HookeJeevesAlgorythm(std::vector <float> X0, float delta, int step_reduction_coef, float end_point_param, int functionId) {
	print_function(functionId);
	std::vector <float> X1 = X0;
	std::vector <float> X2;
	std::vector <std::vector<float>> X_array;
	X_array.push_back(X0);
	bool stopPoint = false;
	bool nextStepFlag = true;

	std::cout << "starting point: X = (" << X0[0] << ", " << X0[1] << ") Q(X) = " << manage_function(functionId, X0) << std::endl;
	int counter = 0;

	while (delta > end_point_param) {
		X1 = neighborhood_search(X1, delta, functionId);
		if (X1[0] == X0[0] && X1[1] == X0[1]) {
			delta = delta / step_reduction_coef;
		}
		else if (find(X_array.begin(), X_array.end(), X1) != X_array.end()) {
			delta = delta / step_reduction_coef;
		}
		else {
			X_array.push_back(X1);
			if (counter == 0) {
				counter++;
			}

			X2 = { X_array[X_array.size() - 1][0] + X_array[X_array.size() - 1][0] - X_array[X_array.size() - 2][0],
				X_array[X_array.size() - 1][1] + X_array[X_array.size() - 1][1] - X_array[X_array.size() - 2][1] };

			X_array.push_back(X2);
			X0 = X1;
			X1 = X2;
			counter++;
		}
		//std::cout << X0[0] << " " << X0[1] << std::endl;
	}
	std::cout << "\nresult:\nX = (" << X1[0] << ", " << X1[1] << ")\nQ(X) = " << manage_function(functionId, X1) << "\ndelta = " << delta << "\niterations:  " << counter - 1 << "\n\n" << std::endl;
}

void main(void) {
	std::vector <float> X0 = { -1, 1 };

	float delta = 1;
	int step_reduction_coef = 2;
	float end_point_param = 10e-7;
	int functionId = 0;

	HookeJeevesAlgorythm(X0, delta, step_reduction_coef, end_point_param, functionId);

	X0 = { -2, 2 };
	functionId = 1;

	HookeJeevesAlgorythm(X0, delta, step_reduction_coef, end_point_param, functionId);
}