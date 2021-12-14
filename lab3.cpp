#include <iostream>
#include <limits>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <limits>
#include <numeric>
#include <functional>

#define MATRIX_SIZE 8
#define graphic(x) (1/x)

std::vector<double> sko(0);
double compare( std::vector<double> rec, std::vector<double> prev )
{
  std::vector<double> diff(MATRIX_SIZE, 0);

  for (int i = 0; i < MATRIX_SIZE; i++)
    diff[i] = rec[i] - prev[i];

  double sum = std::accumulate(diff.begin(), diff.end(), 0.0);
  double mean = sum / diff.size();

  std::vector<double> dif(diff.size());
  std::transform(diff.begin(), diff.end(), dif.begin(),
    std::bind2nd(std::minus<double>(), mean));

  double sq_sum = std::inner_product(dif.begin(), dif.end(), dif.begin(), 0.0);
  
  sko.push_back(std::sqrt(sq_sum / diff.size()));
  return sko.end();
}

void answer(std::vector<double> &vecpos, std::vector<std::vector<double>> matrix, double epsilon, int step)
{
  std::vector<double> prev_vecpos(vecpos);

  prev_vecpos[0] = -std::numeric_limits<double>::max();

  for (int i = 0; i < step && compare(vecpos, prev_vecpos) > epsilon; i++)
  {
    prev_vecpos = std::vector<double>(vecpos);
    
    std::vector<double> new_vec(MATRIX_SIZE, 0);
    for (int i = 0; i < MATRIX_SIZE; i++)
      for (int j = 0; j < MATRIX_SIZE; j++)
        new_vec[j] += vecpos[i] * matrix[i][j];

    vecpos = new_vec;
  }
}

std::vector<double> gauss(std::vector<std::vector<double>> a,  std::vector<double> y, int n)
{
  std::vector<double> x(n);
  double max;
  int k, index;
  const double eps = 0.00001;  // точность
  k = 0;

  while (k < n)
  {
    // Поиск строки с максимальным a[i][k]
    max = abs(a[k][k]);
    index = k;
    for (int i = k + 1; i < n; i++)
    {
      if (abs(a[i][k]) > max)
      {
        max = abs(a[i][k]);
        index = i;
      }
    }
    for (int j = 0; j < n; j++)
    {
      double temp = a[k][j];
      a[k][j] = a[index][j];
      a[index][j] = temp;
    }
    double temp = y[k];
    y[k] = y[index];
    y[index] = temp;
    for (int i = k; i < n; i++)
    {
      double temp = a[i][k];
      if (abs(temp) < eps) 
        continue;
      for (int j = 0; j < n; j++)
        a[i][j] = a[i][j] / temp;
      y[i] = y[i] / temp;
      if (i == k)  
        continue;
      for (int j = 0; j < n; j++)
        a[i][j] = a[i][j] - a[k][j];
      y[i] = y[i] - y[k];
    }
    k++;
  }

  for (k = n - 1; k >= 0; k--)
  {
    x[k] = y[k];
    for (int i = 0; i < k; i++)
      y[i] = y[i] - a[i][k] * x[k];
  }

  return x;
}

std::vector<double> answer(std::vector<std::vector<double>> matrix)
{
  for (int i = 0; i < MATRIX_SIZE; i++)
  {
    for (int j = i; j < MATRIX_SIZE; j++)
      std::swap(matrix[i][j], matrix[j][i]);
    matrix[i][i] -= 1;
  }
  matrix[MATRIX_SIZE - 1] = std::vector<double>(MATRIX_SIZE, 1);

  std::vector<double> ans(MATRIX_SIZE, 0);
  ans[MATRIX_SIZE - 1] = 1;
  
  return gauss(matrix, ans, MATRIX_SIZE);
}

int main(void)
{
  std::vector<std::vector<double>> matrix(MATRIX_SIZE, std::vector<double>(MATRIX_SIZE, 0));
  std::vector<double> vecpos = { 1, 0, 0, 0, 0, 0, 0, 0 };
  std::vector<double> sko;

  srand(15);

  for (int i = 0; i < MATRIX_SIZE; i++)
  {
    float sum = 0;
    for (auto &x : matrix[i])
    {
      auto rand_number = rand();
      sum += rand_number;
      x = rand_number;
    }
    for (auto &x : matrix[i])
      x /= sum;
  }
  
  answer(vecpos, matrix, 0.00001, 10);

  for (auto x : vecpos)
    std::cout << x << " ";
  std::cout << "Sum: " << std::accumulate(vecpos.begin(), vecpos.end(), 0.0) << std::endl;

  auto vec = answer(matrix);
  for (auto x : vec)
    std::cout << x << " ";
  std::cout << "Sum: " << std::accumulate(vecpos.begin(), vecpos.end(), 0.0) << std::endl;


  return 0;
}