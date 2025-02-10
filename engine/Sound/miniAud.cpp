#include "./miniaudio/extras/miniaudio_split/miniaudio.h"
#include <stdio.h>
#include <chrono>
#include <thread>

ma_engine gEngine;
ma_resource_manager* gResourceManager = nullptr;
std::vector<ma_sound> gSounds;
ma_sound_group gVoiceGroup;
ma_sound_group gCollisionGroup;

int ma_init()
{
    // https://miniaud.io/docs/manual/index.html
    ma_result result = ma_engine_init(NULL, &gEngine);
    if (result != MA_SUCCESS)
    {
        return -1;
    }
    
    gResourceManager = ma_engine_get_resource_manager(&gEngine);
    if (gResourceManager == NULL)
    {
        return -1;
    }
    gSounds.resize(5);
    int flags = 0;
    ma_sound_group_init(&gEngine, flags, 0, &gCollisionGroup);
    ma_engine_set_volume(&gEngine, 30.0f);

    return 0;
}

int ma_adjustStuff()
{
    std::chrono::milliseconds timespan1(1000);
    std::this_thread::sleep_for(timespan1);
    ma_result result = MA_SUCCESS;// 
    if (result != MA_SUCCESS)
        return -1;
    return 0;
}

int ma_initSound(const char* p, int soundIndex)
{
    ma_uint32 flags = MA_SOUND_FLAG_DECODE;// | MA_SOUND_FLAG_ASYNC;
    if (soundIndex > gSounds.size())
    {
        gSounds.resize(2 * soundIndex);
    }

    ma_result result = ma_sound_init_from_file(&gEngine, p, flags, NULL, NULL, &gSounds[soundIndex]);
    if (result != MA_SUCCESS) {
        return result;
    }
    if (soundIndex == 0)
    {
        ma_sound_set_volume(&gSounds[soundIndex], 1.0f);
        ma_sound_set_position(&gSounds[soundIndex], -20, 0, -20);
    }
    else
    {
        ma_sound_set_volume(&gSounds[soundIndex], 1.0f);
        ma_sound_set_position(&gSounds[soundIndex], 20, 0, -20);
    }
    std::chrono::milliseconds timespan(1000);
    std::this_thread::sleep_for(timespan);
    return soundIndex;
}

void ma_setPosition(int soundIndex, float x, float y, float z)
{
    ma_sound_set_position(&gSounds[soundIndex], x, y, z);
}

void ma_setListenerPosition(float x, float y, float z)
{
    ma_engine_listener_set_position(&gEngine, 0, x, y, z);
}

void playSoundImp(int ndx)
{
    ma_sound_start(&gSounds[ndx]);
}


int ma_playSound(int ndx)
{
    std::thread t1(playSoundImp, ndx); t1.join();
    return 0;
}

void ma_close()
{
    std::vector<ma_sound> gSounds;
    for(ma_sound& var:  gSounds)
    {
        //ma_sound_uninit(&var);
    }
    //ma_engine_uninit(&gEngine);
}
