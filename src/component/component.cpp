#include "component.h"

using namespace Mino;

void Component::enable() { gameObject->enableComponent(self.lock()); }

void Component::disable() { gameObject->disableComponent(self.lock()); }
