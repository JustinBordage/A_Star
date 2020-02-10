#pragma once
#include <string>
#include <iostream>

class Vector2Int
{
public:
	int x;
	int y;

	Vector2Int()
	{
		x = 0;
		y = 0;
	}

	Vector2Int(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	Vector2Int operator+(const Vector2Int& other) const
	{
		//Adds this vector's x with the other vector's x
		//and performs the same exact operation for the y axis
		return Vector2Int(x + other.x, y + other.y);
	}

	Vector2Int operator-(const Vector2Int& other) const
	{
		//Subtracts this vector's x by the other vector's x
		//and performs the same exact operation for the y axis
		return Vector2Int(x - other.x, y - other.y);
	}

	Vector2Int operator*(const Vector2Int& other) const
	{
		//Multiplies this vector's x with the other vector's x
		//and performs the same exact operation for the y axis
		return Vector2Int(x * other.x, y * other.y);
	}

	Vector2Int operator/(const Vector2Int& other) const
	{
		//Divides this vector's x by the other vector's x
		//and performs the same exact operation for the y axis
		return Vector2Int(x / other.x, y / other.y);
	}

	bool operator==(const Vector2Int& other) const
	{
		//Checks if both the x and y values are the same
		return (x == other.x && y == other.y);
	}

	bool operator!=(const Vector2Int& other) const
	{
		//Checks if both the x and y values are the same
		return (x != other.x || y != other.y);
	}
};

class Vector2Short
{
public:
	short x;
	short y;

	Vector2Short()
	{
		x = 0;
		y = 0;
	}

	Vector2Short(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	Vector2Short operator+(const Vector2Short& other) const
	{
		//Adds this vector's x with the other vector's x
		//and performs the same exact operation for the y axis
		return Vector2Short(x + other.x, y + other.y);
	}

	Vector2Short operator-(const Vector2Short& other) const
	{
		//Subtracts this vector's x by the other vector's x
		//and performs the same exact operation for the y axis
		return Vector2Short(x - other.x, y - other.y);
	}

	Vector2Short operator*(const Vector2Short& other) const
	{
		//Multiplies this vector's x with the other vector's x
		//and performs the same exact operation for the y axis
		return Vector2Short(x * other.x, y * other.y);
	}

	Vector2Short operator/(const Vector2Short& other) const
	{
		//Divides this vector's x by the other vector's x
		//and performs the same exact operation for the y axis
		return Vector2Short(x / other.x, y / other.y);
	}

	bool operator==(const Vector2Short& other) const
	{
		//Checks if both the x and y values are the same
		return (x == other.x && y == other.y);
	}

	bool operator!=(const Vector2Short& other) const
	{
		//Checks if both the x and y values are the same
		return (x != other.x || y != other.y);
	}
};

/*std::ostream& operator<<(std::ostream& stream, const Vector2Int& other)
{
	//Prints out each of the datamembers as a list
	//"Element 1, Element 2, Element 3, etc."
	stream << other.x << ", " << other.y;
	return stream;
}*/