#include "Weapon.h"
#include "../player_stats.h"

Weapon::~Weapon()
{
	base_proj = NULL;
	delete base_proj;
}
