#ifndef SFX_H
#define SFX_H

#ifdef DEDICATED_ONLY
#error "Can't use this in dedicated server"
#endif //DEDICATED_ONLY

#include "util/vec.h"
#include "gusanos/sound.h"

void update_volume( int oldValue );


struct Listener
{
	Vec pos;
	Vec spd;
};


class SfxDriver;
class Sound;

class Sfx
{
public:
		
	Sfx();
	~Sfx();
	
	bool init();
	void shutDown();
	void registerInConsole();
	void think();
	void clear();
	void registerListener(Listener* listener);
	void removeListener(Listener* listener);
	void volumeChange();
	SfxDriver* getDriver();
	
	operator bool(); // Returns true if it's safe to use this object
private:
	SfxDriver *driver;
};

extern Sfx sfx;

#endif // _GFX_H_