#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <array>
#include <cmath>
#include <stdint.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>


#define N 202
#define K 202

struct RectanglePoints {
	double x_0, x_l;
	double y_0, y_m;
};

namespace InitialFunctions {
	inline double leftSideRectangle(double y) { return 15 * sqrt(y); }
	inline double rightSideRectangle(double y) { return 30 * y * (1 - y); }
	inline double upSideRectangle(double x) { return 15 * (1 - x); }
	inline double downSideRectangle(double x) { return 0; }

}

using namespace InitialFunctions;

class Grid {
private:

	RectanglePoints rectangle;

	double length_x;
	double length_y;
	double h_xyStep;
	
public:
	Grid(); //default constructor
	Grid(double l, double m, double h, std::array<double, N>& x, std::array<double, N>& y);

    void setPoints();
	void createGrid(std::array<double, N>& x, std::array<double, K>& y);
	void getGrid(std::array<double, N>& x, std::array<double, K>& y);
};

Grid::Grid()
{
	length_x = 1;
	length_y = 1;
	h_xyStep = 0.2;

	setPoints();
}

Grid::Grid(double l, double m, double h, std::array<double, N>& x, std::array<double, K>& y) {

	length_x = l;
	length_y = m;
	h_xyStep = h;

	setPoints();
	createGrid(x, y);
}

void Grid::setPoints()
{

	rectangle.x_0 = 0;
	rectangle.y_0 = 0;
	rectangle.x_l = rectangle.x_0 + length_x;
	rectangle.y_m = rectangle.y_0 + length_y;
}

void Grid::createGrid(std::array<double, N>& x, std::array<double, K>& y)
{
	x[0] = rectangle.x_0;
	y[0] = rectangle.y_0;
	
	for (uint8_t i = 0; i < N; ++i)
	{
		x[i] = i * h_xyStep;
	}

	for (uint8_t j = 0; j < K; ++j)
	{
		y[j] = j * h_xyStep;
	}
}

void Grid::getGrid(std::array<double, N>& x, std::array<double, K>& y)
{
	for (uint8_t i = 0; i < N; i++)
		printf("%lf ", x[i]);
	printf("\n\n");
	for (uint8_t j = 0; j < K; j++)
		printf("%lf ", y[j]);
	printf("\n\n");

}


class LaplasEquationSolver {
public:
	double w;
	LaplasEquationSolver(double(*u1)[N], std::array<double, N>& x, std::array<double, K>& y, double w);
};

LaplasEquationSolver::LaplasEquationSolver(double(*u1)[N], std::array<double, N>& x, std::array<double, K>& y, double w)
{
	double u2[N][K] = { 0 }, u_new[N][K] = { 0 };
	std::vector<double> errors;
	double max_error;
	double eps = 0.01;

	for (uint8_t i = 0; i < N; ++i)
		for (uint8_t j = 0; j < K; ++j) {
			u1[i][j] = 0;
			u2[i][j] = 0;
			u_new[i][j] = 0;
		}


	for (uint8_t i = 0; i < N; ++i)
	{
		u1[i][0] = downSideRectangle(x[i]);
		u1[i][K - 1] = upSideRectangle(x[i]);
	}

	for (uint8_t j = 0; j < K; ++j)
	{
		u1[0][j] = leftSideRectangle(y[j]);
		u1[N - 1][j] = rightSideRectangle(y[j]);
	}

	for (uint8_t i = 0; i < N; ++i)
	{
		u2[i][0] = downSideRectangle(x[i]);
		u2[i][K - 1] = upSideRectangle(x[i]);
	}

	for (uint8_t j = 0; j < K; ++j)
	{
		u2[0][j] = leftSideRectangle(y[j]);
		u2[N - 1][j] = rightSideRectangle(y[j]);
	}

	int k = 0;
	clock_t start_s=clock();
	do {
		for (uint8_t j = 1; j < K - 1; ++j) {
			for (uint8_t i = 1; i < N - 1; ++i) {
				u1[i][j] = (1.0 / 4.0) * (u1[i - 1][j] + u1[i + 1][j] + u1[i][j - 1] + u1[i][j + 1]);
			}
		}

		for (uint8_t j = 1; j < K - 1; ++j) {
			for (uint8_t i = 1; i < N - 1; ++i) {
				u2[i][j] = (1.0 / 4.0) * (u1[i - 1][j] + u1[i + 1][j] + u1[i][j - 1] + u1[i][j + 1]);
			}
		}

		for (uint8_t j = 0; j < K; ++j) {
			for (uint8_t i = 0; i < N; ++i) {
				u_new[i][j] = u1[i][j] + w * (u2[i][j] - u1[i][j]);
			}
		}

		for (uint8_t i = 0; i < N; ++i) {
			for (uint8_t j = 0; j < K; ++j) {
				errors.push_back(fabs(u_new[i][j] - u1[i][j]));;
			}
		}
		max_error = *std::max_element(errors.begin(), errors.end());
		//if(k%100==0)std::cout<<k<<" "<<max_error<<"\n";
		errors.clear();
		for (uint8_t i = 0; i < N; ++i) {
			for (uint8_t j = 0; j < K; ++j) {
				u1[i][j] = u_new[i][j];
			}
		}
		k++;
	} while (max_error > eps);
	clock_t end_s=clock();

	std::cout << "Input = "<<N<<" "<<K <<std::endl;
	std::cout << "w= " << w << ", iteration= " << k << ", max error = " << max_error << std::endl;
	std::cout << "cost time = "<< end_s - start_s << " clocks"<< std::endl;
}




int main(void) 
{
	std::array<double, N> x = { 0 };
	std::array<double, K> y = { 0 };
	double u[N][K] = { 0 };

	/*calling 2nd constructor*/
	Grid grid(1, 1, 0.2, x, y); 

	/*print grid to console*/
	//grid.getGrid(x, y);

	double w = 1.9;
	LaplasEquationSolver mySolver(u, x, y, w);



	return 0;
}
