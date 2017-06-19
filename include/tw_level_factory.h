#ifndef TW_LEVEL_FACTORY_H
#define TW_LEVEL_FACTORY_H

#include "../engine/include/level_factory.h"

#include "tw_menu.h"
#include "tw_playable_level.h"
#include "tw_limbo.h"
#include "tw_cutscene.h"

using namespace ijengine;

class TWLevelFactory : public LevelFactory {
	public:
		Level *make_level(const string& level_id);
		void release(Level *level);
};

#endif
