#include "UserSceneData.h"
queue<userChange> UserSceneData::changes;
mutex UserSceneData::changesMutex;