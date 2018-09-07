#include "m3d/M3D.h"
#include "m3d/utils/FileHelper.h"
#include <stdio.h>
#include <stdlib.h>

#include<string.h>

#if defined WIN32 
#   include <direct.h>  
#   include <io.h>  

#   include <shlobj.h>  
#   include <sys/stat.h>  
#   include <sys/types.h>  
#else // Linux  
#   include <dirent.h>  
#   include <unistd.h>  
#   include <sys/stat.h>  
#   include <sys/types.h>  
#   include <pwd.h>  
#include "Utility.h"
#endif  
#include <iostream>
#include <fstream>
namespace M3D {
	string FileHelper::GetUnionStylePath(const string filePath)
	{
		string linuxstylePath = filePath;
		FileHelper::Replace_all(linuxstylePath, "\\", "/");
		return linuxstylePath;
	}

	void FileHelper::Replace_all(string& src, const string& oldvalue, const string& newvalue)
	{
		while (true)
		{
			string::size_type pos(0);
			if ((pos = src.find(oldvalue)) != string::npos)
			{
				src.replace(pos, oldvalue.length(), newvalue);
			}
			else
			{
				break;
			}
		}
	}

	string FileHelper::GetDirName(const string& filePath)
	{
		string tFilePath = filePath;
		const char* path = tFilePath.c_str();
		char* dir;
		char* s;
#ifdef WIN32
		dir = _strdup(path);
#endif

#ifdef __MOBILE__
		dir = strdup(path);
#endif
		s = strrchr(dir, '/');
		//s = strrchr(dir, '\\');
		if (s)
			s[1] = '\0';
		else
			dir[0] = '\0';

		tFilePath = string(dir);
		free(dir);
		return tFilePath;
	}

	string FileHelper::GetFileName(const string& filePath)
	{
		std::size_t pos = filePath.find_last_of("/");

	string name = "";

	if(pos > 0 && pos < filePath.size()-1)
	{
		name = filePath.substr(pos+1,filePath.size());
	}

		return name;
	}

	string FileHelper::GetFileNameWithoutExt(const string& filePath)
	{
		std::size_t pos = filePath.find_last_of("/");

		string name = "";

		if (pos > 0 && pos < filePath.size() - 1)
		{
			name = filePath.substr(pos + 1, filePath.size());
		}

		pos = name.find_last_of(".");

		 
		if (pos > 0 && pos < name.size() - 1)
		{
			name = name.substr(0,pos);
		}
		return name;
	}

	string FileHelper::GetExt(const string& filePath)
	{
		string ext;
		int dotIndex = filePath.find_last_of('.');
		if (dotIndex != -1)
		{
			ext = filePath.substr(dotIndex + 1);
		}
		return ext;
	}

	string FileHelper::GetEnCode(const string& filePath)
	{
		string code = "ASCII";

		return code;
	}

	bool FileHelper::FileExist(const string& filePath)
	{
		bool exist = false;
#ifdef WIN32
		if (_access(filePath.c_str(), 0) != -1)
		{
			exist = true;
		}
		else {
			LOGE("File not exist %d, access-- %d", errno, _access(filePath.c_str(), 0));
		}
#endif

#ifdef __MOBILE__
		if (access(filePath.c_str(), 0) != -1)
		{
			exist = true;
		}
		else {
			LOGE("File not exist %d, access-- %d", errno, access(filePath.c_str(), F_OK));
		}
#endif


		return exist;
	}

	int FileHelper::is_dir(const char* path) {//�ж��Ƿ���Ŀ¼
#ifdef __MOBILE__
		struct stat st;
		stat(path, &st);
		if (S_ISDIR(st.st_mode)) {
			return 1;
		}
		else {
			return 0;
		}
#endif
		return 0;
	}
	/*�ַ���������*/
	int FileHelper::endwith(const char* s, char c) {//�����ж��ַ�����β�Ƿ�Ϊ��/��
		if (s[strlen(s) - 1] == c) {
			return 1;
		}
		else {
			return 0;
		}
	}
	string FileHelper::str_contact(const char* str1, const char* str2) {//�ַ�������
		char* result;
		result = (char*)calloc(1, strlen(str1) + strlen(str2) + 1);//str1�ĳ���+str2�ĳ���+\0;
		if (!result) {//����ڴ涯̬����ʧ��
			printf("�ַ�������ʱ���ڴ涯̬����ʧ��\n");
			exit(1);
		}
		memset(result, 0, strlen(str1) + strlen(str2) + 1);
		strcat(result, str1);
		strcat(result, str2);//�ַ���ƴ��
		string retStr = result;
		return retStr;
	}
	/*���ƺ���*/
	void FileHelper::copy_file(const char* source_path, const char *destination_path) {//�����ļ�
		char buffer[1024];
		FILE *in, *out;//���������ļ������ֱ������ļ��Ķ�ȡ��д��int len;
		if ((in = fopen(source_path, "r")) == NULL) {//��Դ�ļ����ļ���
			printf("Դ�ļ���ʧ�ܣ�\n");
			exit(1);
		}
		if ((out = fopen(destination_path, "w")) == NULL) {//��Ŀ���ļ����ļ���
			printf("Ŀ���ļ�����ʧ�ܣ�\n");
			exit(1);
		}
		int len;//lenΪfread�������ֽڳ�
		while ((len = fread(buffer, 1, 1024, in)) > 0) {//��Դ�ļ��ж�ȡ���ݲ��ŵ��������У��ڶ�������1Ҳ����д��sizeof(char)
			fwrite(buffer, 1, len, out);//��������������д��Ŀ���ļ���
		}
		fclose(out);
		fclose(in);
	}
void FileHelper::copy_folder(const char* source_path,const char *destination_path){//�����ļ���
#ifdef __MOBILE__
		if (!opendir(destination_path)) {
			if (mkdir(destination_path, 0777))//��������ھ���mkdir����������
			{
				printf("�����ļ���ʧ�ܣ�");
			}
		}
		string path;
		path = str_contact(path.c_str(), source_path);//�����䣬�൱��path=source_path
		struct dirent* filename;
		DIR* dp = opendir(path.c_str());//��DIRָ��ָ������ļ���
    if (dp) {
        while (filename = readdir(dp)){//����DIRָ��ָ����ļ��У�Ҳ�����ļ����顣
            path = str_contact(path.c_str(), source_path);
            //���source_path,destination_path��·���ָ�����β����ôsource_path/,destination_path/ֱ����·������
            //����Ҫ��source_path,destination_path���油��·���ָ����ټ��ļ�����˭֪���㴫�ݹ����Ĳ�����f:/a����f:/a/����
            string file_source_path;
            if (!endwith(source_path, '/')) {
                file_source_path = str_contact(file_source_path.c_str(), source_path);
                file_source_path = str_contact(source_path, "/");
            }
            else {
                file_source_path = str_contact(file_source_path.c_str(), source_path);
            }
            string file_destination_path;

            if (!endwith(destination_path, '/')) {
                file_destination_path = str_contact(file_destination_path.c_str(), destination_path);
                file_destination_path = str_contact(destination_path, "/");
            }
            else {
                file_destination_path = str_contact(file_destination_path.c_str(), destination_path);
            }
            //ȡ�ļ����뵱ǰ�ļ���ƴ�ӳ�һ��������·��
            file_source_path = str_contact(file_source_path.c_str(), filename->d_name);
            file_destination_path = str_contact(file_destination_path.c_str(), filename->d_name);
            if (is_dir(file_source_path.c_str())) {//�����Ŀ¼
                if (!endwith(file_source_path.c_str(), '.')) {//ͬʱ������.��β����ΪLinux�������ļ��ж���һ��.�ļ�������������һ��Ŀ¼�������޳���������еݹ�Ļ�������޷�����
                    copy_folder(file_source_path.c_str(), file_destination_path.c_str());//���еݹ���ã��൱�ڽ�������ļ��н��и��ơ�
                }
            }
            else {
                copy_file(file_source_path.c_str(), file_destination_path.c_str());//�����յ�һ�ļ��ĸ��Ʒ������и��ơ�
            }
        }
    }
#else
		string srcDirPath = source_path;
		//	std::string srcDir;
		//#ifdef _WIN32  
		//	int n = 0;
		//	while (srcDirPath.find('\\', n) != std::string::npos)
		//	{
		//		n = srcDirPath.find('\\', n) + 1;
		//	}
		//	if (n == 0)
		//	{
		//		std::cout << "src path error" << std::endl;
		//		return;
		//	}
		//	srcDir = srcDirPath.substr(n - 1, srcDirPath.size());
		//
		//#else  // Linux  
		//	int n = 0;
		//	while (srcDirPath.find('/', n) != std::string::npos)
		//	{
		//		n = srcDirPath.find('/', n) + 1;
		//	}
		//	if (n == 0)
		//	{
		//		std::cout << "src path error" << std::endl;
		//		return;
		//	}
		//	srcDir = srcDirPath.substr(n - 1, srcDirPath.size());
		//
		//#endif  
		string desDirPath = destination_path/* + srcDir*/;
		delete_dir(desDirPath);
		if (!make_dir(desDirPath))
		{
			return;
		}

		std::vector<std::string> fileNameList;
		if (!get_src_files_name(srcDirPath, fileNameList))
		{
			return;
		}

		if (fileNameList.empty())
		{
			std::cout << "src dir is empty" << std::endl;
			return;
		}

		do_copy(srcDirPath, desDirPath, fileNameList);
#endif

	}

	bool FileHelper::make_dir(const std::string& pathName)
	{
#ifdef _WIN32  
		if (::_mkdir(pathName.c_str()) < 0)
		{
			std::cout << "create path error" << std::endl;
			return false;
		}
#else  // Linux  
		if (::mkdir(pathName.c_str(), S_IRWXU | S_IRGRP | S_IXGRP) < 0)
		{
			std::cout << "create path error" << std::endl;
			return false;
		}
#endif  

		return true;
	}

	bool FileHelper::delete_dir(std::string strDir)
	{
#ifdef _WIN32  
		_finddata_t file;
		long lf;
		std::string des = strDir + "\\*.*";
		if ((lf = _findfirst(des.c_str(), &file)) == -1)
		{
			std::cout << strDir << " not found" << std::endl;
			return false;
		}
		else {
			while (_findnext(lf, &file) == 0)
			{
				if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
					continue;
				std::string strCurFilePath = strDir;
				strCurFilePath.append("\\");
				strCurFilePath.append(file.name);
				if (GetFileAttributesA(strCurFilePath.c_str()) == FILE_ATTRIBUTE_DIRECTORY)
				{
					delete_dir(strCurFilePath.c_str());
				}
				else
				{
					remove(strCurFilePath.c_str());
				}
			}
		}
		_findclose(lf);
#else  // Linux  
		DIR *dir;
		struct dirent *ptr;

		if ((dir = opendir(strDir.c_str())) == NULL)
		{
			std::cout << strDir << " not found" << std::endl;
			return false;
		}

		while ((ptr = readdir(dir)) != NULL)
		{
			std::string strCurFilePath = strDir;
			strCurFilePath.append("\\");
			strCurFilePath.append(ptr->d_name);
			if ((ptr->d_name == ".") || (ptr->d_name == ".."))  //current / parent  
				continue;
			else if (ptr->d_type == 8)  //file  
				rmdir(strCurFilePath.c_str());
			else if (ptr->d_type == 10)  //link file  
				continue;
			else if (ptr->d_type == 4)  //dir  
			{
				delete_dir(strCurFilePath.c_str());
			}
		}
		closedir(dir);

#endif  
		rmdir(strDir.c_str());
		return true;
	}
	void
		FileHelper::do_copy(const string&srcDirPath, const string&desDirPath, const std::vector<std::string> &fileNameList)
	{
#pragma omp parallel for  
		for (int i = 0; i < fileNameList.size(); i++)
		{
			std::string nowSrcFilePath, nowDesFilePath;
#ifdef _WIN32  
			nowSrcFilePath = srcDirPath + "\\" + fileNameList.at(i);
			nowDesFilePath = desDirPath + "\\" + fileNameList.at(i);

#else  
			nowSrcFilePath = srcDirPath + "/" + fileNameList.at(i);
			nowDesFilePath = desDirPath + "/" + fileNameList.at(i);

#endif  
			std::ifstream in;
			in.open(nowSrcFilePath, std::ios::in | std::ios::binary);
			if (!in)
			{
				std::cout << "open src file : " << nowSrcFilePath << " failed" << std::endl;
				continue;
			}

			std::ofstream out;
			out.open(nowDesFilePath, std::ios::binary | std::ios::out);
			if (!out)
			{
				std::cout << "create new file : " << nowDesFilePath << " failed" << std::endl;
				in.close();
				continue;
			}

			out << in.rdbuf();

			out.close();
			in.close();
		}
	}

	string FileHelper::ReadAsciiFile(const string& filePath)
	{
		FILE* fp;
		char* str;
		char txt[1000];
		int fileSize;
		if ((fp = fopen(filePath.c_str(),"rb")) == NULL)
		{
			assert(false);
			return "";
		}

		fseek(fp, 0, SEEK_END);
		
		fileSize = ftell(fp);

		rewind(fp);

		str = (char*)malloc(sizeof(char)*fileSize);

		str[0] = 0;

		size_t result =  fread(str, 1, fileSize, fp);

		if (result != fileSize)
		{
			assert(false);
			return "";
		}

		fclose(fp);
		string ret = str;

		free(str);

		return ret;
	}

	std::string FileHelper::ReadTxt(string file)
	{
		string ret = "";
		ifstream infile;
		infile.open(file.data());   //将文件流对象与文件连接起来 
		assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 

		string s;
		while (getline(infile, s))
		{
			ret += s;
		}
		infile.close();             //关闭文件输入流 
		return ret;
	}

	bool FileHelper::ReadBinFile(const string& filePath, char* &readBuffer, int& bufferLength)
	{
		readBuffer = NULL;

		FILE* fp;
			
		if ((fp = fopen(filePath.c_str(), "rb")) == NULL)
		{
			assert(false);
			return false;
		}

		fseek(fp, 0, SEEK_END);

		bufferLength = ftell(fp);

		rewind(fp);

		readBuffer = new char[bufferLength];

		//readBuffer[0] = 0;

		size_t result = fread(readBuffer, 1, bufferLength, fp);

		if (result != bufferLength)
		{
			assert(false);
			fclose(fp);
			return false;
		}
	
		fclose(fp);

		return true;
	}

	bool FileHelper::RemoveFile(const string& filePath)
	{
		if (std::remove(filePath.c_str()) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
		return false;
	}

	int FileHelper::SaveFileAssAscii(const string & fileContent, const string & destFilePath)
	{
		int saveState = 1;
		std::ofstream outfile;
		outfile.open(destFilePath);

		if (!outfile)
		{
			saveState = 0;
		}
		else
		{
			outfile << fileContent;
			outfile.close();
		}

		return saveState;
	}

	int FileHelper::SaveFileAsBin(const char* buffer, int bufferSize,const string& destFilePath)
	{
		int saveState = 1;

		std::ofstream outfile(destFilePath, std::ios::binary);

		if (!outfile)
		{
			saveState = 0;
		}
		else
		{
			outfile.write(buffer,sizeof(char) * bufferSize);
			outfile.close();
		}

		return saveState;
	}

	bool
		FileHelper::get_src_files_name(const string&srcDirPath, std::vector<std::string>& fileNameList)
	{
#ifdef _WIN32  
		_finddata_t file;
		long lf;
		std::string src = srcDirPath + "\\*.*";
		if ((lf = _findfirst(src.c_str(), &file)) == -1)
		{
			std::cout << srcDirPath << " not found" << std::endl;
			return false;
		}
		else {
			while (_findnext(lf, &file) == 0)
			{
				if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
					continue;
				fileNameList.push_back(file.name);
			}
		}


		_findclose(lf);
#else  // Linux  
		DIR *dir;
		struct dirent *ptr;

		if ((dir = opendir(srcDirPath.c_str())) == NULL)
		{
			std::cout << srcDirPath << " not found" << std::endl;
			return false;
		}

		while ((ptr = readdir(dir)) != NULL)
		{
			if ((ptr->d_name == ".") || (ptr->d_name == ".."))  //current / parent  
				continue;
			else if (ptr->d_type == 8)  //file  
				fileNameList.push_back(ptr->d_name);
			else if (ptr->d_type == 10)  //link file  
				continue;
			else if (ptr->d_type == 4)  //dir  
				fileNameList.push_back(ptr->d_name);
		}
		closedir(dir);

#endif  

		return true;

	}
}
