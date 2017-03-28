#include <cstdio>
#include <vector>
#include <algorithm>
double p[4][2] = { { 0.5, 0.25 }, { 0.25, 0.5 }, { 0.5, 0.75 }, { 0.75, 0.5 } };
struct tre{
	int x, y, z;
	tre(){}
	tre(int x, int y, int z) : x(x), y(y), z(z){}
};
int n, K;

//inline long double dist2(long double x1, long double y1, long double x2, long double y2){
inline double dist2(double x1, double y1, double x2, double y2){
	return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

int Main(){
	freopen("dogfood_input.txt", "r", stdin);
	scanf("%d%d", &n, &K);
	std::vector<tre> food;
	for (int i = 0; i < n; i++){
		int t1, t2, t3;
		scanf("%d%d%d", &t1, &t2, &t3);
		food.push_back(tre(t1, t2, t3));
	}
	std::vector<std::vector<int>> bu;
	bu.resize(10000);
	for (auto &i : bu)
		i.resize(10);
#pragma omp parallel for
	for (int i = 0; i < 10000; i++){
		printf("doing i: %d\n", i);
		for (int j = 0; j < 10000; j++)
			for (int k = 0; k < 4; k++){
				double x = p[k][0] + i, y = p[k][1] + j;
				std::vector<std::pair<double, int>>  tvec;
				for (auto l : food)
					tvec.push_back(std::make_pair(dist2(l.x, l.y, x, y), l.z));
				std::sort(tvec.begin(), tvec.end());
				++bu[i][tvec[K - 1].second - 1];
			}
	}
	freopen("res.txt", "w", stdout);
	std::vector<int> res;
	res.resize(10);
	for (auto &i : bu)
		for (int j = 0; j < 10; j++)
			res[j] += i[j];
	for (auto i : res)
		printf("%.10f\n", i * 1.0 / 400000000);
}