#pragma once

int ma_init();
int ma_initSound(const char* p, int ndx); // returns sound index
void ma_setPosition(int soundIndex, float x, float y, float z);
void ma_setListenerPosition(float x, float y, float z);
int ma_adjustStuff();
int ma_playSound(int ndx);
void ma_close();
