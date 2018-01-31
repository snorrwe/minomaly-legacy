#include "component.h"

using namespace Mino;

void Component::enable() { gameObject->enableComponent(this); }

void Component::disable() { gameObject->disableComponent(this); }
