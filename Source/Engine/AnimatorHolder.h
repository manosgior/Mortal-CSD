#pragma once

#include <list>
#include "Animation.h"
#include "Animator.h"

class AnimatorHolder {
private:
	static std::list<Animator *> running, suspended;

	class ProgressFunctor : public std::unary_function<Animator *, void> {
	private:
		timestamp_t t;
	public:
		void operator()(Animator *) const;
		ProgressFunctor();
		ProgressFunctor(timestamp_t t);
	};
public:
	static void Register(Animator *a);
	static void cancel(Animator *a);
	static void cancelAll();
	static void markAsRunning(Animator *a);
	static void markAsSuspended(Animator *a);
	static void progress(timestamp_t currTime);
	static void timeshiftAll(timestamp_t offset);
};