#include <iostream>
#include <vector>
#include <functional>

class Symplex
{
public:
	std::vector<std::vector<double>> main_table;
	int m, n;
	bool max;
	std::vector<double> basis;
	std::vector<double> result;

	Symplex( std::vector<std::vector<double>> primer, bool max_ = true )
	{
		m = primer.size();
		n = primer[0].size();
		max = max_;

		main_table.resize(m, std::vector<double>(n + m - 1));
		basis = std::vector<double>();

		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < main_table[i].size(); j++)
			{
				if (j < n)
					main_table[i][j] = primer[i][j];
				else
					main_table[i][j] = 0;
			}
			
			if ((n + i) < main_table[i].size())
			{
				main_table[i][n + i] = 1;
				basis.push_back(n + i);
			}
		}

		n = main_table[0].size();
	}

	Symplex(std::vector<std::vector<double>> primer, std::vector<double> basis_, bool max_ = true)
	{
		m = primer.size();
		n = primer[0].size();
		max = max_;

		main_table.resize(m, std::vector<double>(n + m - 1));

		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < main_table[i].size(); j++)
			{
				if (j < n)
					main_table[i][j] = primer[i][j];
				else
					main_table[i][j] = 0;
			}
			
			if ((n + i) < main_table[i].size())
			{
				main_table[i][n + i] = basis_[basis.size()];
				basis.push_back(basis_[basis.size()]);
			}
		}

		n = main_table[0].size();
	}	

	
	std::vector<std::vector<double>> Calculate( int size )
	{
		int mainCol, mainRow; 

		while (!IsItEnd())
		{
			mainCol = findMainCol();
			mainRow = findMainRow(mainCol);
			basis[mainRow] = mainCol;

			std::vector<std::vector<double>> new_table(m, std::vector<double>(n));

			for (int j = 0; j < n; j++)
				new_table[mainRow][j] = main_table[mainRow][j] / main_table[mainRow][mainCol];

			for (int i = 0; i < m; i++)
			{
				if (i == mainRow)
					continue;

				for (int j = 0; j < n; j++)
					new_table[i][j] = main_table[i][j] - main_table[i][mainCol] * new_table[mainRow][j];
			}
			main_table = new_table;
		}
		result.resize(size);

		
		for (int i = 0; i < result.size(); i++)
		{
			auto x = std::find(basis.begin(), basis.end(), i + 1);
			if (x != basis.end())
				result[i] = main_table[std::distance(basis.begin(), x)][0];
			else
				result[i] = 0;
		}

		return main_table;
	}

	bool IsItEnd( void )
	{
		if (max)
		{
			for (int i = 1; i < n; i++)
				if (main_table[m - 1][i] < 0)
					return false;

			return true;
		}
		else
		{
			for (int i = 1; i < n; i++)
				if (main_table[m - 1][i] > 0)
					return false;

			return true;
		}
	}

	int findMainCol( void )
	{
		int mainCol = 1;

		for (int j = 2; j < n; j++)
			if (main_table[m - 1][j] < main_table[m - 1][mainCol])
				mainCol = j;

		return mainCol;
	}

	int findMainRow(int mainCol)
	{
		int mainRow = 0;

		for (int i = 0; i < m - 1; i++)
			if (main_table[i][mainCol] > 0)
			{
				mainRow = i;
				break;
			}

		for (int i = mainRow + 1; i < m - 1; i++)
			if ((main_table[i][mainCol] > 0) && ((main_table[i][0] / main_table[i][mainCol]) < (main_table[mainRow][0] / main_table[mainRow][mainCol])))
				mainRow = i;

		return mainRow;
	}
};

int main(void)
{
	std::vector<std::vector<double>> primer = { {0, -1,	1}, {-2, 0.5, -1.5}, {0, 1, 2} };
	Symplex symplex_method(primer, true);
	auto table_result = symplex_method.Calculate(4);

	for (auto x : symplex_method.result)
		std::cout << x << " ";
}
