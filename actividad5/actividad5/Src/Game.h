#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "SFMLRenderer.h"
#include <list>
#include "Avatar.h"

using namespace sf;
class Game
{
private:
	//Propiedades de la ventana
	int alto;
	int ancho;
	RenderWindow *wnd;
	Color clearColor;

	//objetos de box2d
	b2World *phyWorld;
	SFMLRenderer *debugRender;

	//tiempo de frame
	float frameTime;
	int fps;

	//cuerpo de box2d 
	b2Body* cabezaBody;
	Avatar* cabezaAvatar;

	b2Body* torsoBody;
	Avatar* torsoAvatar;

	b2Body* brazoIzqBody;
	Avatar* brazoIzqAvatar;

	b2Body* brazoDerBody;
	Avatar* brazoDerAvatar;

	b2Body* piernaIzqBody;
	Avatar* piernaIzqAvatar;

	b2Body* piernaDerBody;
	Avatar* piernaDerAvatar;

	Avatar* extremidadSeleccionada;
	bool SiguiendoMouse = false;

	b2DistanceJoint* cuello;

	b2DistanceJoint* hombroIzq;
	b2DistanceJoint* hombroDer;

	b2DistanceJoint* rodillaIzq;
	b2DistanceJoint* rodillaDer;

	sf::Texture texturaPelota;

	int velocidad = 10000;
		

public:

	//Constructores, destructores e inicializadores
	Game(int ancho, int alto,std::string titulo);
	void CreateEnemy(int x, int y);
	void InitPhysics();

	//Main game loop
	void Loop();
	void DrawGame();
	void UpdatePhysics();
	void DoEvents();
	void SetZoom();
	

	
};

