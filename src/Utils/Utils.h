//
// Created by gifi on 05.12.2021.
//

#pragma once

#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <map>

char* ReadFile(const std::string&  name);
void ImportObj(const std::string& filePath, std::vector<float>& verts);
void ImportObj5(const std::string& filePath, std::vector<float>& verts);
std::map<std::string, int> ImportObjMultiTex(const std::string& filePath, std::vector<float>& verts);
void ExportObj(const std::string& filePath, std::vector<float>& verts, std::vector<float>& tex, std::vector<int>& indexes, bool v2);