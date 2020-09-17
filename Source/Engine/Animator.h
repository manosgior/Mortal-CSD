#pragma once
#include <functional>

typedef unsigned long timestamp_t;

enum animatorstate_t {
	ANIMATOR_FINISHED, ANIMATOR_RUNNING, ANIMATOR_STOPPED
};

class Animator;

class Animator {
protected:
	timestamp_t				startingTime;
	timestamp_t				lastTime;
	animatorstate_t			state;
	std::function<void(void)>	onFinish;

	void					notifyStopped();
	static void				(*registerAtHolder)(Animator*);
public:
	Animator();
	virtual ~Animator();

	void stop(bool runOnFinish);
	bool hasFinished() const;
	virtual void timeShift(timestamp_t offset);
	timestamp_t getStartingTime();
	timestamp_t getLastTime();
	void setLastTime(timestamp_t);
	virtual void progress(timestamp_t currTime) = 0;
	template <typename Tfunc>
	void setOnFinish(const Tfunc& onFinishFunc) {
		onFinish = onFinishFunc;
	}
	animatorstate_t getState();
};