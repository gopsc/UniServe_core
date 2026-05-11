#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
namespace qing {
class MatrixObject {	/* 矩阵式对象 */
public:
	void fromStdin()	/* 解析标准输入中的动作脚本 */
	{
		count  = 0;
		_clear_all();
		_parse(std::cin);
	}

	void fromFile(const std::string path)	/* 解析文件输入流中的动作脚本 */
	{
		count  = 0;
		std::ifstream file(path.c_str(), std::ios::in);
		if (!file.is_open())
			throw std::runtime_error("Failed to open file");
		_clear_all();
		_parse(file);
		file.close();
	}

	void fromStr(const std::string& script)	/* 解析字符串流中的动作脚本 */
	{
		std::stringstream ss(script);
		count = 0;
		_clear_all();
		_parse(ss);
	}

private:
	std::string name = "";
	std::vector<std::vector<std::string>> data;
	std::vector<std::string> arr;  /* 临时使用 */
	char word[16] = {0}; /* FIXME: 这个栈有越界风险 */
	int top = 0;
	int count = 0;

	void _parse(std::istream& src) {	/* 进行脚本解析 */
		_clear_tmp();
		char c = 0;
		bool end_flag = false;
		bool comment_flag = false;
		bool named_flag = false;
		while (!end_flag) {
			end_flag = !(bool)src.get(c);
			std::cout << c;
			/*---- NAME 命名 ----*/
			if (!comment_flag && !named_flag && c == '@') {
				named_flag = true;
			}
			else if (named_flag && c == '\n') {
				comment_flag = false;
				named_flag = false;
				count ++;
			}
			else if (named_flag) {
				name += c;
			}
			/*---- COMMENT 注释 ----*/
			else if (!comment_flag && c == '#') {	/* 注释以井号#开头 */
				comment_flag = true;
			}
			else if (comment_flag && c == '\n') {	/* 注释以换行符结尾 */
				comment_flag = false;
				count ++;
			}
			else if (comment_flag && c != '\n') {	/* 注释内容被忽略 */
				;
			}
			/*---- DATA 数据 ----*/
			else if (!comment_flag && (c == ' ' || c == '\t')) {
				if (top > 0) _submit_word();
			}
			else if (!comment_flag && c == ';') {
				if (top > 0) _submit_word();
				if (arr.size() > 0) _submit_arr();
			}
			else if (!comment_flag && (c == '\n' || c == '\r')) {
				count ++;
			}
			else if (!comment_flag) {
				word[top++] = c;
			}
		}
	}

	void _clear_tmp() {	/* 清空临时容器变量 */
		word[0] = '\0';
		top = 0;
		arr.clear();
	}


	void _clear_all() {	/* 请空所有临时变量 */
		word[0] = '\0';
		top = 0;
		arr.clear();
		data.clear();
	}

	void _submit_word() {	/* 提交项目 */
		word[top] = '\0';
		auto item = std::string(word);
		arr.push_back(item);
		word[0] = '\0';
		top =0;
	}

	void _submit_arr() {	/* 提交数组 */
		data.push_back(arr);
		arr.clear();
	}
};
}
