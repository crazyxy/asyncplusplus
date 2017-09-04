namespace async {
namespace detail {

// Set of events that an task_wait_event can hold
enum wait_type {
	// The task that is being waited on has completed
	task_finished = 1,

	// A task is available to execute from the scheduler
	task_available = 2
};

// OS-supported event object which can be used to wait for either a task to
// finish or for the scheduler to have more work for the current thread.
class task_wait_event {
	std::mutex m;
	std::condition_variable c;
	int event_mask;

public:
	task_wait_event()
		: event_mask(0) {}

	// Wait for an event to occur. Returns the event(s) that occurred. This also
	// clears any pending events afterwards.
	int wait() {
		std::unique_lock<std::mutex> lock(m);
		while (event_mask == 0)
			c.wait(lock);
		int result = event_mask;
		event_mask = 0;
		return result != 0;
	}

	// Check if a specific event is ready
	bool try_wait(int event) {
		std::unique_lock<std::mutex> lock(m);
		int result = event_mask & event;
		event_mask &= ~event;
		return result != 0;
	}

	// Signal an event and wake up a sleeping thread
	void signal(int event) {
		std::lock_guard<std::mutex> lock(m);
		event_mask |= event;
		c.notify_one();
	}
};

} // namespace detail
} // namespace async
