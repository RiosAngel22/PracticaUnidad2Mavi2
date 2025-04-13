#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
    // Inicialización de la ventana de renderizado
    wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
    wnd->setVisible(true);
    fps = 60;
    wnd->setFramerateLimit(fps);
    frameTime = 1.0f / fps;
    SetZoom(); // Configuración del zoom de la cámara
    InitPhysics(); // Inicialización del mundo físico
}

// Método principal que maneja el bucle del juego
void Game::Loop()
{
    while (wnd->isOpen())
    {
        wnd->clear(clearColor); // Limpia la ventana con un color especificado
        DoEvents(); // Procesa los eventos del sistema
        UpdatePhysics(); // Actualiza la simulación física
        DrawGame(); // Dibuja el juego en la ventana
        wnd->display(); // Muestra la ventana renderizada
    }
}

// Actualiza la simulación física
void Game::UpdatePhysics()
{
    phyWorld->Step(frameTime, 8, 8); // Avanza la simulación física
    phyWorld->ClearForces(); // Limpia las fuerzas aplicadas a los cuerpos
    phyWorld->DebugDraw(); // Dibuja el mundo físico (para depuración)
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


    cabezaAvatar->Actualizar();
    cabezaAvatar->Dibujar(*wnd);

    torsoAvatar->Actualizar();
    torsoAvatar->Dibujar(*wnd);

    brazoIzqAvatar->Actualizar();
    brazoIzqAvatar->Dibujar(*wnd);

    brazoDerAvatar->Actualizar();
    brazoDerAvatar->Dibujar(*wnd);

    piernaIzqAvatar->Actualizar();
    piernaIzqAvatar->Dibujar(*wnd);

    piernaDerAvatar->Actualizar();
    piernaDerAvatar->Dibujar(*wnd);

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

        case Event::MouseButtonPressed:
            if (Mouse::isButtonPressed(Mouse::Left)) {
                if (cabezaAvatar->checkearClick(*wnd)) {
                    extremidadSeleccionada = cabezaAvatar;
                    SiguiendoMouse = true;
                    break;
                }
                if (torsoAvatar->checkearClick(*wnd)) {
                    extremidadSeleccionada = torsoAvatar;
                    SiguiendoMouse = true;
                    break;
                }
                if (piernaIzqAvatar->checkearClick(*wnd)) {
                    extremidadSeleccionada = piernaIzqAvatar;
                    SiguiendoMouse = true;
                    break;
                }
                if (piernaDerAvatar->checkearClick(*wnd)) {
                    extremidadSeleccionada = piernaDerAvatar;
                    SiguiendoMouse = true;
                    break;
                }
                if (brazoIzqAvatar->checkearClick(*wnd)) {
                    extremidadSeleccionada = brazoIzqAvatar;
                    SiguiendoMouse = true;
                    break;
                }
                if (brazoDerAvatar->checkearClick(*wnd)) {
                    extremidadSeleccionada = brazoDerAvatar;
                    SiguiendoMouse = true;
                    break;
                }
            }

        case Event::MouseButtonReleased:
            SiguiendoMouse = false;
            break;
        }

        if (SiguiendoMouse == true) {
            extremidadSeleccionada->SetPosition(b2Vec2(Mouse::getPosition(*wnd).x * 0.125, Mouse::getPosition(*wnd).y * 0.17));
        }

        //alzamos al ragdoll
        if (Keyboard::isKeyPressed(Keyboard::Q)) {
            cabezaAvatar->AplicarImpulso(b2Vec2(0.0f, -1000.0f),b2Vec2(cabezaBody->GetWorldCenter()));
            torsoAvatar->AplicarImpulso(b2Vec2(0.0f, -1000.0f), b2Vec2(cabezaBody->GetWorldCenter()));
        }

        //lo movemos con las teclas
        if (Keyboard::isKeyPressed(Keyboard::W)) {
            cabezaAvatar->AplicarFuerza(b2Vec2(0.0f, -velocidad), b2Vec2(cabezaBody->GetWorldCenter()));
        }
        if (Keyboard::isKeyPressed(Keyboard::S)) {
            cabezaAvatar->AplicarFuerza(b2Vec2(0.0f, velocidad), b2Vec2(cabezaBody->GetWorldCenter()));
        }
        if (Keyboard::isKeyPressed(Keyboard::D)) {
            cabezaAvatar->AplicarFuerza(b2Vec2(velocidad, 0.0f), b2Vec2(cabezaBody->GetWorldCenter()));
        }
        if (Keyboard::isKeyPressed(Keyboard::A)) {
            cabezaAvatar->AplicarFuerza(b2Vec2(-velocidad, 0.0f), b2Vec2(cabezaBody->GetWorldCenter()));
        }
    }
}

// Configura el área visible en la ventana de renderizado
void Game::SetZoom()
{
    View camara;
    camara.setSize(100.0f, 100.0f); // Tamaño del área visible
    camara.setCenter(50.0f, 50.0f); // Centra la vista en estas coordenadas
    wnd->setView(camara); // Asigna la vista a la ventana
}

// Inicializa el mundo físico y los elementos estáticos del juego
void Game::InitPhysics()
{    

    // Inicializa el mundo físico con la gravedad por defecto
    phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

    // Inicializa el renderizador de depuración para el mundo físico
    debugRender = new SFMLRenderer(wnd);
    debugRender->SetFlags(UINT_MAX); // Configura el renderizado para que muestre todo
    phyWorld->SetDebugDraw(debugRender);

    // Crea los elementos estáticos del juego (suelo y paredes)
    b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
    groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

    b2Body* ceilingBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
    ceilingBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);
  
    b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

    b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

    texturaPelota.loadFromFile("rectangulo.png");


    // creamos las extremidades

    //cabeza---------------------------------------------------------------------
    cabezaBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 5, 6, 5,1,1);
    cabezaBody->SetTransform(b2Vec2(50.0f, 25.5f), 0.0f);

    cabezaAvatar = new Avatar(cabezaBody, new sf::Sprite(texturaPelota));


    //torso------------------------------------------------------------------------
    torsoBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 8, 12, 5, 1, 0);
    torsoBody->SetTransform(b2Vec2(50.0f, 35.0f), 0.0f);

    torsoAvatar = new Avatar(torsoBody, new sf::Sprite(texturaPelota));
    
    //brazos-------------------------------------------------------------------------
    brazoIzqBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2, 10, 2, 1, 0.5);
    brazoIzqBody->SetTransform(b2Vec2(45.0f, 35.0f),0.0f);

    brazoIzqAvatar = new Avatar(brazoIzqBody, new sf::Sprite(texturaPelota));

    brazoDerBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2, 10, 2, 1, 0.5);
    brazoDerBody->SetTransform(b2Vec2(55.0f, 35.0f), 0.0f);

    brazoDerAvatar = new Avatar(brazoDerBody, new sf::Sprite(texturaPelota));
    


    //piernas----------------------------------------------------------------------------
    piernaIzqBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 12, 2, 1, 0.5);
    piernaIzqBody->SetTransform(b2Vec2(48.0f, 45.0f), 0.0f);

    piernaIzqAvatar = new Avatar(piernaIzqBody, new sf::Sprite(texturaPelota));

    piernaDerBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 12, 2, 1, 0.5);
    piernaDerBody->SetTransform(b2Vec2(52.0f, 45.0f), 0.0f);

    piernaDerAvatar = new Avatar(piernaDerBody, new sf::Sprite(texturaPelota));

    //creamos los resortes
    cuello = Box2DHelper::CreateDistanceJoint(phyWorld, cabezaBody,b2Vec2(cabezaBody->GetWorldCenter().x,cabezaBody->GetWorldCenter().y+3), torsoBody, b2Vec2(torsoBody->GetWorldCenter().x, torsoBody->GetWorldCenter().y - 5), 1.0f, 5.0f, 0.5f);

    hombroIzq = Box2DHelper::CreateDistanceJoint(phyWorld, brazoIzqBody, b2Vec2(brazoIzqBody->GetWorldCenter().x, brazoIzqBody->GetWorldCenter().y - 5),torsoBody,b2Vec2(torsoBody->GetWorldCenter().x-4,torsoBody->GetWorldCenter().y - 6), 0.0f, 15.0f, 2.0f);
    hombroDer = Box2DHelper::CreateDistanceJoint(phyWorld, brazoDerBody, b2Vec2(brazoDerBody->GetWorldCenter().x, brazoDerBody->GetWorldCenter().y - 5), torsoBody, b2Vec2(torsoBody->GetWorldCenter().x+4, torsoBody->GetWorldCenter().y - 6), 0.0f, 15.0f, 2.0f);

    rodillaIzq = Box2DHelper::CreateDistanceJoint(phyWorld, piernaIzqBody, b2Vec2(piernaIzqBody->GetWorldCenter().x, piernaIzqBody->GetWorldCenter().y - 5), torsoBody, b2Vec2(torsoBody->GetWorldCenter().x-2, torsoBody->GetWorldCenter().y + 6), 0.0f, 5.0f, 2.0f);
    rodillaDer = Box2DHelper::CreateDistanceJoint(phyWorld, piernaDerBody, b2Vec2(piernaDerBody->GetWorldCenter().x, piernaDerBody->GetWorldCenter().y - 5), torsoBody, b2Vec2(torsoBody->GetWorldCenter().x + 2, torsoBody->GetWorldCenter().y + 6), 0.0f, 5.0f, 2.0f);

}

