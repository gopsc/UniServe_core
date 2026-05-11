#pragma once
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <unistd.h>
#include "cn/中文化.hpp"
#include "hd/PCA9685.h"

namespace qing {
class ActParser{  /* 动作脚本解析器 */
public:

    void fromStdin() {	/* 解析标准输入中的动作脚本 */
        count  = 0;
        clear_all();
        parse(std::cin);
    }

    void fromFile(const std::string& path) {	/* 解析文件输入流中的动作脚本 */
        count  = 0;
        std::ifstream file(path.c_str(), std::ios::in);
        if (!file.is_open())
            throw std::runtime_error("Failed to open file");
        clear_all();
        parse(file);
        file.close();
    }

    void fromStr(const std::string& script) {	/* 解析字符串流中的动作脚本 */
        std::stringstream ss(script);
        count = 0;
        clear_all();
	parse(ss);
    }

    void play() {	/* 执行刚刚解析出来的机器人和动作 */
        if (devices.empty() || matrix.empty())
            throw std::runtime_error("no action or device");
        action(devices, matrix);
    }

    std::vector<float> get_max() {	/* 获得机器人的最大值向量， FIXME: 把逻辑写在外面 */
        auto res  = std::vector<float>();
        for (auto& item: this->devices)
            res.push_back(item.get_max());
        return res;	/* 这个数组中包含机器人每个舵机的角度上限 */
    }

    std::vector<float> now() {	/* 获取当前这一组动作 */
        if (count >= matrix.size())
            throw std::runtime_error("now finished");
        return matrix[count];
    }

    void next() {	/* 执行下一组动作 */
        if (count >= matrix.size())
            throw std::runtime_error("no next action");
        auto tmp_vec = matrix[count++];
        auto tmp_matrix = std::vector<std::vector<float>> ({tmp_vec});
        action(devices, tmp_matrix);
    }

    void next(std::vector<float>& act) {	/* 输入下一个动作执行 */
        count++;
        auto vv = std::vector<std::vector<float>> ({act});
        action(devices, vv);
    }

private:
    char word[16] = {0}; /* FIXME: 这个栈有越界风险 */
    int top = 0;
    int count = 0;
    std::vector<float> arr;  /* 临时使用 */
    std::vector<std::vector<float>> matrix; /* 解析出的整套动作 */
    std::vector<Servos> devices; /* 机器人抽象 */
    void parse(std::istream& src) {  /* NOTE: 修改为非静态成员函数能够减少行数 FIXME: 改为使用C++风格的文件操作 */
        clear_tmp();
        char c = 0;
        bool end_flag  = true;
        bool comment_flag  = false;
        bool obj_flag = false;
        while (end_flag) { /* 255 for arm */
            end_flag = (bool)src.get(c);
            //std::cout<<c;
            if ((!comment_flag && c >= '0' && c <= '9') || c == '+' || c == '-') {  /* 读取数字 */
                word[top++] = c;
                std::cout<<c;
            }
            else if (!comment_flag && c == ' ') {  /* 每个角度以空格结尾 */
                word[top] = '\0';
                submit_servo();
            }
            else if (!comment_flag && (c == '\r' || c == '\n')) {  /* 忽略回车 */
                ;
            }
	    else if (!comment_flag && c == '#') {  /* 注释以#开头 */
                comment_flag = true;
            }
	    else if (comment_flag && c == '\n') {  /* 注释以换行结尾 */
                comment_flag = false;
            }
	    else if (comment_flag && c != '\n') {  /* 忽略注释 */
                ;
            }
	    else if (!comment_flag && !obj_flag && top == 0 && c == '!') {  /* 构建模式 */
                obj_flag = true;
            }
            else if (!comment_flag && obj_flag && (c==';' || end_flag)) {  /* 构建机器人 */
                word[top] = '\0';
                submit_servo();
                submit_devices();
                obj_flag = false;
            }   
	    else if (!comment_flag && !obj_flag && (c == ';' || end_flag)) {   /* 每个动作以分号结尾 */
                word[top] = '\0';
                submit_servo();
                submit_devices();
            }
            else throw std::runtime_error(
                    "Invalid action: " + std::to_string(int(c)));

        }
    }

    void submit_servo() {
        if (top > 0) {
            float angle = (float)atoi(word);
            arr.push_back(angle);
            std::cout << angle << std::endl;
        }
        word[0] = '\0';
        top = 0;
    }

    void submit_act() {
        if (!arr.empty()) {
            std::cout<<"recive acts: " << arr.size() << std::endl;
            matrix.push_back(arr);
            arr =  std::vector<float>{};
        }
    }

    void submit_devices() {
        if (!arr.empty()) {
            std::cout << "recive devices: " << arr.size() << std::endl;
            devices = deviceFactory(arr);
            arr = std::vector<float>{};
        }
    }

    void clear_tmp() {
        word[0] = '\0';
        top = 0;
        arr = std::vector<float> {};
    }

    void clear_all() {
        word[0] = '\0';
        top = 0;
        arr = std::vector<float> {};
        matrix = std::vector<std::vector<float>> {};
        devices = std::vector<Servo> {};
    }

    /* 机器人工厂，FIXME: 这里可以做成舵机工厂就可以了， */
    static std::vector<Servo> devicesFactory(const std::vector<float>& maxs) {
        std::vector<Servo> devices;
        for (int i=0; i < maxs.size(); ++i)
            devices.push_back(Servo(i, std::abs(maxs[i]), 0, (maxs[i] >= 0) ? false : true));
        return devices;
    }

    static void _do(std::vector<Servc>& devices, std::vector<float>& act)	/* 机器人移动，输入一组舵机对象和一组目标角度 */
        if (devices.size() + 1 != acts.size())
            throw std::runtime_error("act and device not match");

        for (int i=0; i<devices.size(); ++i)	/* FIXME: 随机下标以消除先后时间差 */
            devices[i].setAngle(act[i]);
 
        constexpr int scale  = 1000;	/* 最后一组数据是毫秒延时 */
	usleep(act[act.size()-1] * scale);
    }
    
    static void action(	/* 机器人整套动作 */
        std::vector<Servo>& devices, std::vector<std::vector<float>>& acts)
    {
        for (auto act: acts)
            _do(devices, act);
    }
};
}
