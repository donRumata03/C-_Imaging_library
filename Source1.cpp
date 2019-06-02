#include <fstream>
#include <iostream>

using namespace std;


class Image {
private:

public:
	int*** ms;
	int w;
	int h;
	int num_leafs;
	int randcolor() {
		return rand() % 256;
	}
	void neu(int r, int g, int b){
		ms = new int**[w];
		for (int x = 0; x < w; x++) {
			ms[x] = new int*[h];
			for (int y = 0; y < h; y++)
			{
				ms[x][y] = new int[3];
				ms[x][y][0] = r;
				ms[x][y][1] = g;
				ms[x][y][2] = b;
			}
		}
	}
	float dist(int n1, int n2) {
		return pow((n2 - n1), 2);
	}
	void open(char name[100]) {
		fstream img(name);
		char s;
		int n;
		img >> s >> s;
		img >> w >> h >> n;
		ms = new int**[w];
		for (int x = 0; x < w; x++) {
			ms[x] = new int*[h];
		}


		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				ms[x][y] = new int[3];
			}
		}
		long i = 0;
		while (true) {
			int y = (int)(i / w);
			int x = i % w;
			if (x >= w || y >= h) {
				break;
			}
			int r, g, b;
			img >> r >> g >> b;
			ms[x][y][0] = r;
			ms[x][y][1] = g;
			ms[x][y][2] = b;
			i++;
		}
	}
	void save(char name[100]) {
		ofstream img(name);
		img << "P3" << endl;
		
		img << w << " " << h << endl;
		img << "255" << endl;


		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++)
			{
				int r = ms[x][y][0];
				int g = ms[x][y][1];
				int b = ms[x][y][2];
				img << r << " " << g << " " << b << endl;
			}
		}
	}
	void putpixel(int x, int y, int r, int g, int b) {
		ms[x][y][0] = r;
		ms[x][y][1] = g;
		ms[x][y][2] = b;
	}
	int* getpixel(int x, int y) {
		int r = ms[x][y][0];
		int g = ms[x][y][1];
		int b = ms[x][y][2];
		int col[3] = { r, g, b };
		return col;
	}
	void printpixel(int x, int y) {
		cout << this->getpixel(x, y)[0];
	}
	bool** obxod(int x0, int y0, int limit) {
		bool** taken = new bool*[w];
		for (int i = 0; i < w; i++) {
			taken[i] = new bool[h];
			for (int j = 0; j < h; j++) {
				taken[i][j] = false;
			}
		}
		int **last_wave = new int*[1];
		last_wave[0] = new int[2];
		last_wave[0][0] = x0;
		last_wave[0][1] = y0;
		int len_last = 1;
		while (len_last > 0) {
			int len_this = 0;
			int** this_wave = new int* [(len_last * 4)];
			for (int i = 0; i < (len_last * 4); i++) {
				this_wave[i] = new int[2];
				this_wave[i][0] = 0;
				this_wave[i][1] = 0;
			}
			for (int pix = 0; pix < len_last; pix++) {
				int x = last_wave[pix][0];
				int y = last_wave[pix][1];
				int* col = this->getpixel(x, y);
				if (y != 0) {
					int* col_up = this->getpixel(x, y - 1);
					if ((!taken[x][y - 1]) && (((this->dist(col[0], col_up[0])) + (this->dist(col[1], col_up[1])) + (this->dist(col[2], col_up[2]))) < limit)) {
						this_wave[len_this][0] = x;
						this_wave[len_this][1] = y - 1;
						len_this++;
						taken[x][y - 1] = true;
						// cout << x << " " << y - 1 << endl;
					}
				}
				if (x != 0) {
					int* col_left = this->getpixel(x - 1, y);
					if ((!taken[x - 1][y]) && ((this->dist(col[0], col_left[0]) + this->dist(col[1], col_left[1]) + this->dist(col[2], col_left[2])) < limit)) {
						this_wave[len_this][0] = x - 1;
						this_wave[len_this][1] = y;
						len_this++;
						taken[x - 1][y] = true;
						// cout << x - 1 << " " << y << endl;
					}
				}
				if (y != h - 1) {
					int* col_down = this->getpixel(x, y + 1);
					if ((!taken[x][y + 1]) && ((this->dist(col[0], col_down[0]) + this->dist(col[1], col_down[1]) + this->dist(col[2], col_down[2])) < limit)) {
						this_wave[len_this][0] = x;
						this_wave[len_this][1] = y + 1;
						len_this++;
						taken[x][y + 1] = true;
						// cout << x << " " << y + 1 << endl;
					}
				}
				if (x != w - 1) {
					int* col_right = this->getpixel(x + 1, y);
					if ((!taken[x + 1][y]) && ((this->dist(col[0], col_right[0]) + this->dist(col[1], col_right[1]) + this->dist(col[2], col_right[2])) < limit)) {
						this_wave[len_this][0] = x + 1;
						this_wave[len_this][1] = y;
						len_this++;
						taken[x + 1][y] = true;
						// cout << x + 1 << " " << y << endl;
					}
				}
			}
			len_last = len_this;
			delete[] last_wave;
			last_wave = this_wave;
		}
		return taken;
	}
	int** bin_obxod(int x0, int y0, bool** taken) {
		int** all_wave = new int*[w*h];
		int len_all = 0;
		bool bool_0 = taken[x0][y0];
		for (int i = 0; i < w*h; i++) {
			all_wave[i] = new int[2];
			all_wave[i][0] = w+1;
			all_wave[i][1] = h+1;
		}
		int **last_wave = new int*[1];
		last_wave[0] = new int[2];
		last_wave[0][0] = x0;
		last_wave[0][1] = y0;
		int len_last = 1;
		while (len_last > 0) {
			int len_this = 0;
			int** this_wave = new int*[(len_last * 4)];
			for (int i = 0; i < (len_last * 4); i++) {
				this_wave[i] = new int[2];
				this_wave[i][0] = 0;
				this_wave[i][1] = 0;
			}
			for (int pix = 0; pix < len_last; pix++) {
				int x = last_wave[pix][0];
				int y = last_wave[pix][1];
				int* col = this->getpixel(x, y);
				if (y != 0) {
					if (taken[x][y - 1] == bool_0) {
						this_wave[len_this][0] = x;
						this_wave[len_this][1] = y - 1;
						len_this++;
						taken[x][y - 1] = !bool_0;
					}
				}
				if (x != 0) {
					if (taken[x - 1][y] == bool_0) {
						this_wave[len_this][0] = x - 1;
						this_wave[len_this][1] = y;
						len_this++;
						taken[x - 1][y] = !bool_0;
					}
				}
				if (y != h - 1) {
					if (taken[x][y + 1] == bool_0) {
						this_wave[len_this][0] = x;
						this_wave[len_this][1] = y + 1;
						len_this++;
						taken[x][y + 1] = !bool_0;
					}
				}
				if (x != w - 1) {
					if (taken[x + 1][y] == bool_0) {
						this_wave[len_this][0] = x + 1;
						this_wave[len_this][1] = y;
						len_this++;
						taken[x + 1][y] = !bool_0;
					}
				}
			}
			for (int ind = 0; ind < len_this; ind++) {
				all_wave[len_all][0] = (int)(this_wave[ind][0]);
				all_wave[len_all][1] = (int)(this_wave[ind][1]);
				len_all++;
			}
			len_last = len_this;
			delete[] last_wave;
			last_wave = this_wave;
		}
		return all_wave;
	}
	int** cool_bin_obxod(int x0, int y0, bool** taken) {
		int** all_wave = new int*[w*h];
		int len_all = 0;
		bool bool_0 = taken[x0][y0];
		for (int i = 0; i < w*h; i++) {
			all_wave[i] = new int[2];
			all_wave[i][0] = w + 1;
			all_wave[i][1] = h + 1;
		}
		int **last_wave = new int*[1];
		last_wave[0] = new int[2];
		last_wave[0][0] = x0;
		last_wave[0][1] = y0;
		int len_last = 1;
		while (len_last > 0) {
			int len_this = 0;
			int** this_wave = new int*[(len_last * 4)];
			for (int i = 0; i < (len_last * 4); i++) {
				this_wave[i] = new int[2];
				this_wave[i][0] = 0;
				this_wave[i][1] = 0;
			}
			for (int pix = 0; pix < len_last; pix++) {
				int x = last_wave[pix][0];
				int y = last_wave[pix][1];
				int* col = this->getpixel(x, y);
				if (y != 0) {
					if (taken[x][y - 1] == bool_0) {
						this_wave[len_this][0] = x;
						this_wave[len_this][1] = y - 1;
						len_this++;
						taken[x][y - 1] = !bool_0;
					}
				}
				if (x != 0) {
					if (taken[x - 1][y] == bool_0) {
						this_wave[len_this][0] = x - 1;
						this_wave[len_this][1] = y;
						len_this++;
						taken[x - 1][y] = !bool_0;
					}
				}
				if (y != 0 && x != 0) {
					if (taken[x - 1][y - 1] == bool_0) {
						this_wave[len_this][0] = x - 1;
						this_wave[len_this][1] = y - 1;
						len_this++;
						taken[x - 1][y - 1] = !bool_0;
					}
				}
				if (y != h - 1) {
					if (taken[x][y + 1] == bool_0) {
						this_wave[len_this][0] = x;
						this_wave[len_this][1] = y + 1;
						len_this++;
						taken[x][y + 1] = !bool_0;
					}
				}
				if (y != 0 && x != w - 1) {
					if (taken[x + 1][y - 1] == bool_0) {
						this_wave[len_this][0] = x + 1;
						this_wave[len_this][1] = y - 1;
						len_this++;
						taken[x + 1][y - 1] = !bool_0;
					}
				}
				if (x != w - 1) {
					if (taken[x + 1][y] == bool_0) {
						this_wave[len_this][0] = x + 1;
						this_wave[len_this][1] = y;
						len_this++;
						taken[x + 1][y] = !bool_0;
					}
				}
				if (x != 0 && y != h - 1) {
					if (taken[x - 1][y+ 1] == bool_0) {
						this_wave[len_this][0] = x - 1;
						this_wave[len_this][1] = y + 1;
						len_this++;
						taken[x - 1][y+1] = !bool_0;
					}
				}
				if (x != w - 1 && y != h-1) {
					if (taken[x + 1][y + 1] == bool_0) {
						this_wave[len_this][0] = x + 1;
						this_wave[len_this][1] = y + 1;
						len_this++;
						taken[x + 1][y+1] = !bool_0;
					}
				}
			}
			for (int ind = 0; ind < len_this; ind++) {
				all_wave[len_all][0] = (int)(this_wave[ind][0]);
				all_wave[len_all][1] = (int)(this_wave[ind][1]);
				len_all++;
			}
			len_last = len_this;
			delete[] last_wave;
			last_wave = this_wave;
		}
		return all_wave;
	}
	int*** make_list(bool** taken) {
		num_leafs = 0;
		const int max_num_leafs = 10;
		int*** leafs = new int**[max_num_leafs];
		for (int l = 0; l < max_num_leafs; l++) {
			leafs[l] = new int*[w*h];
			for (int pix = 0; pix < this->w * this->h; pix++) {
				leafs[l][pix] = new int[2];
				leafs[l][pix][0] = 0;
				leafs[l][pix][1] = 0;
			}
		}
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				if (taken[x][y]) {
					int** leaf = this->cool_bin_obxod(x, y, taken);
					int r = this->randcolor();
					int g = this->randcolor();
					int b = this->randcolor();
					int	i = 0;
					while (i < (w*h) && leaf[i][0] < w && leaf[i][1] < h) {
						leafs[num_leafs][i][0] = leaf[i][0];
						leafs[num_leafs][i][1] = leaf[i][1];
						//this->putpixel(leaf[i][0], leaf[i][1], r, g, b);
						i++;
					}

				
				num_leafs++;

				}
			}
		}
		return leafs;
	}
	bool** find_borders(bool** taken) {
		bool** new_taken = new bool*[w];
		for (int x = 0; x < w; x++) {
			new_taken[x] = new bool[h];
			for (int y = 0; y < h; y++) {
				new_taken[x][y] = false;
				if (taken[x][y]) {
					if (x != 0 && (!taken[x - 1][y])) {
						new_taken[x][y] = true;
						continue;
					}
					if (y != 0 && (!taken[x][y - 1])) {
						new_taken[x][y] = true;
						continue;
					}
					if (x <= w-2 && (!taken[x + 1][y])) {
						new_taken[x][y] = true;
						continue;
					}
					if (y <= h-2 && (!taken[x][y + 1
])) {
						new_taken[x][y] = true;
						continue;
					}
				}
			}
		}
		return new_taken;
	}
	double* make_line(int num, int** points) {
		long sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
		for (int i = 0; i < num; i++) {
			sum_x += points[i][0];
			sum_y += points[i][1];
			sum_xy += points[i][1] * points[i][0];
			sum_x2 += points[i][0] * points[i][0];
		}
		double a = ((double)((num * sum_xy) - (sum_x * sum_y))) / ((double)((num * sum_x2)  - (sum_x * sum_x)));
		double b = ((double)(sum_y - (a * sum_x))) / ((double)(num));
		double c[2] = {a, b};
		return c;
	}
	bool** l_to_t(int **list) {
		bool** taken = new bool*[w];
		for (int x = 0; x < w; x++) {
			taken[x] = new bool[h];
			for (int y = 0; y < h; y++) {
				taken[x][y] = false;
			}
		}
		int i = 0;
		while (i < w*h && list[i][0] != 0) {
			taken[list[i][0]][list[i][1]] = true;
			i++;
		}
		return taken;
	}
	int** conseq(bool** taken) {
		bool ** new_taken = new bool* [w];
		for (int x = 0; x < w; x++) { new_taken[x] = new bool[h]; for (int y = 0; y < h; y++) { new_taken[x][y] = false; } }
		int point[2];
		int** cons_list = new int*[w*h];
		for (int i = 0; i < w*h; i++) {
			cons_list[i] = new int[2];
			cons_list[i][0] = w + 1;
			cons_list[i][1] = h + 1;
		}

		int** variants = new int*[w*h];
		for (int i = 0; i < w*h; i++) {
			variants[i] = new int[2];
			variants[i][0] = w + 1;
			variants	[i][1] = h + 1;
		}
		int last_var = -1;

		bool flag_found = false;
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				if (taken[x][y]) {
					point[0] = x;
					point[1] = y;
					cout << "First point: " << x << " " << y << endl;
					flag_found = true;
					break;
				}
			}
			if (flag_found) {
				break;
			}
		}
		int point_0[2];
		point_0[0] = point[0];
		point_0[1] = point[1];
		int come = -1;
		new_taken[point_0[0]][point_0[1]] = true;
		int num_point = 0;
		do  {
			// Following the hour pointer
			bool had = false;
			int x = point[0];
			int y = point[1];
			if (come != 4 && y != 0 && taken[x][y - 1] && (!new_taken[x][y - 1])) {
				point[0] = x;
				point[1] = y - 1;
				come = 0;
				new_taken[x][y - 1] = true;
				had = true;
			}
			if(come != 5 && y != 0 && x != w - 1 && taken[x + 1][y - 1] && (!new_taken[x + 1][y - 1])) {
				if (!had) {
					point[0] = x + 1;
					point[1] = y - 1;
					come = 1;
					new_taken[x + 1][y - 1] = true;
					had = true;
				}
				else {
					last_var++;
					variants[last_var][0] = point[0];
					variants[last_var][1] = point[1];
				}
			}
			if (come != 6 && x != w - 1 && taken[x + 1][y] && (!new_taken[x + 1][y])) {
				if (!had) {
					point[0] = x + 1;
					point[1] = y;
					come = 2;
					new_taken[x + 1][y] = true;
					had = true;
				}
				else {
					last_var++;
					variants[last_var][0] = point[0];
					variants[last_var][1] = point[1];
				}
			}
			if (come != 7 && y != h - 1 && x != w - 1 && taken[x + 1][y + 1] && (!new_taken[x + 1][y + 1])) {
				if (!had) {
					point[0] = x + 1;
					point[1] = y + 1;
					come = 3;
					new_taken[x + 1][y + 1] = true;
					had = true;
				}
				else {
					last_var++;
					variants[last_var][0] = point[0];
					variants[last_var][1] = point[1];
				}
			}
			if (come != 0 && y != h - 1 && taken[x][y + 1] && (!new_taken[x][y + 1])) {
				if (!had) {
					had = true;
					point[0] = x;
					point[1] = y + 1;
					come = 4;
					new_taken[x][y + 1] = true;
				}
				else {
					last_var++;
					variants[last_var][0] = point[0];
					variants[last_var][1] = point[1];
				}
			}
			if (come != 1 && y != h - 1 && x != 0 && taken[x - 1][y + 1] && (!new_taken[x - 1][y + 1])) {
				if (!had) {
					had = true;
					point[0] = x - 1;
					point[1] = y + 1;
					come = 5;
					new_taken[x - 1][y + 1] = true;
				}
				else {
					last_var++;
					variants[last_var][0] = point[0];
					variants[last_var][1] = point[1];
				}
			}
			if (come != 2 && x != 0 && taken[x - 1][y] && (!new_taken[x - 1][y])) {
				if (!had) {
					had = true;
					point[0] = x - 1;
					point[1] = y;
					come = 6;
					new_taken[x - 1][y] = true;
				}
				else {
					last_var++;
					variants[last_var][0] = point[0];
					variants[last_var][1] = point[1];
				}
			}
			if (come != 3 && x != 0 && y != 0 && taken[x - 1][y - 1] && (!new_taken[x - 1][y - 1])) {
				if (!had) {
					had = true;
					point[0] = x - 1;
					point[1] = y - 1;
					come = 7;
					new_taken[x - 1][y - 1] = true;
				}
				else {
					last_var++;
					variants[last_var][0] = point[0];
					variants[last_var][1] = point[1];
				}
			}
			if (!had && last_var != -1 && variants[last_var][0] != w + 1)
			{
				cout << "Switched!" << endl;
				point[0] = variants[last_var][0];
				point[1] = variants[last_var][1];
				variants[last_var][0] = w-1;
				last_var--;
			}
			cout << point[0] << " " << point[1] << endl;
			cout << come << endl;
			cons_list[num_point][0] = point[0];
			cons_list[num_point][1] = point[1];
			num_point++;
			
		} while (point[0] != point_0[0] || point[1] != point_0[1]);
		
		cout << "Num_points: " << num_point << endl;
		return cons_list;
	}
};


void save(int*** ms, char name[100], int w, int h) {
	ofstream img(name);
	img << "P3" << endl;
	
	img << w << " " << h << endl;
	img << "255" << endl;

	
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++)
		{
			int r = ms[x][y][0];
			int g = ms[x][y][1];
			int b = ms[x][y][2];
			img << r << " " << g << " " << b << endl;
		}

	}
}

int*** open(int size[2],  char name[100]) {
	fstream img(name);
	char s;
	int h, w, n;
	img >> s >> s;
	img >> w >> h >> n;
	int*** ms = new int**[w];
	for (int x = 0; x < w; x++) {
		ms[x] = new int*[h];
	}

	
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
		ms[x][y] = new int[3];
	}
	}
	long i = 0;
	while (true) {
		int y = (int)(i/w);
		int x = i % w;
		if (x >= w || y >= h) {
			break;
		}
		int r, g, b;
		img >> r >> g >> b;
		// cout << x  << " " << y;
		ms[x][y][0] = r;
		ms[x][y][1] = g;
		ms[x][y][2] = b;
		i++;
	}
	size[0] = w;
	size[1] = h;
	return ms;
}


void putpixel(int ***ms, int x, int y, int r, int g, int b) {
	ms[x][y][0] = r;
	ms[x][y][1] = g;
	ms[x][y][2] = b;
}


int* getpixel(int ***ms, int x, int y) {
	int r = ms[x][y][0];
	int g = ms[x][y][1];
	int b = ms[x][y][2];
	int col[3] = {r, g, b};
	return col;
}


int main()
{
	Image image;
	image.open((char*)"saved.ppm");
	bool** taken = image.obxod(0, 0, 100);
	
	
	Image img2;
	img2.w = image.w;
	img2.h = image.h;
	img2.neu(0, 0, 0);
	
	bool** borders = img2.find_borders(taken);
	for (int x = 0; x < image.w; x++) {
		for (int y = 0; y < image.h; y++) {
			if (taken[x][y]) {
				img2.putpixel(x, y, 200, 200, 0);
			}
			else {
				img2.putpixel(x, y, 0, 200, 200);
			}
		}
	}
	img2.save((char*)("Hello.ppm"));
	int *** lists = img2.make_list(borders);
	// cout << "    " << lists[1][0][0] << "  " << lists[1][0][1] << endl;
	
	int *** lists = img2.make_list(borders);
	int* list1[5];
	for (int i = 0; i < 5; i++) {
		list1[i] = new int[2];
	}
	for (int i = 0; i < img2.num_leafs; i++) {
		int r = img2.randcolor();
		int g = img2.randcolor();
		int b = img2.randcolor();
		int num_list = 0;
		while (num_list < img2.w * img2.h && (lists[i][num_list][0] != img2.w + 1))
		{
			if (i == 0 && num_list < 5) {
				list1[num_list][0] = lists[i][num_list][0];
				list1[num_list][1] = lists[i][num_list][1];
				// cout << list1[num_list][0] << " " << list1[num_list][1] << endl;
			}
			img2.putpixel(lists[i][num_list][0], lists[i][num_list][1], r, g, b);
			num_list++;
		}
	}
	double* ks = img2.make_line(5, list1);
	double a = ks[0];
	double b = ks[1];
	cout << "A: " << a << " B: " << b << endl;

	
	img2.save((char*)"result.ppm");
	system("pause");
}