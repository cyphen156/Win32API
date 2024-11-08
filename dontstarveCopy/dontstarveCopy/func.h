#pragma once
class Object;

void CreateObj(Object* createObj, ObjType objType);
void DeleteObj(Object* createObj);
void ChangeScene(SceneType next);




template<typename T>
void Safe_Delete_Vec(std::vector<T>& _vec)
{
    for (size_t i = 0; i < _vec.size(); ++i)
    {
        delete _vec[i];
    }
    _vec.clear();
}


template<typename T1, typename T2>
void Safe_Delete_Map(std::map<T1, T2>& _map)
{
    typename std::map<T1, T2>::iterator iter = _map.begin();

    for (; iter != _map.end(); ++iter)
    {
        if (iter->second != nullptr)
        {
            delete iter->second;
        }
    }
    _map.clear();
}