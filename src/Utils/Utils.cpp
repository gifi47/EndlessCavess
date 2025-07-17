//
// Created by gifi on 05.12.2021.
//

#include "Utils.h"

char* ReadFile(const std::string& name){
    std::ifstream fin(name, std::ios::in | std::ios::binary);
    if (fin) {
        fin.seekg(0, std::ios::end);
#ifdef __GNUC__
	long long length{fin.tellg()};
#else
        int64_t length{fin.tellg()};
#endif
        fin.seekg(0, std::ios::beg);
        char *data{new char[length + 1]};
        fin.read(data, length);
        data[length] = '\0';
        fin.close();
        return data;
    }
    throw(errno);
}

void ImportObj(const std::string& filePath, std::vector<float>& verts) {
    std::ifstream fin(filePath, std::ios::in);
    std::string tp;
    std::vector<float> positions;
    std::vector<float> texCoords;
    std::vector<int> positionsIndexes;
    while (fin >> tp) {
        //std::cout << tp << "\n";
        if (tp.size() == 1) {
            switch (tp[0])
            {
            case 'v':
                float x, y, z;
                fin >> x >> y >> z;
                positions.push_back(x);
                positions.push_back(y);
                positions.push_back(z);

                //positions.push_back((rand() % 101) / 100.0);
                //positions.push_back((rand() % 101) / 100.0);
                //positions.push_back((rand() % 101) / 100.0);

                positions.push_back(0.23);
                positions.push_back(0.1);
                positions.push_back(0.78);
                break;
            case 'f':
                for (int i7 = 0; i7 < 3; i7++) {
                    int p1, p2, p3;
                    char temp;
                    fin >> p1 >> temp >> p2 >> temp >> p3;
                    positionsIndexes.push_back(p1 - 1);
                    positionsIndexes.push_back(p2 - 1);
                    //positionsIndexes.push_back(p2 - 1);
                    //positionsIndexes.push_back(p3 - 1);
                    //std::cout << p1 << " " << p2 << " " << p3 << "\n";
                }
                break;
            default:
                std::string s;
                std::getline(fin, s);
                //std::cout << s << "\n";
                break;
            }
        }
        else {
            if (tp[0] == 'v' && tp[1] == 't') {
                float t1, t2;
                fin >> t1 >> t2;
                texCoords.push_back(t1);
                texCoords.push_back(t2);
            }
            else {
                std::string s;
                std::getline(fin, s);
                //std::cout << s << "\n";
            }
        }
    }
    verts.resize(0);
    for (int i = 0; i < positionsIndexes.size(); i+=2) {
        int ind = positionsIndexes[i] * 6;
        int indTexCoord = positionsIndexes[i + 1] * 2;
        verts.push_back(positions[ind]);
        verts.push_back(positions[ind + 1]);
        verts.push_back(positions[ind + 2]);
        verts.push_back(positions[ind + 3]);
        verts.push_back(positions[ind + 4]);
        verts.push_back(positions[ind + 5]);
        verts.push_back(texCoords[indTexCoord]);
        verts.push_back(texCoords[indTexCoord + 1]);
    }
    fin.close();
    std::cout << "Loaded " << filePath << " verts loaded:" << verts.size()/8 << "\n";
}

void ImportObj5(const std::string& filePath, std::vector<float>& verts)
{
    std::ifstream fin(filePath, std::ios::in);
    std::string tp;
    std::vector<float> positions;
    std::vector<float> texCoords;
    std::vector<int> positionsIndexes;
    while (fin >> tp) {
        //std::cout << tp << "\n";
        if (tp.size() == 1) {
            switch (tp[0])
            {
            case 'v':
                float x, y, z;
                fin >> x >> y >> z;
                positions.push_back(x);
                positions.push_back(y);
                positions.push_back(z);
                break;
            case 'f':
                for (int i7 = 0; i7 < 3; i7++) {
                    int p1, p2, p3;
                    char temp;
                    fin >> p1 >> temp >> p2 >> temp >> p3;
                    positionsIndexes.push_back(p1 - 1);
                    positionsIndexes.push_back(p2 - 1);
                    //positionsIndexes.push_back(p2 - 1);
                    //positionsIndexes.push_back(p3 - 1);
                    //std::cout << p1 << " " << p2 << " " << p3 << "\n";
                }
                break;
            default:
                std::string s;
                std::getline(fin, s);
                //std::cout << s << "\n";
                break;
            }
        }
        else {
            if (tp[0] == 'v' && tp[1] == 't') {
                float t1, t2;
                fin >> t1 >> t2;
                texCoords.push_back(t1);
                texCoords.push_back(t2);
            }
            else {
                std::string s;
                std::getline(fin, s);
                //std::cout << s << "\n";
            }
        }
    }
    verts.resize(0);
    for (int i = 0; i < positionsIndexes.size(); i += 2) {
        int ind = positionsIndexes[i] * 3;
        int indTexCoord = positionsIndexes[i + 1] * 2;
        verts.push_back(positions[ind]);
        verts.push_back(positions[ind + 1]);
        verts.push_back(positions[ind + 2]);
        verts.push_back(texCoords[indTexCoord]);
        verts.push_back(texCoords[indTexCoord + 1]);
    }
    fin.close();
    std::cout << "Loaded " << filePath << " verts loaded:" << verts.size() / 5 << "\n";
}

std::map<std::string, int> ImportObjMultiTex(const std::string& filePath, std::vector<float>& verts) {
    std::ifstream fin(filePath, std::ios::in);
    std::string tp;
    std::vector<float> positions;
    std::vector<float> texCoords;
    std::vector<int> positionsIndexes;

    std::map<std::string, int> materials;
    int materialId = 0;
    float r = ((rand() % 101) / 100.0);
    float g = ((rand() % 101) / 100.0);
    float b = ((rand() % 101) / 100.0);
    while (fin >> tp) {
        if (tp.size() == 1) {
            switch (tp[0])
            {
            case 'v':
                // Position XYZ
                float x, y, z;
                fin >> x >> y >> z;
                positions.push_back(x);
                positions.push_back(y);
                positions.push_back(z);

                // Color RGB

                r = ((rand() % 101) / 100.0);
                g = ((rand() % 101) / 100.0);
                b = ((rand() % 101) / 100.0);

                positions.push_back(r);
                positions.push_back(g);
                positions.push_back(b);
                break;
            case 'f':
                for (int i7 = 0; i7 < 3; i7++) {
                    int p1, p2, p3;
                    char temp;
                    fin >> p1 >> temp >> p2 >> temp >> p3;
                    positionsIndexes.push_back(p1 - 1);
                    positionsIndexes.push_back(p2 - 1);
                    positionsIndexes.push_back(materialId);
                }
                break;
            default:
                std::string s;
                std::getline(fin, s);
                //std::cout << s << "\n";
                break;
            }
        }
        else {
            if (tp[0] == 'v' && tp[1] == 't') {
                float t1, t2;
                fin >> t1 >> t2;
                texCoords.push_back(t1);
                texCoords.push_back(t2);
            }
            else if (tp[0] == 'u' && tp[1] == 's') {
                std::string materialName;
                fin >> materialName;
                if (!materials.contains(materialName)) {
                    materials[materialName] = materials.size();
                }
                materialId = materials[materialName];
            }
            else {
                std::string s;
                std::getline(fin, s);
                //std::cout << s << "\n";
            }
        }
    }
    verts.resize(0);
    for (int i = 0; i < positionsIndexes.size(); i += 3) {
        int ind = positionsIndexes[i] * 6;
        int indTexCoord = positionsIndexes[i + 1] * 2;
        verts.push_back(positions[ind]);
        verts.push_back(positions[ind + 1]);
        verts.push_back(positions[ind + 2]);
        verts.push_back(positions[ind + 3]);
        verts.push_back(positions[ind + 4]);
        verts.push_back(positions[ind + 5]);
        verts.push_back(texCoords[indTexCoord]);
        verts.push_back(texCoords[indTexCoord + 1]);
        verts.push_back(positionsIndexes[i + 2]);
    }
    fin.close();
    std::cout << "Loaded " << filePath << " verts loaded:" << verts.size()/9 << "\n";
    return materials;
}

void ExportObj(const std::string& filePath, std::vector<float>& verts, std::vector<float>& tex, std::vector<int>& indexes, bool v2 = false) {
    std::ofstream fout(filePath, std::ios::out);
    if (v2) {
        for (int i = 0; i < verts.size(); i += 2) {
            fout << "v " << verts[i] << " " << verts[i + 1] << " 0\n";
        }
    }
    else {
        for (int i = 0; i < verts.size(); i += 3) {
            fout << "v " << verts[i] << " " << verts[i + 1] << " " << verts[i + 2] << "\n";
        }
    }
    for (int i = 0; i < tex.size(); i += 2) {
        fout << "vt " << tex[i] << " " << tex[i + 1] << "\n";
    }
    for (int i = 0; i < indexes.size(); i += 3) {
        fout << "f ";
        for (int j = 0; j < 3; j++) {
            fout << indexes[i + j] + 1 << "/" << indexes[i + j] + 1 << "/" << indexes[i + j] + 1 << " ";
        }
        fout << "\n";
    }
    fout.close();
}