#ifndef FIREFLY_SYSTEM_H
#define FIREFLY_SYSTEM_H

#include <entt/entt.hpp>

class FireflySystem {
private:

public:
	FireflySystem() = default;
	~FireflySystem() = default;

	static void Update(entt::registry& registry);
};

#endif