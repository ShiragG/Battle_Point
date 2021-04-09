#ifndef POSITION_H
#define POSITION_H
struct Position
{
	int x = 1;
	int y = 1;


	friend bool operator==(const Position& left, const Position& right)
	{
		if ((left.x == right.x) && (left.y == right.y))
			return true;
		else
			return false;
	}
};
#endif // !POSITION_H


