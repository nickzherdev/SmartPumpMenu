#pragma once
#include "options.h"

void handleNewMessages(int numNewMessages, Settings &settings);

void connect_wifi(void);

void run_bot(Settings &settings);
