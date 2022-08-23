//	Elena Rangelov
//	31.1.2022

#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <string>
#include <ctime>
#include <math.h>
#include <iomanip>
#include <array>
#include <algorithm>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <cstdlib>
#include <stack>
#include <cmath>
#include <queue>

using namespace std;

string ifilename = "image.ppm";
const string ofilename = "imageg.ppm";
const string ofilename2 = "image2.ppm";
const string ofilename3 = "image3.ppm";
const string ofilenamef = "imagef.ppm";
const string ofilenamev = "imagev.ppm";

int WIDTH, HEIGHT;

// ---------------------------------------------------- PART 1 -----------------------------------------------

void drawArc(int cx, int cy, int y, int x, vector<vector<string>> image, string color) {

	const string black = color;
	const string white = "1 1 1 ";

	int cyy1 = cy - y;
	int cyy2 = cy + y;
	int cxx1 = cx - x;
	int cxx2 = cx + x;
	int cyx1 = cy - x;
	int cyx2 = cy + x;
	int cxy1 = cx - y;
	int cxy2 = cx + y;

	if (cyy1 < 0)
		cyy1 = 0;
	if (cxx1 < 0)
		cxx1 = 0;
	if (cyx1 < 0)
		cyx1 = 0;
	if (cxy1 < 0)
		cxy1 = 0;

	if (cxx2 >= WIDTH)
		cxx2 = 0;
	if (cyy2 >= HEIGHT)
		cyy2 = 0;
	if (cxy2 >= WIDTH)
		cxy2 = 0;
	if (cyx2 >= HEIGHT)
		cyx2 = 0;


	image[cxx1][cyy1] = black;
	image[cxx1][cyy2] = black;
	image[cxx2][cyy1] = black;
	image[cxx2][cyy2] = black;
	image[cxy1][cyx1] = black;
	image[cxy1][cyx2] = black;
	image[cxy2][cyx1] = black;
	image[cxy2][cyx2] = black;
}

void drawCircle(int r, int center[], vector<vector<string>>& image, string color) {

	int x, y, xmax, y2, y2_new, ty, cy, cx;
	xmax = (int)(r * 0.70710678) + 1; // maximum x at radius/sqrt(2)
	cy = center[1];
	cx = center[0];
	y = r;
	x = 0;
	y2 = y * y;
	ty = (2 * y) - 1;
	y2_new = y2;


	for (x = 0; x <= xmax; x++) {
		if ((y2 - y2_new) >= ty) {
			y2 -= ty;
			y -= 1;
			ty -= 2;
		}
		drawArc(cx, cy, y, x, image, color);
		y2_new -= (2 * x) - 3;


	}
}

class Point {
private:
	double x;
	double y;

public:
	Point() {
		x = 0;
		y = 0;
	}

	Point(double x_input, double y_input) {
		x = x_input;
		y = y_input;
	}

	void draw(vector<vector<string>>& image, string color) {

		int x1 = x;// *WIDTH;
		int y1 = y;// *HEIGHT;
		int center[] = { x1, y1 };

		if (0 <= x1 && x < WIDTH && 0 <= y && y < HEIGHT) {
			image[x1][y1] = color;
			drawCircle(2, center, image, color);
		}
	}

	double get_x() {
		return x;
	}

	double get_y() {
		return y;
	}

	void set_x(double xx) {
		x = xx;
	}

	void set_y(double yy) {
		y = yy;
	}

	bool equals(Point p) {
		return (x == p.get_x() && y == p.get_y());
	}

};

class Line {
private:
	Point a;
	Point b;
	double m;
	Point inter;

public:
	Line() {
		a = Point();
		b = Point();
		m = 0.0;
		inter = Point();
	}

	Line(Point a_input, Point b_input) {
		a = a_input;
		b = b_input;
		m = (b.get_y() - a.get_y()) / (b.get_x() - a.get_x());
		inter.set_y(a.get_y() - a.get_x() * m);
		inter.set_x(0);
	}

	Line(Point a_input, double m_input) {

		m = m_input; //* ( WIDTH / double( HEIGHT ) );

		//cout << setprecision(10) << " " << m_input << " " << a_input.get_x() << " " << a_input.get_y() << endl;

		//cout << m << endl;

		a = a_input;
		//b = Point(a.get_x() + 100 * cos(dir), a.get_y() + 100 * sin(dir));
		//m = (b.get_y() - a.get_y()) / (b.get_x() - a.get_x());
		inter.set_y(a.get_y() - a.get_x() * m);
		inter.set_x(0);

		//cout << "all " << a.get_x() << " " << a.get_y() << "    " << b.get_x() << " " << b.get_y() << endl;


		double x_0[] = { 0, inter.get_y() };
		double y_0[] = { -inter.get_y() / m, 0 };
		double x_1[] = { 0.99999, 0.999999 * m + inter.get_y() };
		double y_1[] = { (0.999999 - inter.get_y()) / m, 0.9999999 };

		double point_a[2] = { -1, -1 };
		double point_b[2] = { -1, -1 };

		if (0.0 <= x_0[1] && x_0[1] < 1.0) {

			point_a[0] = x_0[0];
			point_a[1] = x_0[1];
		}
		if (0.0 <= y_0[0] && y_0[0] < 1.0) {
			if (point_a[0] == -1) {
				point_a[0] = y_0[0];
				point_a[1] = y_0[1];
			}
			else {
				point_b[0] = y_0[0];
				point_b[1] = y_0[1];
			}
		}
		if (0.0 <= x_1[1] && x_1[1] < 1.0) {
			if (point_a[0] == -1) {
				point_a[0] = x_1[0];
				point_a[1] = x_1[1];
			}
			else {
				point_b[0] = x_1[0];
				point_b[1] = x_1[1];
			}
		}
		if (0.0 <= y_1[0] && y_1[0] < 1.0) {
			if (point_a[0] == -1) {
				point_a[0] = y_1[0];
				point_a[1] = y_1[1];
			}
			else {
				point_b[0] = y_1[0];
				point_b[1] = y_1[1];
			}
		}

		a.set_x(point_a[0]);
		a.set_y(point_a[1]);
		b.set_x(point_b[0]);
		b.set_y(point_b[1]);
		
	}

	void make_line() {

		double x_0[] = { 0, inter.get_y() };
		double y_0[] = { -inter.get_y() / m, 0 };
		double x_1[] = { 0.99999, 0.999999 * m + inter.get_y() };
		double y_1[] = { (0.999999 - inter.get_y()) / m, 0.9999999 };

		double point_a[2] = { -1, -1 };
		double point_b[2] = { -1, -1 };

		if (0.0 <= x_0[1] && x_0[1] < 1.0) {

			point_a[0] = x_0[0];
			point_a[1] = x_0[1];
		}
		if (0.0 <= y_0[0] && y_0[0] < 1.0) {
			if (point_a[0] == -1) {
				point_a[0] = y_0[0];
				point_a[1] = y_0[1];
			}
			else {
				point_b[0] = y_0[0];
				point_b[1] = y_0[1];
			}
		}
		if (0.0 <= x_1[1] && x_1[1] < 1.0) {
			if (point_a[0] == -1) {
				point_a[0] = x_1[0];
				point_a[1] = x_1[1];
			}
			else {
				point_b[0] = x_1[0];
				point_b[1] = x_1[1];
			}
		}
		if (0.0 <= y_1[0] && y_1[0] < 1.0) {
			if (point_a[0] == -1) {
				point_a[0] = y_1[0];
				point_a[1] = y_1[1];
			}
			else {
				point_b[0] = y_1[0];
				point_b[1] = y_1[1];
			}
		}

		a.set_x(point_a[0]);
		a.set_y(point_a[1]);
		b.set_x(point_b[0]);
		b.set_y(point_b[1]);
	}

	Point get_a() {
		return a;
	}

	Point get_b() {
		return b;
	}

	double segment_length() {
		return sqrt(pow((a.get_x() - b.get_x()), 2) + pow((a.get_y() - b.get_y()), 2));
	}

	Line find_perpendicular(Point point) {
		double perp_m = -1 / m;
		double perp_inter = point.get_y() - perp_m * point.get_x();

		Point point_a = { 0, perp_inter };
		Point point_b = point;

		Line perp = Line(point_a, point_b);
		return perp;
	}

	Point find_intersection(Line line) {
		Point intersection;

		intersection.set_x((line.inter.get_y() - inter.get_y()) / (m - line.m));
		intersection.set_y(intersection.get_x() * m + inter.get_y());

		return intersection;

	}

	Point find_E(Point point, double distance) {

		double change_x = distance / (sqrt(pow(m, 2) + 1));
		double change_y = m * change_x;

		Point ret = Point(point.get_x() + change_x, point.get_y() + change_y);
		return ret;
	}

	void vote_line(vector<vector<int>>& votes) {

		int x1 = a.get_x() * WIDTH;
		int x2 = b.get_x() * WIDTH;
		int y1 = a.get_y() * HEIGHT;
		int y2 = b.get_y() * HEIGHT;

		//cout << x1 << " " << y1 << " " << x2 << " " << y2 << endl;

		//cout << m << endl;

		int dx = abs(x2 - x1);
		int dy = abs(y2 - y1);

		int i = x1;
		int j = y1;
		int e = dy - dx;

		// down right

		if (x2 - x1 > 0 && y2 - y1 > 0) {

			if (e < 0)
				//shallow
				for (i; i < x2; i++) {
					votes[i][j]++;
					if (e >= 0) {
						j += 1;
						if (j == HEIGHT)
							break;
						e -= dx;
					}
					e += dy;
				}
			else {
				//steep
				for (j; j < y2; j++) {
					votes[i][j]++;
					if (e >= 0) {
						i += 1;
						if (i == WIDTH)
							break;
						e -= dy;
					}
					e += dx;
				}
			}
		}

		// down left

		if (x2 - x1 < 0 && y2 - y1 > 0) {

			if (e < 0)
				//shallow
				for (i; i > x2; i--) {
					votes[i][j]++;
					if (e >= 0) {
						j += 1;
						if (j == HEIGHT)
							break;
						e -= dx;
					}
					e += dy;
				}
			else {
				//steep
				for (j; j < y2; j++) {
					votes[i][j]++;
					if (e >= 0) {
						i -= 1;
						if (i == -1)
							break;
						e -= dy;
					}
					e += dx;
				}
			}
		}

		// up right

		if (x2 - x1 > 0 && y2 - y1 < 0) {

			if (e < 0)
				//shallow
				for (i; i < x2; i++) {
					votes[i][j]++;
					if (e >= 0) {
						j -= 1;
						if (j == -1)
							break;
						e -= dx;
					}
					e += dy;
				}
			else {
				//steep
				for (j; j > y2; j--) {
					votes[i][j]++;
					if (e >= 0) {
						i += 1;
						if (i == WIDTH)
							break;
						e -= dy;
					}
					e += dx;
				}
			}
		}

		// up left

		if (x2 - x1 < 0 && y2 - y1 < 0) {

			if (e < 0)
				//shallow
				for (i; i > x2; i--) {
					votes[i][j]++;
					if (e >= 0) {
						j -= 1;
						if (j == -1)
							break;
						e -= dx;
					}
					e += dy;
				}
			else {
				//steep
				for (j; j > y2; j--) {
					votes[i][j]++;
					if (e >= 0) {
						i -= 1;
						if (i == -1)
							break;
						e -= dy;
					}
					e += dx;
				}
			}
		}

	}

	void draw1(vector<vector<string>>& image, string color) {

		const string black = color;
		const string white = "1 1 1 ";

		int axSIZE = a.get_x();
		int aySIZE = a.get_y();
		int bxSIZE = b.get_x();
		int bySIZE = b.get_y();

		int dx = bxSIZE - axSIZE;
		int dy = bySIZE - aySIZE;
		int y = aySIZE;
		int x = axSIZE;
		int d = abs(abs(dy) - abs(dx));

		string dir = "";

		if (dx == 0 && dy == 0)
			return;
		if (dx == 0)
			dir = "y";
		if (dy == 0)
			dir = "x";
		if (abs(dx) > abs(dy))
			dir = "s";
		if (dx > 0)
			dir += "r";
		else dir += "l";
		if (dy > 0)
			dir += "d";
		else dir += "u";

		dx = abs(dx);
		dy = abs(dy);


		if (dir.find('x') != string::npos)
			if (dir.find('r') != string::npos)
				for (int x = a.get_x(); x <= b.get_x(); x++)
					image[x][y] = black;
			else
				for (int x = a.get_x(); x >= b.get_x(); x--)
					image[x][y] = black;
		else if (dir.find('y') != string::npos)
			if (dir.find('d') != string::npos)
				for (int y = a.get_y(); y <= b.get_y(); y++)
					image[x][y] = black;
			else
				for (int y = a.get_y(); y >= b.get_y(); y--)
					image[x][y] = black;
		else
			if (dir.find('s') == string::npos)
				if (dir.find('d') != string::npos)
					for (int y = aySIZE; y < bySIZE; y++)
					{
						image[x][y] = black;
						if (d >= 0)
						{
							if (dir.find('r') != string::npos)
								x += 1;
							else x -= 1;
							d -= dy;
						}
						d += dx;
					}
				else
					for (y = aySIZE; y > bySIZE; y--)
					{
							image[x][y] = black;
							if (d <= 0)
							{
								if (dir.find('r') != string::npos)
									x += 1;
								else x -= 1;
								d -= dy;
							}
							d += dx;
						
					}
			else
				if (dir.find('r') != string::npos)
					for (x = axSIZE; x < bxSIZE; x++)
					{
							image[x][y] = black;
							if (d >= 0)
							{
								if (dir.find('d') != string::npos)
									y += 1;
								else y -= 1;
								d -= dx;
							}
							d += dy;
						
					}
				else
					for (x = axSIZE; x > bxSIZE; x--)
					{
						image[x][y] = black;
						if (d <= 0)
						{
							if (dir.find('d') != string::npos)
								y += 1;
							else y -= 1;
							d -= dx;
						}
						d += dy;
					}

	}

	void draw2(vector<vector<string>>& image, string color) {
		
			//calculating range for line between start and end point
			int dx = b.get_x() - a.get_x();
			int dy = b.get_y() - a.get_y();

			int x = a.get_x();
			int y = a.get_y();

			//this is the case when slope(m) < 1
			if (abs(dx) > abs(dy))
			{
				image[x][y] = color;
				int pk = (2 * abs(dy)) - abs(dx);

				for (int i = 0; i < abs(dx); i++)
				{
					x = x + 1;
					if (pk < 0)
						pk = pk + (2 * abs(dy));
					else
					{
						y = y + 1;
						pk = pk + (2 * abs(dy)) - (2 * abs(dx));
					}
					image[x][y] = color;
				}
			}
			else
			{
				//this is the case when slope is greater than or equal to 1  i.e: m>=1
				image[x][y] = color;
				int pk = (2 * abs(dx)) - abs(dy);

				for (int i = 0; i < abs(dy); i++)
				{
					y = y + 1;
					if (pk < 0)
						pk = pk + (2 * abs(dx));
					else
					{
						x = x + 1;
						pk = pk + (2 * abs(dx)) - (2 * abs(dy));
					}

					image[x][y] = color;
				}

			}
		}

	void draw(vector<vector<string>>& image, string color) {

		int x1 = a.get_x();
		int x2 = b.get_x();
		int y1 = a.get_y();
		int y2 = b.get_y();

		cout << x1 << " " << y1 << " " << x2 << " " << y2 << endl;

		int dx = abs(x2 - x1);
		int dy = abs(y2 - y1);

		int i = x1;
		int j = y1;
		int e = dy - dx;

		// down right

		if (x2 - x1 > 0 && y2 - y1 > 0) {

			if (e < 0)
				//shallow
				for (i; i < x2; i++) {
					image[i][j] = color;
					if (e >= 0) {
						j += 1;
						e -= dx;
					}
					e += dy;
				}
			else {
				//steep
				for (j; j < y2; j++) {
					image[i][j] = color;
					if (e >= 0) {
						i += 1;
						e -= dy;
					}
					e += dx;
				}
			}
		}

		// down left

		if (x2 - x1 < 0 && y2 - y1 > 0) {

			if (e < 0)
				//shallow
				for (i; i > x2; i--) {
					image[i][j] = color;
					if (e >= 0) {
						j += 1;
						e -= dx;
					}
					e += dy;
				}
			else {
				//steep
				for (j; j < y2; j++) {
					image[i][j] = color;
					if (e >= 0) {
						i -= 1;
						if (i == -1)
							break;
						e -= dy;
					}
					e += dx;
				}
			}
		}

		// up right

		if (x2 - x1 > 0 && y2 - y1 < 0) {

			if (e < 0)
				//shallow
				for (i; i < x2; i++) {
					image[i][j] = color;
					if (e >= 0) {
						j -= 1;
						if (j == -1)
							break;
						e -= dx;
					}
					e += dy;
				}
			else {
				//steep
				for (j; j > y2; j--) {
					image[i][j] = color;
					if (e >= 0) {
						i += 1;
						e -= dy;
					}
					e += dx;
				}
			}
		}

		// up left

		if (x2 - x1 < 0 && y2 - y1 < 0) {

			if (e < 0)
				//shallow
				for (i; i > x2; i--) {
					image[i][j] = color;
					if (e >= 0) {
						j -= 1;
						if (j == -1)
							break;
						e -= dx;
					}
					e += dy;
				}
			else {
				//steep
				for (j; j > y2; j--) {
					image[i][j] = color;
					if (e >= 0) {
						i -= 1;
						if (i == -1)
							break;
						e -= dy;
					}
					e += dx;
				}
			}
		}

	}

};


vector<string> split(string str) {
	string temp = "";
	char del = ' ';

	vector<string> v;

	for (int i = 0; i < (int)str.size(); i++) {
		if (str[i] != del) {
			temp += str[i];
		}
		else {
			v.push_back(temp);
			temp = "";
		}
	}

	return v;
}

void area_fill(vector<vector<int>>& color, int xx, int yy) {

	queue<tuple<int, int>> q;

	q.push(make_tuple(xx, yy));

	while (!q.empty()) {

		int x = get<0>(q.front());
		int y = get<1>(q.front());

		q.pop();

		if (color[x][y] == 3 || color[x][y] == 0 || x < 1 || y < 1 || x >= color.size() || y >= color[0].size())
			continue;

		if (color[x][y] == 1 || color[x][y] == 2)
			color[x][y] = 3;
		
		q.push(make_tuple(x - 1, y));
		q.push(make_tuple(x, y - 1));
		q.push(make_tuple(x + 1, y));
		q.push(make_tuple(x, y + 1));

		q.push(make_tuple(x + 1, y + 1));
		q.push(make_tuple(x - 1, y + 1));
		q.push(make_tuple(x - 1, y - 1));
		q.push(make_tuple(x + 1, y - 1));
	
	}

}

void area_fill_r(vector<vector<int>>& color, int x, int y) {


	if (color[x][y] == 3 || color[x][y] == 0 || x < 1 || y < 1 || x >= color.size() || y >= color[0].size())
		return;

	if (color[x][y] == 1 || color[x][y] == 2)
		color[x][y] = 3;

	area_fill(color, x - 1, y);
	area_fill(color, x, y - 1);
	area_fill(color, x + 1, y);
	area_fill(color, x, y + 1);

	area_fill(color, x + 1, y + 1);
	area_fill(color, x - 1, y + 1);
	area_fill(color, x - 1, y - 1);
	area_fill(color, x + 1, y - 1);

}

vector<vector<string>> grayscale() {

	string line;
	vector<string> input;
	ifstream infile;
	infile.open(ifilename);

	string token = "";
	int i = 0;

	int height = 0;
	int width = 0;

	while (infile >> token) {

		if (i == 1)
			width = stoi(token);
		else if (i == 2)
			height = stoi(token);
		else if (i > 2)
			break;

		i++;
	}

	WIDTH = width;
	HEIGHT = height;

	ofstream outfile;
	outfile.open(ofilename, ios::out | ios::trunc | ios::binary);
	outfile << "P3 " << width << " " << height << " 255\n";

	vector<vector<string>> color;

	for (int i = 0; i < width; i++) {
		vector<string> v;
		for (int j = 0; j < height; j++) {

			infile >> token;
			int red = stoi(token);

			infile >> token;
			int green = stoi(token);

			infile >> token;
			int blue = stoi(token);

			int avg = (red + green + blue) / 3;

			outfile << to_string(avg) << " " << to_string(avg) << " " << to_string(avg) << " ";

			v.push_back(to_string(avg));
		}
		color.push_back(v);
		outfile << "\n";
	}

	outfile.close();

	return color;

}

vector<vector<tuple<double, double>>> suppression(vector<vector<string>> color) {

	float kernelx[3][3] = {
			{ -1, 0, 1 },
			{ -2, 0, 2 },
			{ -1, 0, 1 } };

	float kernely[3][3] = {
			{ -1, -2, -1 },
			{ 0,  0,  0 },
			{ 1,  2,  1 } };

	int width = color[0].size();
	int height = color.size();

	vector<vector<tuple<double, double>>> magn(height, vector<tuple<double, double>>(width, tuple<double, double>(0, 0)));

	double magX, magY;

	for (int row = 1; row < height - 1; row++) {

		for (int col = 1; col < width - 1; col++) {

			magX = 0.0;
			magY = 0.0;

			for (int a = 0; a < 3; a++)
			{
				for (int b = 0; b < 3; b++)
				{
					int xn = row + a - 1;
					int yn = col + b - 1;

				
					magX += stod(color[xn][yn]) * kernelx[a][b];
					magY += stod(color[xn][yn]) * kernely[a][b];

				}
			}

			magn[row][col] = make_tuple(magX, magY);
			//cout << magX << " " << magY << endl;

		}
	}

	return magn;

}

vector<vector<tuple<double, double>>> suppression_int(vector<vector<int>> color) {
	float kernelx[3][3] = {
			{ -1, 0, 1 },
			{ -2, 0, 2 },
			{ -1, 0, 1 } };

	float kernely[3][3] = {
			{ -1, -2, -1 },
			{ 0,  0,  0 },
			{ 1,  2,  1 } };

	int width = color[0].size();
	int height = color.size();

	vector<vector<tuple<double, double>>> magn(height, vector<tuple<double, double>>(width, tuple<double, double>(0, 0)));

	for (int row = 1; row < height - 1; row++) {

		for (int col = 1; col < width - 1; col++) {

			double magX = 0.0;
			double magY = 0.0;

			for (int a = 0; a < 3; a++)
			{
				for (int b = 0; b < 3; b++)
				{
					int xn = row + a - 1;
					int yn = col + b - 1;

					double c = color[xn][yn];
					double ky = kernelx[a][b];
					double kx = kernely[a][b];

					magX += c * kx;
					magY += c * ky;

				}


				magn[row][col] = make_tuple(magX, magY);

			}
		}

		return magn;

	}
}

vector<vector<int>> hysteresis(vector<vector<string>> color, int low, int high) {

	float kernelx[3][3] = {
			{ -1, 0, 1 },
			{ -2, 0, 2 },
			{ -1, 0, 1 } };

	float kernely[3][3] = {
			{ -1, -2, -1 },
			{ 0,  0,  0 },
			{ 1,  2,  1 } };

	int width = color[0].size();
	int height = color.size();

	vector<vector<int>> magn(height, vector<int>(width, 0));

	for (int row = 1; row < height - 1; row++) {

		for (int col = 1; col < width - 1; col++) {

			double magX = 0.0;
			double magY = 0.0;

			for (int a = 0; a < 3; a++)
			{
				for (int b = 0; b < 3; b++)
				{
					int xn = row + a - 1;
					int yn = col + b - 1;

					magX += stoi(color[xn][yn]) * kernelx[a][b];
					magY += stoi(color[xn][yn]) * kernely[a][b];

				}
			}
			int magnitude = sqrt(magX * magX + magY * magY);

			magn[row][col] = magnitude;

		}
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (magn[i][j] >= high)
				magn[i][j] = 2;
			else if (magn[i][j] >= low)
				magn[i][j] = 1;
			else magn[i][j] = 0;
		}
	}

	return magn;

}

vector<vector<tuple<double, double>>> magn_dirs(vector<vector<tuple<double, double>>> magnitudes) {
	vector<vector<tuple<double, double>>> suppression(magnitudes.size(), vector<tuple<double, double>>(magnitudes[0].size(), tuple<double, double>(0, 0)));

	for (int i = 0; i < magnitudes.size() - 1; i++)

		for (int j = 0; j < magnitudes[0].size() - 1; j++) {

			double magn = sqrt(pow(get<0>(magnitudes[i][j]), 2) + pow(get<1>(magnitudes[i][j]), 2));
			double dir = atan2(get<1>(magnitudes[i][j]), get<0>(magnitudes[i][j]));
			suppression[i][j] = make_tuple(magn, dir);

		}
	return suppression;
}

vector<vector<tuple<double, int>>> magn_dirs_1(vector<vector<tuple<double, double>>> magnitudes) {
	vector<vector<tuple<double, int>>> suppression(magnitudes.size(), vector<tuple<double, int>>(magnitudes[0].size(), tuple<double, int>(0, 0)));

	for (int i = 0; i < magnitudes.size() - 1; i++)

		for (int j = 0; j < magnitudes[0].size() - 1; j++) {

			double magn = sqrt(pow(get<0>(magnitudes[i][j]), 2) + pow(get<1>(magnitudes[i][j]), 2));
			int dir = int(round((atan2(get<1>(magnitudes[i][j]), get<0>(magnitudes[i][j])) * (180 / M_PI)) / 45.0) * 45.0);
			suppression[i][j] = make_tuple(magn, dir);

		}

	return suppression;
}

vector<vector<int>> nms(vector<vector<tuple<double, double>>> magnitudes) {

	vector<vector<tuple<double, int>>> suppression = magn_dirs_1(magnitudes);

	vector<vector<int>> magn_v(magnitudes.size(), vector<int>(magnitudes[0].size(), 0));

	for (int i = 1; i < suppression.size() - 1; i++)
		for (int j = 1; j < suppression[0].size() - 1; j++) {

			double dir = get<1>(suppression[i][j]);
			double magn = get<0>(suppression[i][j]);
			double magn1;
			double magn2;

			if (dir == 180 || dir == -180 || dir == 0) {
				magn1 = get<0>(suppression[i][j - 1]);
				magn2 = get<0>(suppression[i][j + 1]);
			}
			else if (dir == 45 || dir == -135) {
				magn1 = get<0>(suppression[i + 1][j + 1]);
				magn2 = get<0>(suppression[i - 1][j - 1]);
			}
			else if (dir == 135 || dir == -45) {
				magn1 = get<0>(suppression[i + 1][j - 1]);
				magn2 = get<0>(suppression[i - 1][j + 1]);
			}
			else if (dir == 90 || dir == -90) {
				magn1 = get<0>(suppression[i + 1][j]);
				magn2 = get<0>(suppression[i - 1][j]);
			}

			if (magn > magn1 && magn > magn2)
				magn_v[i][j] = 1;

		}


	ofstream outfile;
	outfile.open(ofilename3, ios::out | ios::trunc | ios::binary);
	outfile << "P3 " << magnitudes.size() << " " << magnitudes[0].size() << " 1 ";

	for (int i = 0; i < magn_v.size(); i++) {
		for (int j = 0; j < magn_v[0].size(); j++)
			if (magn_v[i][j] == 1)
				outfile << "1 1 1 ";
			else
				outfile << "0 0 0 ";
		outfile << endl;
	}


	return magn_v;
}

vector<vector<int>> hysteresis_method(int low, int high) {

	vector<vector<string>> color = grayscale();
	vector<vector<int>> magn = hysteresis(color, low, high);

	for (int i = 1; i < magn.size() - 1; i++)
		for (int j = 1; j < magn[0].size() - 1; j++)
			if (magn[i][j] == 2) {
				area_fill(magn, i, j);
			}


	ofstream outfile;
	outfile.open(ofilename2, ios::out | ios::trunc | ios::binary);
	outfile << "P3 " << color.size() << " " << color[0].size() << " 1 ";

	for (int i = 0; i < magn.size(); i++) {
		for (int j = 0; j < magn[0].size(); j++)
			if (magn[i][j] == 3) {
				magn[i][j] = 1;
				outfile << "1 1 1 ";
			}
			else
				outfile << "0 0 0 ";
		outfile << endl;
	}

	outfile.close();

	return magn;
}

vector<vector<int>> canny_edge(int argc, char** argv) {

	int low = 110;
	int high = 170;

	if (argc > 0) {

		int i = 1;
		while (i < argc) {

			string temp;
			string t = argv[i];

			if (t == "-L") {
				i++;
				temp += argv[i];
				low = stoi(temp);
			}
			else if (t == "-H") {
				i++;
				temp += argv[i];
				high = stoi(temp);
			}
			else if (t == "-F") {
				i++;
				temp += argv[i];
				ifilename = temp;
			}
			i++;

		}
	}

	vector<vector<int>> magn1 = hysteresis_method(low, high);

	vector<vector<string>> color = grayscale();
	vector<vector<tuple<double, double>>> magn = suppression(color);

	vector<vector<int>> magn2 = nms(magn);

	ofstream outfile;
	outfile.open(ofilenamef, ios::out | ios::trunc | ios::binary);
	outfile << "P3 " << magn1.size() << " " << magn1[0].size() << " 1 ";

	int width = color[0].size();
	int height = color.size();

	vector<vector<int>> ret(height, vector<int>(width, 0));

	for (int i = 0; i < magn1.size(); i++) {
		for (int j = 0; j < magn1[0].size(); j++) {
			if (magn1[i][j] == 1 && magn2[i][j] == 1) {
				outfile << "1 1 1 ";
				ret[i][j] = 1;
			}
			else {
				outfile << "0 0 0 ";
				ret[i][j] = 0;
			}
		}
		outfile << endl;
	}


	return ret;
}

vector<vector<int>> voting(vector<vector<string>> g, vector<vector<int>> c) {

	int width = g[0].size();
	int height = g.size();

	vector<vector<int>> votes(height, vector<int>(width, 0));

	vector<vector<tuple<double, double>>> m = suppression(g);
	//vector<vector<tuple<double, double>>> md = magn_dirs(m);

	Point p = Point(-1, -1);

	int count = 0;
	for (int i = 0; i < m.size(); i++) {
		for (int j = 0; j < m[0].size(); j++) {
			if (c[i][j] == 1) {
				if (!(get<0>(m[i][j]) == 0 || get<1>(m[i][j]) == 0)) {
					count++;
					//cout << double(get<1>(m[i][j])) << " " << double(get<0>(m[i][j])) << " " << double(get<1>(m[i][j])) / double(get<0>(m[i][j])) << endl;
					Line l = Line(Point(double(i) / WIDTH, double(j) / HEIGHT), double(get<1>(m[i][j])) / double(get<0>(m[i][j])));
					l.vote_line(votes);
				}
			}

		}
	}


	return votes;
}

void generate_voting(vector<vector<int>> votes) {

	int max = 0;

	for(int i = 0; i < votes.size(); i++)
		for (int j = 0; j < votes[0].size(); j++) 
			if (votes[i][j] > max)
				max = votes[i][j];

	ofstream outfile;
	outfile.open(ofilenamev, ios::out | ios::trunc | ios::binary);
	outfile << "P3 " << votes.size() << " " << votes[0].size() << " " << to_string(max) << " ";

	for (int i = 0; i < votes.size(); i++) {
		for (int j = 0; j < votes[0].size(); j++) {
			string s = to_string(votes[i][j]);																																										//my son.
			outfile << s << " " << s << " " << s << " ";																																							//he has cancer
		}
		outfile << endl;
	}	

}


void part1(int argc, char* argv[]) {
	

	vector<vector<int>> c_edge = canny_edge(argc, argv);
	vector<vector<string>> g = grayscale();
	vector<vector<int>> votes = voting(g, c_edge);
	generate_voting(votes); 

}

int main(int argc, char* argv[]) {
	part1(argc, argv);
}
