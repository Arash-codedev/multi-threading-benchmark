#pragma once

#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS

#include <string>
#include <fstream>

void call_loger(std::string a);

namespace filesystem
{

	void ensure_folder(const boost::filesystem::path &filepath)
	{
		if(!boost::filesystem::exists(filepath))
			boost::filesystem::create_directory(filepath);	
	}

	bool exists(const boost::filesystem::path &filepath)
	{
		return boost::filesystem::exists(filepath);
	}

	std::string read_file(const boost::filesystem::path &filepath)
	{
		if(!boost::filesystem::exists(filepath))
			throw std::runtime_error(std::string("file \"")+filepath.string()+"\" does not exist!");
		std::string data;
		std::ifstream in(filepath.string().c_str());
		std::getline(in, data, std::string::traits_type::to_char_type(
						  std::string::traits_type::eof()));
		return data;
	}

	void write_file(const boost::filesystem::path &filepath, const std::string &text)
	{
		std::ofstream out(filepath.string().c_str());
		out << text;
		out.close();
	}

	void copy_file(const boost::filesystem::path &from,const boost::filesystem::path &to)
	{
		std::ifstream stream_from(from.string(), std::ios::binary);
		std::ofstream stream_to(to.string(), std::ios::binary);
		stream_to << stream_from.rdbuf();
	}

	void copy_folder(const boost::filesystem::path &from,const boost::filesystem::path &to,const bool is_merged=true)
	{
		if(!boost::filesystem::exists(from))
			throw std::runtime_error(std::string("Folder ")+from.string()+" does not exist.");
		if(!boost::filesystem::is_directory(from) )
			throw std::runtime_error(std::string("")+from.string()+" is not a folder.");
		if(!is_merged && boost::filesystem::exists(to))
			throw std::runtime_error(std::string("")+to.string()+" already does exist.");
		if(!boost::filesystem::exists(to) && !boost::filesystem::create_directory(to))
			throw std::runtime_error(std::string("Cannot create ")+to.string()+" .");

		for(boost::filesystem::directory_iterator file_it(from);file_it != boost::filesystem::directory_iterator(); ++file_it)
			if(boost::filesystem::is_directory(file_it->path()))
				copy_folder(file_it->path(),to/file_it->path().filename(),is_merged);
			else
			{
				// there is a bug in boost::filesystem 
				// causing this error: undefined reference to `boost::filesystem::detail::copy_file ...
				// boost::filesystem::copy_file(file_it->path(),to/file_it->path().filename());

				filesystem::copy_file(file_it->path(),to/file_it->path().filename());
			}

	}

};
