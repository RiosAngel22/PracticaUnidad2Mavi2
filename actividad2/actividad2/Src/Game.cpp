#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
    // Inicializaci�n de la ventana de renderizado
    wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
    wnd->setVisible(true);
    fps = 60;
    wnd->setFramerateLimit(fps);
    frameTime = 1.0f / fps;
    SetZoom(); // Configuraci�n del zoom de la c�mara
    InitPhysics(); // Inicializaci�n del mundo f�sico
}

// M�todo principal que maneja el bucle del juego
void Game::Loop()
{
    while (wnd->isOpen())
    {
        wnd->clear(clearColor); // Limpia la ventana con un color especificado
        DoEvents(); // Procesa los eventos del sistema
        UpdatePhysics(); // Actualiza la simulaci�n f�sica
        DrawGame(); // Dibuja el juego en la ventana
        wnd->display(); // Muestra la ventana renderizada
    }
}

// Actualiza la simulaci�n f�sica
void Game::UpdatePhysics()
{
    phyWorld->Step(frameTime, 8, 8); // Avanza la simulaci�n f�sica
    phyWorld->ClearForces(); // Limpia las fuerzas aplicadas a los cuerpos
    phyWorld->DebugDraw(); // Dibuja el mundo f�sico (para depuraci�n)
}

// Dibuja los elementos del juego en la ventana
void Game::DrawGame()
{
    // Dibujamos el suelo
    sf::RectangleShape groundShape(sf::Vector2f(500, 5));
    groundShape.setFillColor(sf::Color::Red);
    groundShape.setPosition(0, 95);
    wnd->draw(groundShape);

    // Dibujamos las paredes
    sf::RectangleShape leftWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
    leftWallShape.setFillColor(sf::Color::Red);
    leftWallShape.setPosition(100, 0); // X = 100 para que comience donde termina el suelo
    wnd->draw(leftWallShape);

    sf::RectangleShape rightWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
    rightWallShape.setFillColor(sf::Color::Red);
    rightWallShape.setPosition(90, 0); // X = 90 para que comience donde termina el suelo
    wnd->draw(rightWallShape);

    sf::RectangleShape ceilingShape(sf::Vector2f(500, 5));
    ceilingShape.setFillColor(sf::Color::Red);
    ceilingShape.setPosition(0, 0);
    wnd->draw(ceilingShape);


    controlBodyAvatar->Actualizar(); // Actualiza la posici�n del avatar
    controlBodyAvatar->Dibujar(*wnd); // Dibuja el avatar en la ventana

    controlBodyAvatar2->Actualizar();
    controlBodyAvatar2->Dibujar(*wnd);
}

// Procesa los eventos del sistema
void Game::DoEvents()
{
    Event evt;
    while (wnd->pollEvent(evt))
    {
        switch (evt.type)
        {
        case Event::Closed:
            wnd->close(); // Cierra la ventana
            break;
        }
        if (Mouse::isButtonPressed(Mouse::Left)) {
            if (controlBodyAvatar->checkearClick(*wnd)) {
                controlBodyAvatar->SetPosition(b2Vec2(Mouse::getPosition(*wnd).x * 0.125, Mouse::getPosition(*wnd).y * 0.17));
                break;
            }
            if (controlBodyAvatar2->checkearClick(*wnd)) {
                controlBodyAvatar2->SetPosition(b2Vec2(Mouse::getPosition(*wnd).x * 0.125, Mouse::getPosition(*wnd).y * 0.17));
                break;
            }
        }
        
        
    
    
    }
}

// Configura el �rea visible en la ventana de renderizado
void Game::SetZoom()
{
    View camara;
    camara.setSize(100.0f, 100.0f); // Tama�o del �rea visible
    camara.setCenter(50.0f, 50.0f); // Centra la vista en estas coordenadas
    wnd->setView(camara); // Asigna la vista a la ventana
}

// Inicializa el mundo f�sico y los elementos est�ticos del juego
void Game::InitPhysics()
{
    // Inicializa el mundo f�sico con la gravedad por defecto
    phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

    // Inicializa el renderizador de depuraci�n para el mundo f�sico
    debugRender = new SFMLRenderer(wnd);
    debugRender->SetFlags(UINT_MAX); // Configura el renderizado para que muestre todo
    phyWorld->SetDebugDraw(debugRender);

    // Crea los elementos est�ticos del juego (suelo y paredes)
    b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
    groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

    b2Body* ceilingBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
    ceilingBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);
  
    b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

    b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

    // Carga la textura de la pelota para el avatar
    texturaPelota.loadFromFile("Pelota.png");

    // Crea un cuerpo de c�rculo controlado por el teclado
    controlBody = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 5, 0, 1);
    controlBody->SetTransform(b2Vec2(25.0f, 25.0f), 0.0f);

    // Inicializa pelota
    controlBodyAvatar = new Avatar(controlBody, new sf::Sprite(texturaPelota));

    controlBody2 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 5, 0, 1);
    controlBody2->SetTransform(b2Vec2(25.0f, 50.0f), 0.0f);

    //Inicializa pelota2
    controlBodyAvatar2 = new Avatar(controlBody2, new sf::Sprite(texturaPelota));

    Resorte = Box2DHelper::CreateDistanceJoint(phyWorld, controlBody, controlBody->GetWorldCenter() , controlBody2, controlBody2->GetWorldCenter(),0,20,0.2);
}

