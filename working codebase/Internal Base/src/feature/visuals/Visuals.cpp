#include "Visuals.h"
#include "esp/Esp.h"
#include "enemycounter/EnemyCounter.h"
#include "fov/Fov.h"
void Visuals::Render()
{
    GameTitle::Render();
	FOV::Render();
}
