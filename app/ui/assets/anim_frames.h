#ifndef IMAGE_DATA_H
#define IMAGE_DATA_H

#include <stdint.h>

#define ANIM_WIDTH       128
#define ANIM_HEIGHT      128
#define ANIM_NUM_FRAMES  21

/** Raw bitmap data arranged by [frame][byte] */
extern const uint8_t anim_frames[ANIM_NUM_FRAMES][2048];

#endif /* IMAGE_DATA_H */
