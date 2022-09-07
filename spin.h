#pragma once
#include  <cstdlib>
#define M_PI       3.14159265358979323846
#define WIN

#ifdef WIN
double drand48()
{
	return rand() / (RAND_MAX + 1.0);
}
#endif 

class spin
{
public:
	enum ModeType
	{
		Ising,
		Heisenberg
	};

	static const ModeType Mode = Ising;
	double Sz, Sx, Sy;

	spin()
	{
		reset();
	};

	void reset()
	{
		float Fi, cosTeta, Teta;
		Fi = drand48()*360;
		cosTeta = 1-2*drand48();

		Teta = 180 / M_PI * acos(cosTeta); //radians to grad
		if (Mode==Ising)
		{
			Teta = 180 * ((int) (2*drand48()));
		}
		Sz = cos(Teta * M_PI / 180);
		Sx = sin(Teta * M_PI / 180) * cos(Fi * M_PI / 180);
		Sy = sin(Teta * M_PI / 180) * sin(Fi * M_PI / 180);
	}
};

class FVector
{
public:
	float X, Y, Z;
	FVector(float x, float y, float z):
		X(x), Y(y), Z(z)
	{}
	FVector()
	{
		X = 0;
		Y = 0;
		Z = 0;
	}
	FVector operator += (const FVector& rhs)
	{
		this->X += rhs.X;
		this->Y += rhs.Y;
		this->Z += rhs.Z;

		return *this;
	}
};