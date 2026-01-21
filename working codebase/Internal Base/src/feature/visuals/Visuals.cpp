#include "Visuals.h"
#include "esp/Esp.h"
#include "enemycounter/EnemyCounter.h"
#include "fov/Fov.h"
void Visuals::Render()
{
    ESP::setup();
	GameTitle::Render();
	FOV::Render();
}
