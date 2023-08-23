#pragma once

#include "alias.h"
#include "background.h"
#include "bar_item.h"
#include "bar_manager.h"
#include "display.h"
#include "event.h"
#include "group.h"
#include "mach.h"
#include "misc/defines.h"
#include "misc/helpers.h"
#include "slider.h"
#include <regex.h>

MACH_HANDLER(mach_message_handler);
void handle_message_mach(struct mach_buffer *buffer);
