#pragma once
#include <atomic>
#include <string_view>

#include "version.h"

std::atomic<bool> running{true};

void handle_signal(int signal);
void prepareOptions(int argc, char* argv[]);
void initialize();
void start();
void stop();

int main (int argc, char* argv[]);
