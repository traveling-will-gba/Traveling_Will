#ifndef TRAVELING_WILL_LEVEL_FACTORY_H
#define TRAVELING_WILL_LEVEL_FACTORY_H

#include <ijengine/level_factory.h>

#include "menu.h"

using namespace ijengine;

class TravelingWillLevelFactory : public LevelFactory {
	public:
		TravelingWillLevel *make_level(const string& level_id);
		void release(Level *level);
};

#endif
