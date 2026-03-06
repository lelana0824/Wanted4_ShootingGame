#pragma once
class Bounds
{
public:
	Bounds(int x, int y, int width = 1, int height = 1)
		: x(x), y(y), width(width), height(height) 
	{

	}
	
	// Getter
	inline int X() const { return x; }
	inline int Y() const { return y; }
	inline int Width() const { return width; }
	inline int Height() const { return height; }
	inline int MaxX() const { return x + width; }
	inline int MaxY() const { return y + height; }

	bool Intersects(const Bounds& other) const;
private:
	int x = 0;
	int y = 0;

	int width = 1;
	int height = 1;
};

