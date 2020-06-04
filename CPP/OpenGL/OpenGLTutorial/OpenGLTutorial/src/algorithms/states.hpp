#ifndef STATES_HPP
#define STATES_HPP

namespace States {
	// check state
	template<typename T>
	bool isActive(T* states, int target) {
		return (*states & (1 << target)) == (1 << target);
	}

	// activate state
	template<typename T>
	void activate(T* states, int target) {
		*states |= 1 << target;
	}

	// uniquely activate state (no others can be active)
	template<typename T>
	void uniquelyActivate(T* states, int target) {
		*states &= 1 << target;
	}

	// deactivate state
	template<typename T>
	void deactivate(T* states, int target) {
		*states &= ~(1 << target);
	}

	// toggle state
	template<typename T>
	void toggle(T* states, int target) {
		*states ^= 1 << target;
	}
}

#endif