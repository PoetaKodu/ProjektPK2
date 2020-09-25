#pragma once

#include <Filter/ParseArgs.h>

#include <stdbool.h>

/** Uruchamia aplikację z odpowiednim ustawieniem.
 * @param launchCfg_ ustawienie aplikacji
 * @return true jeśli wszystko przebiegło pomyślnie, w przeciwnym wypadku false
 * */
bool run(LaunchConfig *launchCfg_);