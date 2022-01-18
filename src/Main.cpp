#include "Platform/Platform.hpp"

int main()
{
	util::Platform platform;
#if defined(_DEBUG)
	std::cout << "Hello World!" << std::endl;
#endif

	sf::RenderWindow window;
	float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
	window.create(sf::VideoMode(1500.0f * screenScalingFactor, 1000.0f * screenScalingFactor), "Fractals!");
	platform.setIcon(window.getSystemHandle());

	std::vector<sf::Vector2f> points = {
		sf::Vector2f(400.0f, 720.0f),
		sf::Vector2f(1100.0f, 720.0f)
	};

	float lineDistance = points[1].x - points[0].x;
	bool isDiagonal = false;

	sf::Vertex line[2];
	line[0].color = sf::Color::White;
	line[1].color = sf::Color::White;

	bool spaceWasPressed = false;
	bool enterWasPressed = false;
	bool render = true;

	bool doDragonCurve = true; // else it will do a Lévy C curve

	sf::Event event;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			if (!enterWasPressed)
			{
				points = {
					sf::Vector2f(400.0f, 720.0f),
					sf::Vector2f(1100.0f, 720.0f)
				};
				lineDistance = points[1].x - points[0].x;
				isDiagonal = false;
				doDragonCurve = !doDragonCurve;
				render = true;
			}
			enterWasPressed = true;
		}
		else
			enterWasPressed = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			if (!spaceWasPressed)
			{
				sf::Vector2f* previousPoint = &points[0];
				std::vector<sf::Vector2f> newPoints = { *previousPoint };
				float offsetDist = lineDistance / 2;

				sf::Vector2f offsets((isDiagonal) ? offsetDist / M_SQRT2 : 0.0f, (isDiagonal) ? offsetDist / M_SQRT2 : offsetDist);

				bool push = true;

				for (size_t i = 1; i < points.size(); i++)
				{
					const auto currentPoint = &points[i];
					const sf::Vector2f midPoint = (*currentPoint - *previousPoint) / 2.0f;
					sf::Vector2f newPoint((currentPoint->y < previousPoint->y) ? -offsets.x : offsets.x, (currentPoint->x >= previousPoint->x) ? -offsets.y : offsets.y);
					if (currentPoint->x == previousPoint->x) // swap x and y
					{
						const float xVal = newPoint.x;
						newPoint.x = (currentPoint->y < previousPoint->y) ? newPoint.y : -newPoint.y;
						newPoint.y = xVal;
					}
					if (!push)
					{
						newPoint.x = -newPoint.x;
						newPoint.y = -newPoint.y;
					}
					newPoints.push_back(newPoint + midPoint + *previousPoint);
					newPoints.push_back(*currentPoint);
					previousPoint = currentPoint;
					if (doDragonCurve)
						push = !push;
				}
				points = newPoints;
				isDiagonal = !isDiagonal;
				lineDistance = sqrt(2 * pow(lineDistance / 2, 2));
				render = true;
			}
			spaceWasPressed = true;
		}
		else
			spaceWasPressed = false;

		if (render)
		{
			render = false;
			window.clear();
			line[0].position = points[0];
			for (size_t i = 1; i < points.size(); i++)
			{
				const auto currentPoint = &points[i];
				line[1].position = *currentPoint;
				window.draw(line, 2, sf::Lines);
				line[0].position = line[1].position;
			}
			window.display();
		}
	}

	return 0;
}