// IsingHeisenberg_triangle.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <string> 
#include <vector>
#include <math.h>
#include <cassert>

#include "spin.h"

#undef square
#define triangle

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
const int Number = 500;
const int Loops = 10, Steps = 400000;
const int Thermalization = 100000;
float T, J, h, Jrow;

spin S[Number][Number];
int MaxX, MaxY ;
std::vector<float> Magnatization;
std::vector<float> MagnatizationSquare;
float Anisotropy = 0;

double Magnetization(int* S)
{
	int M = 0;
	int i;
	for (i = 0; i < Number; i++)
	{
		M += S[i];
		//		std::cout << "   "<< S[i] << "    "<< "\n";
	}
	return M;
}

double GetMagnetization()
{
	double M = 0;
	for (int i = 0; i < MaxX; i++)
	{
		for (int j = 0; j < MaxY; j++)
		{
			M += S[i][j].Sz;
		//	std::cout << S[i][j].Sz << '\t';
		}
	}

	double XY = MaxX * MaxY;
	M /= XY;

	return (M);
}

double GetMagnetizationSquare()
{
	double Msq = 0;
	for (int i = 0; i < MaxX; i++)
	{
		for (int j = 0; j < MaxY; j++)
		{
			Msq += S[i][j].Sz * S[i][j].Sz;
		}
	}
	double XY = MaxX * MaxY;


	Msq /= XY;
	//	UE_LOG(LogTemp, Warning, TEXT("Msq: %f, -----f"), Msq);
	return (Msq);
}

void TextOut(const std::string & Text)
{
	// #include <stdio.h>

	FILE* stream;
	char Stri[25];

	stream =
		fopen("Data.txt", "a");

	//  "Data.txt"
	char* p;
	p = (char*)Text.c_str();
	if (stream != NULL)
	{
		fprintf(stream, p, "DataEnh.txt");

		fclose(stream);
	}
	return;
}

void  EraseAllSpins()
{
	int X, Y;
	for (X = 1; X < MaxX + 1; X++)
	{
		for (Y = 1; Y < MaxY + 1; Y++)
		{
			S[X - 1][Y - 1].reset();
		}
	}

}

double EnergyCalc(const spin& Spin, int X, int Y)
{
	//!!! May not work correctly due to issue that Spins are fail to turn on time
	//needs to be checked
	double Energy = 0;

	assert(X >= 1 && X <= MaxX);
	assert(Y >= 1 && Y <= MaxY);

#ifdef square

	// square

	if (X < MaxX)
	{
		Energy += J * Spin.Sz * S[X][Y - 1].Sz * 0.25;
		Energy += J * Spin.Sx * S[X][Y - 1].Sx * 0.25;
		Energy += J * Spin.Sy * S[X][Y - 1].Sy * 0.25;

	}
	if (Y < MaxY)
	{
		Energy += J * Spin.Sz * S[X - 1][Y].Sz * 0.25;
		Energy += J * Spin.Sx * S[X - 1][Y].Sx * 0.25;
		Energy += J * Spin.Sy * S[X - 1][Y].Sy * 0.25;
	}
	if (X > 1)
	{
		Energy += J * Spin.Sz * S[X - 2][Y - 1].Sz * 0.25;
		Energy += J * Spin.Sx * S[X - 2][Y - 1].Sx * 0.25;
		Energy += J * Spin.Sy * S[X - 2][Y - 1].Sy * 0.25;
	}
	if (Y > 1)
	{
		Energy += J * Spin.Sz * S[X - 1][Y - 2].Sz * 0.25;
		Energy += J * Spin.Sx * S[X - 1][Y - 2].Sx * 0.25;
		Energy += J * Spin.Sy * S[X - 1][Y - 2].Sy * 0.25;
	}

	Energy += -h * Spin.Sz * 0.5 - Anisotropy * Spin.Sz * Spin.Sz * 0.25;

#endif square

#ifdef triangle

	enum SpinPosition
	{

		FirstTriangleLeft,
		FirstTriangleRight,
		FirstTriangleUp,
		SecondTriangleLeft,
		SecondTriangleRight,
		SecondTriangleDown,
		NullPos
	};
	SpinPosition Position = NullPos;


	if (Y % 2 == 1)
	{
		switch ((X - 1) % 6)
		{
		case 0:
		{
			Position = FirstTriangleLeft;
		}break;
		case 1:
		{
			Position = FirstTriangleUp;
		}break;
		case 2:
		{
			Position = FirstTriangleRight;
		}break;
		case 3:
		{
			Position = SecondTriangleLeft;
		}break;
		case 4:
		{
			Position = SecondTriangleDown;
		}break;
		case 5:
		{
			Position = SecondTriangleRight;
		}break;
		}
	}
	if (Y % 2 == 0)
	{
		switch ((X - 1) % 6)
		{
		case 0:
		{
			Position = SecondTriangleLeft;
		}break;
		case 1:
		{
			Position = SecondTriangleDown;
		}break;
		case 2:
		{
			Position = SecondTriangleRight;
		}break;
		case 3:
		{
			Position = FirstTriangleLeft;
		}break;
		case 4:
		{
			Position = FirstTriangleUp;
		}break;
		case 5:
		{
			Position = FirstTriangleRight;
		}break;
		}
	}

	if (X < MaxX)
	{
		Energy += J * Spin.Sz * S[X][Y - 1].Sz * 0.25;
		Energy += J * Spin.Sx * S[X][Y - 1].Sx * 0.25;
		Energy += J * Spin.Sy * S[X][Y - 1].Sy * 0.25;

	}

	if (X > 1)
	{
		Energy += J * Spin.Sz * S[X - 2][Y - 1].Sz * 0.25;
		Energy += J * Spin.Sx * S[X - 2][Y - 1].Sx * 0.25;
		Energy += J * Spin.Sy * S[X - 2][Y - 1].Sy * 0.25;
	}

	switch (Position)
	{
	case (FirstTriangleUp):
	{
		if ((Y < MaxY))
		{
			Energy += Jrow * Spin.Sz * S[X - 1][Y].Sz * 0.25;
			Energy += Jrow * Spin.Sx * S[X - 1][Y].Sx * 0.25;
			Energy += Jrow * Spin.Sy * S[X - 1][Y].Sy * 0.25;
		}
	}break;
	case (SecondTriangleDown):
	{
		if (Y > 1)
		{

			Energy += Jrow * Spin.Sz * S[X - 1][Y - 2].Sz * 0.25;
			Energy += Jrow * Spin.Sx * S[X - 1][Y - 2].Sx * 0.25;
			Energy += Jrow * Spin.Sy * S[X - 1][Y - 2].Sy * 0.25;
		}
	}break;
	case FirstTriangleLeft:
	{
		if ((X < MaxX - 1))
		{
			Energy += J * Spin.Sz * S[X + 1][Y - 1].Sz * 0.25;
			Energy += J * Spin.Sx * S[X + 1][Y - 1].Sx * 0.25;
			Energy += J * Spin.Sy * S[X + 1][Y - 1].Sy * 0.25;
		}
	}break;
	case FirstTriangleRight:
	{
		if ((X > 2))
		{
			Energy += J * Spin.Sz * S[X - 3][Y - 1].Sz * 0.25;
			Energy += J * Spin.Sx * S[X - 3][Y - 1].Sx * 0.25;
			Energy += J * Spin.Sy * S[X - 3][Y - 1].Sy * 0.25;
		}
	}break;
	case SecondTriangleLeft:
	{
		if ((X < MaxX - 1))
		{
			Energy += J * Spin.Sz * S[X + 1][Y - 1].Sz * 0.25;
			Energy += J * Spin.Sx * S[X + 1][Y - 1].Sx * 0.25;
			Energy += J * Spin.Sy * S[X + 1][Y - 1].Sy * 0.25;
		}
	}break;
	case SecondTriangleRight:
	{
		if ((X > 2))
		{
			Energy += J * Spin.Sz * S[X - 3][Y - 1].Sz * 0.25;
			Energy += J * Spin.Sx * S[X - 3][Y - 1].Sx * 0.25;
			Energy += J * Spin.Sy * S[X - 3][Y - 1].Sy * 0.25;
		}
	}break;
	case NullPos:
	{
		assert(false);
	}break;
	}



	Energy += -h * Spin.Sz * 0.5 - Anisotropy * Spin.Sz * Spin.Sz * 0.25;
#endif triangle
	return Energy;

}

double EnergyCalcTotal()
{
	int X = 0, Y = 0;
	double Energy = 0;
	for (X = 1; X < MaxX + 1; X++)
	{
		for (Y = 1; Y < MaxY + 1; Y++)
		{
			Energy += EnergyCalc(S[X - 1][Y - 1], X, Y);
		}
	}
	return  Energy;
}

int ConvertRowToNumber(int Column, int Row)
{
	int rez;

	rez = ((Row - 1) * MaxX) + (Column - 1); /// is result correct??
	return rez;
}

void CalculateRawAndColumn(int Number, int& Column, int& Row)
{
	Row = Number / MaxX + 1;
	Column = Number % MaxX + 1;
}

int main(int argc, char** argv)
{
	T = 0.1;
	J = 1;
	Jrow = 2;
	h = 0.0;
	Anisotropy = 0;
	MaxX = 500, MaxY = 2;
	// initialization
	int i, j;
	EraseAllSpins();

	// MC Steps
	while (h < 3)
	{

		double M = 0;
		for (i = 0; i < Loops; i++)
		{
			double Mtemp = 0;
			int Measurements = 0;
			int SpinNumber;
			int Actors = MaxX * MaxY;
			int X = 0, Y = 0;

			EraseAllSpins();

			for (j = 0; j < Steps; j++)
			{
				SpinNumber = (int) (drand48() * (Actors));
				CalculateRawAndColumn(SpinNumber, X, Y);

				spin NewSpin, TempSpin;
				NewSpin.reset();


				double E_old, E_new;
				E_old = EnergyCalc(S[X - 1][Y - 1], X, Y);
				E_new = EnergyCalc(NewSpin, X, Y); 

/*
				TempSpin = S[X - 1][Y - 1];
				S[X - 1][Y - 1] = NewSpin;
				

				if (E_old < E_new)
				{
					double W = drand48();
					if (exp(-(E_new - E_old) / T) > W)
					{
					 
					}
					else
					{
						S[X - 1][Y - 1] = TempSpin;
					}
				}*/
				


				if (E_old >= E_new)
				{
					S[X - 1][Y - 1] = NewSpin;
				}
				else
				{
					double W = drand48();
					double P = exp(-(E_new - E_old) / T);
					if (P > W && P!=1)
					{
						 S[X - 1][Y - 1] = NewSpin;
					}
				}/**/

				if ((j > Thermalization) && (j % Thermalization == 0))
				{
					Mtemp += GetMagnetization();
					Measurements += 1;
				}
			}
			if (Measurements != 0)
			{
				Mtemp /= Measurements;
				Measurements = 0;
				M += Mtemp;
				Mtemp = 0;
			}

		}
		M /= Loops;
		M /= 2;
		std::cout << M << "\n";
		std::string Result = std::to_string(h) + "   " + std::to_string(M) + " \n";

		TextOut(Result);
		h += 0.1;
	}

	return 0;
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
