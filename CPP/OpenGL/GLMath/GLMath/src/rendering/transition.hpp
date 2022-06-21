#ifndef TRANSITION_H
#define TRANSITION_H

// ensure this function takes all its desired values between t = 0 and t = 1
typedef double(*transition_function)(double t);

template <typename T>
class Transition {
private:
	double cur_t;

protected:
	T start;
	T end;
	double duration;
	
	virtual T calculateNew(double t) { return end; }

public:
	bool finished = false;

	Transition(T start, T end, double duration)
		: start(start), end(end), cur_t(0.0), duration(duration) { }

	T update(double dt) {
		cur_t += dt / duration;
		if (cur_t >= 1.0) {
			finished = true;
			return end;
		}

		return calculateNew(cur_t);
	}
};

template <typename T>
class ProportionalTransition : public Transition<T> {
	T calculateNew(double t) {
		// calculate proportion of progression
		float prop = calculateProportion(t);
		// linear interpolation to get transition value
		return (1.0f - prop) * this->start + prop * this->end;
	}

protected:
	virtual double calculateProportion(double t) { return 1.0; }

public:
	ProportionalTransition(T start, T end, double duration)
		: Transition<T>(start, end, duration) { }
};

template <typename T>
class StepTransition : public ProportionalTransition<T> {
	unsigned int steps;

	double calculateProportion(double t) {
		return floor(t * (double)steps) / (double)steps;
	}

public:
	StepTransition(T start, T end, double duration, unsigned int steps)
		: ProportionalTransition<T>(start, end, duration), steps(steps) { }
};

template <typename T>
class LinearTransition : public ProportionalTransition<T> {
	double calculateProportion(double t) {
		return t;
	}

public:
	LinearTransition(T start, T end, double duration)
		: ProportionalTransition<T>(start, end, duration) { }
};

template<typename T>
class CubicBezier : public ProportionalTransition<T> {
	glm::f64vec2 t0, t1, t2, t3;

	double calculateProportion(double t) {
		double cur_t1 = 1 - t;

		glm::f64vec2 val = (cur_t1 * cur_t1 * cur_t1) * t0
			+ 3 * (cur_t1 * cur_t1 * t) * t1
			+ 3 * (cur_t1 * t * t) * t2
			+ (t * t * t) * t3;

		return val.y;
	}

public:
	CubicBezier(T start,
				double p1, double t1,
				double p2, double t2,
				T end, double duration)
		: ProportionalTransition<T>(start, end, duration),
	      t0(0.0, 0.0), t1(p1, t1), t2(p2, t2), t3(1.0, 1.0) { }

	static CubicBezier<T> newEaseTransition(T start, T end, double duration) {
		return CubicBezier<T>(start, 0.25, 0.1, 0.25, 1.0, end, duration);
	}
};

template <typename T>
class CubicBezierPath : public Transition<T> {
	T t0, t1, t2, t3;

	T calculateNew(double t) {
		double cur_t1 = 1 - t;

		return (float)(cur_t1 * cur_t1 * cur_t1) * t0
			+ 3 * (float)(cur_t1 * cur_t1 * t) * t1
			+ 3 * (float)(cur_t1 * t * t) * t2
			+ (float)(t * t * t) * t3;
	}

public:
	CubicBezierPath(T start, T end, double duration,
		T t1, T t2)
		: Transition<T>(start, end, duration, NULL) {
		this->t0 = start;
		this->t1 = t1;
		this->t2 = t2;
		this->t3 = end;
	}
};

#endif // TRANSITION_H