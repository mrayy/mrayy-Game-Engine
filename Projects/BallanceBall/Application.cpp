
#include "stdafx.h"
#include "Application.h"




namespace mray
{

Application::Application()
{
}
Application::~Application()
{
}

void Application::init(core::array<SOptionElement> *extraOptions)
{
	CMRayApplication::init(extraOptions);
}
void Application::update(float dt)
{
	CMRayApplication::update(dt);
}
void Application::draw()
{
	CMRayApplication::draw();
}
void Application::onDone()
{
	CMRayApplication::onDone();
}


}
