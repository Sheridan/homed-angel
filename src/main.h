#pragma once
#include <atomic>
#include <string_view>

static const std::string_view version = "0.1.0";

std::atomic<bool> running{true};

void handle_signal(int signal);
int main (int argc, char* argv[]);
