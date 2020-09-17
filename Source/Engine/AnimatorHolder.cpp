#include "AnimatorHolder.h"
#include <vector>

std::list<Animator *> AnimatorHolder::running;
std::list<Animator *> AnimatorHolder::suspended;

AnimatorHolder::ProgressFunctor::ProgressFunctor(){ }

void AnimatorHolder::ProgressFunctor::operator()(Animator *a) const {
	a->progress(t);
}

AnimatorHolder::ProgressFunctor::ProgressFunctor(timestamp_t t) : t(t) {}
	
void AnimatorHolder::Register(Animator *a) {
	suspended.push_back(a);
}

void AnimatorHolder::cancel(Animator *a) {
	suspended.remove(a);
}

void AnimatorHolder::cancelAll() {
	running.clear();
	suspended.clear();
}

void AnimatorHolder::markAsRunning(Animator *a) {
	suspended.remove(a);
	running.push_back(a);
}

void AnimatorHolder::markAsSuspended(Animator *a) {
	running.remove(a);
	suspended.push_back(a);
}

void AnimatorHolder::progress(timestamp_t currTime) {
	std::vector<Animator *> justFinished;
	for (std::list<Animator *>::iterator i = running.begin(); i != running.end(); i++) {
		if(!(*i)->hasFinished())
			(ProgressFunctor(currTime))(*i);
		else
			justFinished.push_back(*i);
	}
	for (auto i : justFinished) {
		markAsSuspended(i);
	}
}

void AnimatorHolder::timeshiftAll(timestamp_t offset) {
	for (std::list<Animator *>::iterator i = running.begin(); i != running.end(); i++) {
		(*i)->timeShift(offset);
	}
}
