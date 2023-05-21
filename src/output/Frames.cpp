#include "Frames.h"
mutex Frames::framesMutex;
queue<QRgb*> Frames::frames;