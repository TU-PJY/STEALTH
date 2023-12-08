#pragma once
#include "config.h"

typedef struct {
	GLfloat x, z;
	GLfloat height, width;
	int type;
}Pillar;
extern Pillar p[20];

extern int num;