#ifndef WIDGET_H
#define WIDGET_H

struct Position
{
	Position(const int& _x, const int& _y) 
	: x(_x)
	, y(_y)
	{}

	int x, y;
};
/*
class Widget
{
	virtual void reactToEvent(Event event);
};*/

class RotatingCircleWidget
{
public:
	RotatingCircleWidget(int _posX, int _posY, float _radius, float _indicatorRadius, float _angularVelocity)
	: posX(_posX)
	, posY(_posY)
	, radius(_radius)
	, indicatorRadius(_indicatorRadius)
	, angularVelocity(_angularVelocity)
	, isStarted(false)
	, lastUpdateTime(std::chrono::system_clock::now())
	, startedTime(std::chrono::system_clock::now())
	{
		indicatorPosX = posX;
		indicatorPosY = posY + radius;
	}

	const int getPosX() const { return posX; }
	const int getPosY() const { return posY; }
	const float getRadius() const { return radius; }
	const int getIndicatorPosX() const { return indicatorPosX; }
	const int getIndicatorPosY() const { return indicatorPosY; }
	const float getIndicatorRadius() const { return indicatorRadius; }

	void setAngularVelocity(float _vel) { angularVelocity = _vel; }
	float getAngularVelocity() { return angularVelocity; }
	void addAngularVelocity(float _vel) { angularVelocity += _vel; }

	void start()
	{
		startedTime = std::chrono::system_clock::now();
		isStarted = true;
		lastUpdateTime = startedTime;
	}

	void update()
	{
		if(isStarted)
		{
			lastUpdateTime = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsedTime = lastUpdateTime - startedTime;
			indicatorPosX = posX + ( radius * cos( angularVelocity * elapsedTime.count() ) );
			indicatorPosY = posY + ( radius * sin( angularVelocity * elapsedTime.count() ) );
		}
	}

private:
	int posX, posY;
	int radius;
	int indicatorPosX, indicatorPosY;
	int indicatorRadius;
	float angularVelocity;
	bool isStarted;
	std::chrono::time_point<std::chrono::system_clock> lastUpdateTime;
	std::chrono::time_point<std::chrono::system_clock> startedTime;
};

class MultipointLineWidget
{
public:
	MultipointLineWidget(int _x, int _y, bool _drawLastPoint)
	: xPos(_x)
	, yPos(_y)
	, drawLastPoint(_drawLastPoint)
	{}

	int getPosX() const { return xPos; }
	int getPosY() const { return yPos; }
	void addPoint(const int& x, const int& y) { points.push_back( Position(x,y) ); }
	std::vector<Position> getPointsVector() const { return points; }
	bool getDrawLastPoint() const { return drawLastPoint; }
	void clear() { points.clear(); }

private:
	int xPos, yPos;
	std::vector<Position> points;
	bool drawLastPoint;
};

#endif