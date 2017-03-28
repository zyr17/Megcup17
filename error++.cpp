#include <cstdio>
#include <cassert>
#include <bitset>
#include <vector>
#include <ctime>
#define free fuckfree
int p[4][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
std::bitset<6666> A[6666];
bool b[6666];
bool map[2222][2222], res[2222][2222];
char inputs[4444444];
std::vector<std::bitset<6666>> equals[2222];
bool Const[2222][2222];
int n;
std::vector<std::vector<int>> free;
void gs(int n){
	for (int i = 0; i < n; i++){
		printf("i: %d\n", i);
		if (!A[i][i]){
			for (int j = i + 1; j < n; j++)
				if (A[j][i]){
					auto t = A[i];
					A[i] = A[j];
					A[j] = t;
					b[i] ^= b[j] ^= b[i] ^= b[j];
				}
			if (!A[i][i]){
				//printf("error get! %d\n", i);
				std::vector<int> tvec;
				tvec.push_back(i);
				free.push_back(tvec);
			}
		}
#pragma omp parallel for
		for (int j = 0; j < n; j++)
			if (j != i && A[j][i]){
				A[j] ^= A[i];
				b[j] ^= b[i];
			}
		/*for (int i = 0; i < n; i++){
			for (int j = 0; j < n; j++)
				printf("%d", int(A[i][j]));
			printf(" %d\n", b[i]);
		}
		printf("----------\n");*/
	}
	/*for (int i = n - 1; i >= 0; i--)
		for (int j = 0; j < i; j++)
			if (A[j][i])
				b[j] ^= b[i];*/
	for (auto &tvec : free)
		for (int i = 0; i < n; i++)
			if (A[i][tvec[0]])
				tvec.push_back(i);
}

int tryonce(std::vector<int> &b){
	int tot = 0;
	std::vector<std::vector<bool>> res;
	res.resize(2222);
	for (int i = 0; i < 2222; i++)
		res[i].resize(2222);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < n; j++)
			if (b[i * n + j]){
				tot++;
				res[i][j] = res[i][j] ^ 1;
				for (int k = 0; k < 4; k++)
					for (int l = 1; l < 4; l++){
						int x = i + p[k][0] * l, y = j + p[k][1] * l;
						if (x < 0 || x >= n) continue;
						if (y < 0 || y >= n) continue;
						res[x][y] = res[x][y] ^ 1;
					}
			}
	for (int i = 3; i < n; i++)
		for (int j = 0; j < n; j++)
			if (res[i - 3][j] != map[i - 3][j]){
				tot++;
				res[i][j] = res[i][j] ^ 1;
				for (int k = 0; k < 4; k++)
					for (int l = 1; l < 4; l++){
						int x = i + p[k][0] * l, y = j + p[k][1] * l;
						if (x < 0 || x >= n) continue;
						if (y < 0 || y >= n) continue;
						res[x][y] = res[x][y] ^ 1;
					}
			}
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (res[i][j] != map[i][j]){
				printf("ERROR!\n");
				return INT_MAX;
			}
	return tot;
}

int main(){
	freopen("eppreal.txt", "r", stdin);
	//freopen("t.out", "w", stdout);
	scanf("%d%s", &n, inputs);
	unsigned starttime = time(NULL);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			map[i][j] = inputs[i * n + j] == '1';
	for (int i = 0; i < 3; i++){
		equals[i].resize(2222);
		for (int j = 0; j < n; j++)
			equals[i][j][i * n + j] = 1;
	}
	for (int i = 3; i < n; i++){
		equals[i].resize(2222);
#pragma omp parallel for
		for (int j = 0; j < n; j++){
			int I = i - 3, J = j;
			Const[i][j] = map[I][J] ^ Const[I][J];
			equals[i][j] = equals[I][J];
			for (int k = 0; k < 4; k++)
				for (int l = 1; l < 4; l++){
					int x = I + p[k][0] * l, y = J + p[k][1] * l;
					if (x < 0 || x >= n) continue;
					if (y < 0 || y >= n) continue;
					if (x == i && y == j) continue;
					Const[i][j] = Const[i][j] ^ Const[x][y];
					equals[i][j] ^= equals[x][y];
				}
			/*printf("%d %d: ", i, j);
			for (int ii = 0; ii < n * 3; ii++){
				printf("%d", int(equals[i][j][ii]));
				if ((ii + 1) % n == 0) printf("\n     ");
			}
			printf(" %d\n", int(Const[i][j]));*/
		}
		//if (i > 7) equals[i - 7].clear();
	}
	int nowA = 0;
	for (int i = n - 3; i < n; i++)
		for (int j = 0; j < n; j++){
			A[nowA] = equals[i][j];
			b[nowA] = Const[i][j] ^ map[i][j];
			for (int k = 0; k < 4; k++)
				for (int l = 1; l < 4; l++){
					int x = i + p[k][0] * l, y = j + p[k][1] * l;
					if (x < 0 || x >= n) continue;
					if (y < 0 || y >= n) continue;
					b[nowA] ^= Const[x][y];
					A[nowA] ^= equals[x][y];
				}
			nowA++;
		}
	gs(n * 3);

	std::vector<int> min;
	min.resize(1 << free.size());

#pragma omp parallel for
	for (int i = 0; i < 1 << free.size(); i++){
		std::vector<int> tb;
		for (int j = 0; j < n * 3; j++)
			tb.push_back(b[j]);
		for (int k = 0, l = i; l; k++, l /= 2)
			if (l % 2)
				for (auto q : free[k])
					tb[q] ^= 1;
		min[i] = tryonce(tb);
		printf("try %d: %d\n", i, min[i]);
	}

	int allmin = INT_MAX;
	for (auto i : min)
		if (i < allmin)
			allmin = i;
	
	freopen("res.txt","w",stdout);
	printf("%d\n%d", time(NULL) - starttime, allmin);
}