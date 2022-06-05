#include <SFML/Graphics.hpp>
#include <iostream>




			//caractéristiques du soleil
//-----------------------------------------------------------------------------------------------
class GravitySource {
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::CircleShape planet;
	float strength;

public:
	GravitySource(float x_location, float y_location, float strength, float x_velocity, float y_velocity) { //position x,y ; force d'attraction ; vélocité x,y
		position.x = x_location;
		position.y = y_location;
		velocity.x = x_velocity;
		velocity.y = y_velocity;
		this->strength = strength;

		planet.setPosition(position); //place le soleil aux coordonnées prédéfinies
		planet.setFillColor(sf::Color::Yellow);
		planet.setRadius(20); //Diamètre du soleil: 40
	}

	void render(sf::RenderWindow& window) {
		window.draw(planet); //affiche le soleil à l'écran
	}

	sf::Vector2f get_pos() {
		planet.setPosition(position);
		return position; //renvoie la position du soleil (x,y)
	}

	float get_strength() {
		return strength; //renvoie la force d'attractivité du soleil
	}

	void update_physics(GravitySource& planet) { //le soleil avance d'une vitesse constante
		position.x += velocity.x;
		position.y += velocity.y;
	}
};
//-----------------------------------------------------------------------------------------------




			//caractéristiques des planètes
//-----------------------------------------------------------------------------------------------
class Particle {
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::CircleShape planet;

public:
	Particle(float x_location, float y_location, float velocity_x, float velocity_y) { //position x,y ; velocité x,y
		position.x = x_location;
		position.y = y_location;
		velocity.x = velocity_x;
		velocity.y = velocity_y;

		planet.setPosition(position); //place la planète aux coordonnées prédéfinies
		planet.setFillColor(sf::Color::Cyan);
		planet.setRadius(4); //diamètre de chaque planète: 4
	}

	void render(sf::RenderWindow& window) { //affiche la planète à l'écran
		planet.setPosition(position);
		window.draw(planet);
	}

	void update_physics(GravitySource& planet) { //met à jour les nouvelles coordonnées de la planète
		float distance_x = planet.get_pos().x - position.x; //distance x entre le soleil et la planète
		float distance_y = planet.get_pos().y - position.y; //distance y entre le soleil et la planète

		float distance = sqrt(distance_x * distance_x + distance_y * distance_y); //théorème de Pythagore - distance la plus courte entre le soleil et la planète


					//inverse-square law  -  La force d'attraction gravitationnelle entre deux masses ponctuelles est directement proportionnelle au produit de leurs masses et inversement proportionnelle au carré de leur distance de séparation
		//---------------------------------------------------
		float inverse_distance = 1.f / distance;

		float normalized_x = inverse_distance * distance_x;
		float normalized_y = inverse_distance * distance_y;

		float inverse_square = inverse_distance * inverse_distance;

		float acceleration_x = normalized_x * planet.get_strength() * inverse_square;
		float acceleration_y = normalized_y * planet.get_strength() * inverse_square;
		//---------------------------------------------------


		velocity.x += acceleration_x; //vélocité x,y changeant en fonction de la distance entre le soleil et la planète
		velocity.y += acceleration_y;

		position.x += velocity.x; //nouvelle position x,y de la planète
		position.y += velocity.y;
	}
};
//-----------------------------------------------------------------------------------------------




int main() {
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

	GravitySource source(1300, 800, 5000, -0.5, -0.15);
	Particle particle(1550, 700, -4, -2);
	Particle particle2(1450, 600, -2, -5);
	Particle particle3(1500, 550, 1, 3);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
		}

		window.clear();

		particle.update_physics(source);
		particle2.update_physics(source);
		particle3.update_physics(source);
		source.update_physics(source);

		source.render(window);
		particle.render(window);
		particle2.render(window);
		particle3.render(window);

		window.display();
	}

	return 0;
}
