#pragma once
#include <string>

typedef signed short	offset_t;
typedef unsigned short	delay_t;
typedef std::string	animid_t;
typedef unsigned char frame_t;

class Animation {
	animid_t id;
public:
	animid_t getID(void);
	virtual Animation* clone(animid_t newId) const = 0;
	Animation(animid_t id);
	virtual ~Animation();
};