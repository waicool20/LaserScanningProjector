#pragma once

enum struct render {
  NONE,
  BASIC_TUPLE,
  BASIC_RECT,
  BASIC_BITMAP,
  DEBUG_HOME_LASER,
  INIT_HOME_LASER,
  AUDIO_MIC,
  USB
};

extern render rendering;
