#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include "Kernelinterface.hpp"
#include "Util.hpp"

int main()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	sf::RenderWindow window(sf::VideoMode(400, 200), "Kernel Driver User Mode", sf::Style::Close);

	sf::RectangleShape slider(sf::Vector2f(300.f, 10.f));
	slider.setPosition(50.f, 50.f);
	slider.setFillColor(sf::Color::Blue);

	sf::RectangleShape button(sf::Vector2f(100.f, 50.f));
	button.setPosition(50.f, 100.f);
	button.setFillColor(sf::Color::Red);


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);

					// Check if the button is clicked
					if (button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
					{
						// Perform action when the button is clicked
						// For example, you can update the kernel driver value
						std::cout << "Button Clicked!\n";
					}
				}
			}
		}

		// Update the valueText to show the slider value
		int sliderValue = static_cast<int>(slider.getSize().x);

		// Update the slider position based on mouse movement
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			if (slider.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
			{
				float newSliderWidth = static_cast<float>(mousePos.x - slider.getPosition().x);
				slider.setSize(sf::Vector2f(newSliderWidth, slider.getSize().y));
			}
		}

		window.clear();

		// Render
		window.draw(slider);
		window.draw(button);

		window.display();
	}

	return 0;
}
