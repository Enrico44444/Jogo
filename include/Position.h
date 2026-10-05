#ifndef POSITION_H
#define POSITION_H

class Position
{
private:
	int x;
	int y;

public:
	void move(int dx, int dy);
	Position nextPos(char direction);
	bool equals(Position other);

};

#endif
