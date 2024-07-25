#pragma once
#include <atomic>
#include <string_view>

#include "version.h"

std::atomic<bool> running{true};

void handle_signal(int signal);
int main (int argc, char* argv[]);
