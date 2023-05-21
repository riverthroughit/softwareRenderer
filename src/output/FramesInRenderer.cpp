#include "FramesInRenderer.h"
mutex FramesInRenderer::framesMutex;
queue<QRgb*> FramesInRenderer::frames;