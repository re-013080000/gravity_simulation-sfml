#include <SFML/Graphics.hpp>
#include <iostream>




			//characteristics of the sun
//-----------------------------------------------------------------------------------------------
class GravitySource {
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::CircleShape planet;
	float strength;

public:
	GravitySource(float x_location, float y_location, float strength, float x_velocity, float y_velocity) { //position x,y ; force of attraction ; velocity x,y
		position.x = x_location;
		position.y = y_location;
		velocity.x = x_velocity;
		velocity.y = y_velocity;
		this->strength = strength;

		planet.setPosition(position); //places the sun at the predefined coordinates
		planet.setFillColor(sf::Color::Yellow);
		planet.setRadius(20); //diameter of the sun: 40
	}

	void render(sf::RenderWindow& window) {
		window.draw(planet); //displays the sun on the screen
	}

	sf::Vector2f get_pos() {
		planet.setPosition(position);
		return position; //returns the position of the sun (x,y)
	}

	sf::FloatRect get_bd() {
		return planet.getGlobalBounds(); //return current hitbox
	}

	float get_strength() {
		return strength; //returns the attractiveness of the sun
	}

	void update_physics(GravitySource& planet) { //the sun advances at a constant speed
		position.x += velocity.x;
		position.y += velocity.y;
	}
};
//-----------------------------------------------------------------------------------------------




			//characteristics of the planets
//-----------------------------------------------------------------------------------------------
class Particle {
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::CircleShape planet;

public:
	Particle(float x_location, float y_location, float velocity_x, float velocity_y, int color) { //location x,y ; velocity x,y
		position.x = x_location;
		position.y = y_location;
		velocity.x = velocity_x;
		velocity.y = velocity_y;

		planet.setPosition(position); //places the planet at the predefined coordinates
		planet.setFillColor(sf::Color(85, color*1.8, color));
		planet.setRadius(4); //diameter of each planet: 4
	}

	void render(sf::RenderWindow& window) { //dispaly the planet
		planet.setPosition(position);
		window.draw(planet);
	}

	sf::FloatRect get_bd() {
		return planet.getGlobalBounds(); //return current hitbox
	}

	void burnt() {
		planet.setFillColor(sf::Color(0, 0, 0));
	}

	void update_physics(GravitySource& planet) { //updates the new coordinates of the planet
		float distance_x = planet.get_pos().x - position.x; //distance x between the sun and the planet
		float distance_y = planet.get_pos().y - position.y; //distance y between the sun and the planet

		float distance = sqrt(distance_x * distance_x + distance_y * distance_y); //Pythagorean theorem - shortest distance between the sun and the planet


					//inverse-square law  -  The gravitational attraction force between two point masses is directly proportional to the product of their masses and inversely proportional to the square of their separation distance
		//---------------------------------------------------
		float inverse_distance = 1.f / distance;

		float normalized_x = inverse_distance * distance_x;
		float normalized_y = inverse_distance * distance_y;

		float inverse_square = inverse_distance * inverse_distance;

		float acceleration_x = normalized_x * planet.get_strength() * inverse_square;
		float acceleration_y = normalized_y * planet.get_strength() * inverse_square;
		//---------------------------------------------------


		velocity.x += acceleration_x; //velocity x,y changing according to the distance between the sun and the planet
		velocity.y += acceleration_y;

		position.x += velocity.x; //new location / mouvment
		position.y += velocity.y;
	}
};
//-----------------------------------------------------------------------------------------------




int main() {
	srand(time(0));
	std::cout << R"(
        *******************************************************
                      __  _ ____ __  ___  __   __   __   __  
            _ _ ___  /  \/ |__ //  \( _ )/  \ /  \ /  \ /  \ 
           | '_/ -_)| () | ||_ \ () / _ \ () | () | () | () |
           |_| \___(_)__/|_|___/\__/\___/\__/ \__/ \__/ \__/   

        *******************************************************
)" << '\n';

	sf::RenderWindow window(sf::VideoMode(1600, 1200), "Gravity_Simulation");
	window.setFramerateLimit(60);

	GravitySource source(1300, 800, 7000, -0.5, -0.15); //setting up the sun | position_x, position_y, strength, velocity_x, velocity_y


	//create a certain amount of planets
	Particle* temp;
	std::vector<Particle*> Galaxy;

	for (int i = 0;i < 10000; i++) {
		int color = rand() % 255;
		if (color * 1.2 > 255) { //random color for planets
			color = color * 3 - 255;
		}
		int x_rand = rand() % 1600; //random position x,y for planets
		int y_rand = rand() % 1200;

		temp = new Particle(x_rand, y_rand, 3, -0.5f, color); //var temp for brand new planets
		Galaxy.push_back(temp); //planets stocked in the array/list/vector 'Galaxy'
	}


	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
		}

		window.clear();

		sf::FloatRect sun = source.get_bd(); //get the sun's hitbox

		source.update_physics(source);

		//display the planets at screen
		for (int i = 0; i < 10000; i++) {
			sf::FloatRect planet = Galaxy[i]->get_bd(); //get each planet's hitbox
			if (sun.intersects(planet)) { //delete the planet from the universe if that one touches the sun
				std::vector<Particle*> erase(i); //remove from the list
				Galaxy[i]->burnt(); //remove from the screen (paints it in black)
			}
			Galaxy[i]->update_physics(source);
			Galaxy[i]->render(window);
		}

		source.render(window);

		window.display();
	}

	return 0;
}
