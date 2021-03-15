struct Position
{
	Position(int _x,int _y)
	: x(_x)
	, y(_y)
	{}

	int x;
	int y;
};


class Square
{
public:
	Square(Position _position)
	: position(_position)
	{}
	Position GetPosition(){ return position;}

private:

	Position position;
};