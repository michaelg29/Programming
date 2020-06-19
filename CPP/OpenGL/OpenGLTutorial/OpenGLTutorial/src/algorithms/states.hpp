#ifndef STATES_HPP
#define STATES_HPP

namespace States {
	// check target index
	template<typename T>
	bool isIndexActive(T* states, int target) {
		return (*states & (1 << target)) == (1 << target);
	}

	// activate target index
	template<typename T>
	void activateIndex(T* states, int target) {
		*states |= 1 << target;
	}

	// uniquely activate target index (no others can be active)
	template<typename T>
	void uniquelyActivateIndex(T* states, int target) {
		*states &= 1 << target;
	}

	// deactivate target index
	template<typename T>
	void deactivateIndex(T* states, int target) {
		*states &= ~(1 << target);
	}

	// toggle target index
	template<typename T>
	void toggleIndex(T* states, int target) {
		*states ^= 1 << target;
	}

	// check state
	template<typename T>
	bool isActive(T* states, T state) {
		return (*states & state) == state;
	}

	// activate state
	template<typename T>
	void activate(T* states, T state) {
		*states |= state;
	}

	// uniquely activate state (no others can be active)
	template<typename T>
	void uniquelyActivate(T* states, T state) {
		*states &= state;
	}

	// deactivate state
	template<typename T>
	void deactivate(T* states, T state) {
		*states &= ~state;
	}

	// toggle state
	template<typename T>
	void toggle(T* states, T state) {
		*states ^= state;
	}
}

#endif