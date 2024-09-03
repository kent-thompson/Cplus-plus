//============================================================================
// Name        : Threads.cpp
// Author      : Kent Thompson
// Version     :
// Copyright   : Copyright (c) 2024 Kent Thompson
// Description : shows Truly asynchronous threads working as they would be, if running/monitoring on machines, or seperate processes
//				 or separate processes that are not synchronous
// Build		: g++ -std=c++17 -O0 -g3 -Wall -c -fmessage-length=0 -fsanitize=thread -MMD -MP -MF"src/Threads.d" -MT"src/Threads.o" -o "src/Threads.o" "../src/Threads.cpp"
//============================================================================
#include <iostream>       // std::cout, std::endl
#include <thread>         // std::this_thread::sleep_for
#include <mutex>
#include <chrono>
#include <sstream>
#include <string>
#include<atomic>
#include <memory>

using namespace std::chrono_literals;
using std::cout; using std::endl; using std::string; // does NOT pollute global namespace and reduces the non-informational "sea of "std" prefixes

enum struct State { Error = -1, None, Ready, Paused, Started, Running, Milestone, Done, Complete };
struct ThreadData {
	std::atomic<State> state;
	std::atomic<double> elapsed;
	std::stringstream msg;	// cannot be atomic
};

std::mutex gLock;
int gTCounter = 0;			// thread counter

void coutMsg( ThreadData& data ) {	// because std::stringstream cannot be atomic
	gLock.lock();
		cout << data.msg.str();
	gLock.unlock();
}

// worker thread / tasks
void TaskOne( ThreadData& data ) {
	data.state = State::Started;

	const auto start = std::chrono::high_resolution_clock::now();
	std::this_thread::sleep_for(2000ms);

	const auto mid = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double, std::milli> middiff = mid - start;
	data.msg << "TaskOne 50 Percent at " << middiff.count() << "ms\n";

	data.state = State::Milestone;

	std::this_thread::sleep_for(2000ms);
	const auto end = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double, std::milli> diff = end - start;
	data.elapsed = diff.count();

	data.state = State::Done;
}

void TaskTwo( ThreadData& data ) {
	data.state = State::Started;
	
	const auto start = std::chrono::high_resolution_clock::now();
	std::this_thread::sleep_for(1000ms);

	const auto mid = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double, std::milli> middiff = mid - start;
	data.msg << "TaskTwo 50 Percent at " << middiff.count() << "ms\n";
	data.state = State::Milestone;

	std::this_thread::sleep_for(1000ms);
	const auto end = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double, std::milli> diff = end - start;
	data.elapsed = diff.count();

	data.state = State::Done;
}


int main() {
	ThreadData oneData;
	std::thread t1( TaskOne, std::ref(oneData) );
	t1.detach();	// TRULY ASYNCRONOUS and independent

	ThreadData twoData;
	std::thread t2( TaskTwo, std::ref(twoData) );
	t2.detach();

	// control / event Loop - each loop is a "tick"
	bool running = true;
	cout << "loop running" << endl;
	while ( running ) {
		std::this_thread::yield();

		try {
			#pragma GCC diagnostic push
			#pragma GCC diagnostic ignored "-Wswitch"

			switch( oneData.state ) {

			case State::Started:
				++gTCounter;
				oneData.state = State::Running;
				cout << "TaskOne Running" << endl;
				break;

			case State::Milestone:
				oneData.state = State::Running;
				coutMsg( oneData );
				break;

			case State::Done:
				// could now do some additional process; clean up, et cetera
				--gTCounter;
				oneData.state = State::Complete;
				cout << "TaskOne Took " << oneData.elapsed << "ms\n";
				cout << "TaskOne Complete" << endl;
				break;
			}

			switch( twoData.state ) {
			case State::Started:
				++gTCounter;
				twoData.state = State::Running;
				cout << "TaskTwo Running" << endl;
				break;

			case State::Milestone:
				twoData.state = State::Running;
				coutMsg( twoData );
				break;

			case State::Done:
				// now do some additional process; clean up, et cetera
				--gTCounter;
				twoData.state = State::Complete;
				cout << "TaskTwo Took " << twoData.elapsed << "ms\n";
				cout << "TaskTwo Complete" << endl;
				break;
			}
		} catch( std::exception& e ) {
			cout << e.what();
			// break; or re-throw exception...
		}

		//if( oneData.state == State::Complete && twoData.state == State::Complete ) {
		if( gTCounter < 1 ) {	// all threads done
			running = false;
		}
	}
	cout << "App Finished" << endl;
	return 0;
}
