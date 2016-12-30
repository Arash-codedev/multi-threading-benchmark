#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include "libs/profiler.hpp"

const long range_from=10000000l*10l;
const long range_to=20000000l*10l;

bool is_prime(long a)
{
	if(a<2l)
		return false;
	if(a==2l)
		return true;
	for(long i=2;i*i<=a;i++)
		if(a%i==0)
			return false;
	return true;
}

struct Result
{
	double exe_time;
	uint found;
	ulong checks;
	Result():
		exe_time(0.0),
		found(0),
		checks(0)
	{
		// nothing 
	}
};

void twin_range(long n_start,long n_stop,int index,int processDiv,Result *presult)
{
	// l1+(0,1,...,999)+0*1000
	// l1+(0,1,...,999)+1*1000
	// l1+(0,1,...,999)+2*1000
	// ...

	presult->checks=0;
	presult->found=0;
	const long chunks=1000;
	long r_begin=0,k=0;
	for(long i=0;r_begin<=n_stop;i++)
	{
		r_begin=n_start+(i*processDiv+index)*chunks;
		for(k=r_begin;(k<r_begin+chunks) && (k<=n_stop);k++)
		{
			if(is_prime(k) && is_prime(k+2))
			{
				presult->found++;
			}
			presult->checks++;
		}
	}
	std::cout
		<<"Thread "<<index<<" finished task with "
		<<(presult->checks)<<" checks and "
		<<(presult->found)<<" results found"
		<<std::endl<<std::flush;
}


double main_multi_thread_test(int processDiv)
{
	cronometer timer;
	timer.tic();

	std::vector<std::thread> thread_pool;
	std::vector<Result> results;
	results.assign(processDiv,Result());
	for(int th_index=0;th_index<processDiv;th_index++)
		thread_pool.push_back(std::thread(twin_range,range_from,range_to,th_index,processDiv,&results[th_index]));
	// wait for tasks to finish
	for(std::thread& th : thread_pool)
		th.join();
	ulong total_checks=0;
	ulong total_found=0;
	for(Result r:results)
	{
		total_checks+=r.checks;
		total_found+=r.found;
	}
	std::cout
		<<total_checks<<" numbers were checked. "
		<<total_found<<" twin pairs detected."
		<<std::endl;
	return timer.toc();
}

int main()
{
	struct Info
	{
		int N_thread;
		double process_time;
	};
	std::vector<Info> info;
	for (int N_thread=1;N_thread<=40;N_thread++)
	{
		double t=main_multi_thread_test(N_thread);
		std::cout<<"Process time for N_thread= "<<N_thread<<" is "<<t<<" seconds."<<std::endl;
		info.push_back({N_thread,t});
	}

	std::cout<<"================================="<<std::endl;
	for(Info v:info)
		std::cout<<v.N_thread<<"\t"<<v.process_time<<"\t"<<std::endl;
	std::cout<<"================================="<<std::endl;

	return 0;
}
