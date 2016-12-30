#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include "libs/profiler.hpp"
#include "libs/filesystem.hpp"

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

uint twin_range(long l1,long l2,int processDiv)
{
	uint count=0;
	for(long l=l1;l<=l2;l+=long(processDiv))
		if(is_prime(l) && is_prime(l+2))
		{
			count++;
		}
	return count;
}


double main_multi_thread_test(int processDiv)
{
	cronometer timer;
	timer.tic();

	std::vector<std::thread> thread_pool;
	for(long i=0;i<processDiv;i++)
		thread_pool.push_back(std::thread(twin_range,range_from+i,range_to,processDiv));
	// wait for tasks to finish
	for(std::thread& th : thread_pool)
		th.join();
	return timer.toc();
}

double cpu_temperature()
{
	std::string temperature_file="/sys/class/hwmon/hwmon0/temp1_input";
	std::string tmpr=filesystem::read_file(temperature_file);
	return double(atol(tmpr.c_str()))/1000.0;
}

int main()
{
	struct Info
	{
		int N_thread;
		double process_time;
		double cpu_tempr;
	};
	std::vector<Info> info;
	for (int N_thread=3;N_thread<40;N_thread++)
	{
		double t=main_multi_thread_test(N_thread);
		double cpu_t=cpu_temperature();
		std::cout<<"Process time for N_thread= "<<N_thread<<" is "<<t<<" seconds. CPU temperature is "<<cpu_t<<std::endl<<std::flush;
		info.push_back({N_thread,t,cpu_t});
	}

	std::cout<<"================================="<<std::endl;
	for(Info v:info)
		std::cout<<v.N_thread<<"\t"<<v.process_time<<"\t"<<v.cpu_tempr<<std::endl;
	std::cout<<"================================="<<std::endl;

	return 0;
}
